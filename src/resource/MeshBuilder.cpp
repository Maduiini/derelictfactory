
#include "MeshBuilder.h"
#include "../renderer/VertexBuffer.h"
#include "../renderer/IndexBuffer.h"
#include "../renderer/VertexArrayObject.h"
#include "../renderer/Mesh.h"

#include "Resource.h"

#include "../Debug.h"

#include <string>

namespace der
{

    MeshBuilder::MeshBuilder()
        : m_start_index(0)
        , m_bounding_box_reset(true)
    { }

    void MeshBuilder::reserve(uint32_t vertices, uint32_t faces)
    {
        m_positions.reserve(vertices);
        m_normals.reserve(vertices);
        m_tex_coords.reserve(vertices);
        m_faces.reserve(faces);
    }

    void MeshBuilder::build(Mesh &mesh)
    {
        calculate_tangents();

        VertexBuffer *vbuffer = new VertexBuffer();
        vbuffer->bind();
        vbuffer->resize(m_vertices.size() * sizeof(Vertex), false);
        vbuffer->write(0, vbuffer->get_size(), static_cast<const void*>(m_vertices.data()));

        IndexBuffer *ibuffer = new IndexBuffer();
//        if (m_faces.size() * 3 > 0xffff)
            ibuffer->set_index_type(IndexBuffer::Index32);
        ibuffer->bind();
        ibuffer->resize(m_faces.size() * sizeof(Face), false);
        ibuffer->write(0, ibuffer->get_size(), static_cast<const void*>(m_faces.data()));

        mesh.set_buffers(vbuffer, ibuffer);

        VertexArrayObject *vao = mesh.get_vao();

        const size_t offset_pos = 0;
        const size_t offset_tc = sizeof(float) * 3;
        const size_t offset_no = sizeof(float) * 5;
        const size_t offset_tan = sizeof(float) * 8;
        const size_t stride = sizeof(Vertex);

        vao->bind();
        vao->set_attribute(VertexAttrib::Position, offset_pos, stride, 3);
        vao->set_attribute(VertexAttrib::TexCoord, offset_tc, stride, 2);
        vao->set_attribute(VertexAttrib::Normal, offset_no, stride, 3);
        vao->set_attribute(VertexAttrib::Tangent, offset_tan, stride, 4);

        for (SubMesh &submesh : m_sub_meshes)
        {
            const std::string material_name = std::string(submesh.material) + ".material";
            const ResourceID material = make_resource(material_name.c_str());
            mesh.add_submesh(submesh.start_index, submesh.index_count, material);
        }
        mesh.set_bounding_box(m_bounding_box);
    }

    uint32_t MeshBuilder::get_position_count() const
    { return m_positions.size(); }

    uint32_t MeshBuilder::get_normal_count() const
    { return m_normals.size(); }

    uint32_t MeshBuilder::get_tex_coord_count() const
    { return m_tex_coords.size(); }

    uint32_t MeshBuilder::get_vertex_count() const
    { return m_vertices.size(); }

    void MeshBuilder::add_position(const Vector3 &position)
    { m_positions.push_back(position); }

    void MeshBuilder::add_normal(const Vector3 &normal)
    { m_normals.push_back(normal); }

    void MeshBuilder::add_tex_coord(const Vector2 &tex_coord)
    { m_tex_coords.push_back(tex_coord); }

    bool MeshBuilder::add_face(uint32_t v1, uint32_t v2, uint32_t v3,
                               uint32_t n1, uint32_t n2, uint32_t n3,
                               uint32_t tc1, uint32_t tc2, uint32_t tc3)
    {
        const uint32_t i1 = get_vertex_index(v1, tc1, n1);
        const uint32_t i2 = get_vertex_index(v2, tc2, n2);
        const uint32_t i3 = get_vertex_index(v3, tc3, n3);
        return add_face(i1, i2, i3);
    }

    bool MeshBuilder::add_face(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4,
                               uint32_t n1, uint32_t n2, uint32_t n3, uint32_t n4,
                               uint32_t tc1, uint32_t tc2, uint32_t tc3, uint32_t tc4)
    {
        const uint32_t i1 = get_vertex_index(v1, tc1, n1);
        const uint32_t i2 = get_vertex_index(v2, tc2, n2);
        const uint32_t i3 = get_vertex_index(v3, tc3, n3);
        const uint32_t i4 = get_vertex_index(v4, tc4, n4);
        return add_face(i1, i2, i3) && add_face(i1, i3, i4);
    }

    void MeshBuilder::add_sub_mesh(const char * const name, const char * const material)
    {
        const uint32_t index_count = m_faces.size() * 3 - m_start_index;

        add_sub_mesh(name, material, m_start_index, index_count);
        m_start_index += index_count;
    }

    template <size_t N>
    void copy_max_n(char (&dst)[N], const char *src)
    {
        for (size_t i = 0; i < N - 1; i++)
        {
            dst[i] = (src && *src) ? *src : '\0';
            if (src) src++;
        }
        dst[N - 1] = '\0';
    }

