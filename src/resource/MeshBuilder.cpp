
#include "MeshBuilder.h"
#include "../renderer/VertexBuffer.h"
#include "../renderer/IndexBuffer.h"
#include "../renderer/VertexArrayObject.h"
#include "../renderer/Mesh.h"

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
            mesh.add_submesh(submesh.start_index, submesh.index_count);
        }
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

    void MeshBuilder::add_sub_mesh(const char * const name, const char * const material,
                                   uint32_t start_index, uint32_t index_count)
    {
        SubMesh submesh;
        submesh.start_index = start_index;
        submesh.index_count = index_count;
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

    }

    void MeshBuilder::calculate_tangents()
    {

    }

    // static
    bool MeshBuilder::validate_face(const Face &face)
    {
        return face.i1 != INVALID_INDEX && face.i2 != INVALID_INDEX && face.i3 != INVALID_INDEX;
    }

} // der