    void MeshBuilder::add_sub_mesh(const char * const name, const char * const material,
                                   uint32_t start_index, uint32_t index_count)
    {
        SubMesh submesh;
        submesh.start_index = start_index;
        submesh.index_count = index_count;
        copy_max_n(submesh.name, name);
        copy_max_n(submesh.material, material);
        m_sub_meshes.push_back(submesh);
    }


    bool MeshBuilder::add_face(uint32_t v1, uint32_t v2, uint32_t v3)
    {
        Face face;
        face.i1 = v1;
        face.i2 = v2;
        face.i3 = v3;
        if (!validate_face(face)) return false;

        m_faces.push_back(face);
        update_bounding_box(face);
        return true;
    }

    uint32_t MeshBuilder::get_vertex_index(uint32_t vert_ind, uint32_t tc_ind, uint32_t no_ind)
    {
        VertexIndexElement element(vert_ind, tc_ind, no_ind);

        VertexSet::iterator it = m_vertex_set.find(element);
        if (it != m_vertex_set.end())
        {
            const VertexIndexElement &ind = *it;
            return ind.m_index;
        }
        else
        {
            element.m_index = m_vertices.size();
            m_vertex_set.insert(element);

            Vertex vertex;
            vertex.normal    = Vector3(0.0f, 1.0f, 0.0f);
            vertex.tex_coord = Vector2(0.0f, 0.0f);
            vertex.tangent   = Vector4(0.0f, 0.0f, 1.0f, 1.0f);

            if (vert_ind != INVALID_INDEX && vert_ind < m_positions.size())
                vertex.position = m_positions.at(vert_ind);
            else
                return INVALID_INDEX;

            if (no_ind != INVALID_INDEX)
            {
                if (no_ind < m_normals.size())
                    vertex.normal = m_normals.at(no_ind);
                else
                    ; // error/warning
            }

            if (tc_ind != INVALID_INDEX)
            {
                if (tc_ind < m_tex_coords.size())
                    vertex.tex_coord = m_tex_coords.at(tc_ind);
                else
                    ; // error/warning
            }

            m_vertices.push_back(vertex);
            return element.m_index;
        }
    }

    void MeshBuilder::update_bounding_box(const Face &face)
    {
        update_bounding_box(face.i1);
        update_bounding_box(face.i2);
        update_bounding_box(face.i3);
    }

    void MeshBuilder::update_bounding_box(uint32_t vertex_index)
    {
        const Vertex &vertex = m_vertices.at(vertex_index);
        if (m_bounding_box_reset)
        {
            m_bounding_box.m_min = m_bounding_box.m_max = vertex.position;
            m_bounding_box_reset = false;
        }
        else
        {
            m_bounding_box.add_point(vertex.position);
        }
    }

    void MeshBuilder::calculate_tangents()
    {
        std::vector<Vector3> tan1;
        std::vector<Vector3> tan2;

        tan1.resize(m_vertices.size(), Vector3::zero);
        tan2.resize(m_vertices.size(), Vector3::zero);

        for (size_t i = 0; i < m_faces.size(); i++)
        {
            const Face &face = m_faces.at(i);
            const Vertex &v1 = m_vertices.at(face.i1);
            const Vertex &v2 = m_vertices.at(face.i2);
            const Vertex &v3 = m_vertices.at(face.i3);

            Vector3 edge1 = v1.position - v2.position;
            Vector3 edge2 = v1.position - v3.position;
            Vector2 dtc2 = v1.tex_coord - v3.tex_coord;
            Vector2 dtc1 = v1.tex_coord - v2.tex_coord;

            float l = dtc1.y * dtc2.x - dtc1.x * dtc2.y;
            if (std::abs(l) > 0.0f)
            {
                Vector3 t1dir = (edge2 * dtc1.y - edge1 * dtc2.y) / l;
                Vector3 t2dir = (edge1 * dtc2.x - edge2 * dtc1.x) / l;

                tan1.at(face.i1) += t1dir;
                tan1.at(face.i2) += t1dir;
                tan1.at(face.i3) += t1dir;

                tan2.at(face.i1) += t2dir;
                tan2.at(face.i2) += t2dir;
                tan2.at(face.i3) += t2dir;
            }
        }

        for (size_t i = 0; i < m_vertices.size(); i++)
        {
            Vertex &v = m_vertices.at(i);
            Vector3 t1 = tan1.at(i);
            Vector3 t2 = tan2.at(i);

            Vector3 tan = t1 - v.normal * t1.dot( v.normal );
            tan.normalize();

            if (tan.length2() < 0.2f) tan = Vector3(1.0f, 0.0f, 0.0f);

            // Left handed coordinate system => -(N x T) instead of (N x T)
            float w = (-v.normal.cross(tan).dot(t2) > 0.0f)
                    ? -1.0f : 1.0f;

            DER_ASSERT(tan.dot(tan) != 0.0);

            v.tangent = Vector4(tan, w);
        }
    }

    // static
    bool MeshBuilder::validate_face(const Face &face)
    {
        return face.i1 != INVALID_INDEX && face.i2 != INVALID_INDEX && face.i3 != INVALID_INDEX;
    }

} // der
