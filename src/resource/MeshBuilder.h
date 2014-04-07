
#ifndef H_DER_MESH_BUILDER_H
#define H_DER_MESH_BUILDER_H

#include "../math/Math.h"
#include "../Types.h"

#include <vector>
#include <set>

namespace der
{

    class Mesh;

    class MeshBuilder
    {
    public:
        MeshBuilder();

        void reserve(uint32_t vertices, uint32_t faces);

        void build(Mesh &mesh);

        uint32_t get_position_count() const;
        uint32_t get_normal_count() const;
        uint32_t get_tex_coord_count() const;
        uint32_t get_vertex_count() const;

        void add_position(const Vector3 &position);
        void add_normal(const Vector3 &normal);
        void add_tex_coord(const Vector2 &tex_coord);
        bool add_face(uint32_t v1, uint32_t v2, uint32_t v3,
                      uint32_t n1, uint32_t n2, uint32_t n3,
                      uint32_t tc1, uint32_t tc2, uint32_t tc3);
        bool add_face(uint32_t v1, uint32_t v2, uint32_t v3, uint32_t v4,
                      uint32_t n1, uint32_t n2, uint32_t n3, uint32_t n4,
                      uint32_t tc1, uint32_t tc2, uint32_t tc3, uint32_t tc4);

        void add_sub_mesh(const char * const name, const char * const material);
        void add_sub_mesh(const char * const name, const char * const material,
                          uint32_t startIndex, uint32_t indexCount);

    private:
        struct Face;

        bool add_face(uint32_t v1, uint32_t v2, uint32_t v3);

        uint32_t get_vertex_index(uint32_t vert_ind, uint32_t tc_ind, uint32_t no_ind);
        void update_bounding_box(const Face &face);
        void update_bounding_box(uint32_t vertex_index);

        void calculate_tangents();

        static bool validate_face(const Face &face);

    private:
        struct Vertex
        {
            Vector3 position;
            Vector2 tex_coord;
            Vector3 normal;
            Vector4 tangent;
        };

        static const uint32_t INVALID_INDEX = -1;

        struct Face
        {
            uint32_t i1, i2, i3;
        };

        static const uint32_t MAX_STRING_LENGTH = 32;

        struct SubMesh
        {
            uint32_t    start_index;
            uint32_t    index_count;
            char        name[MAX_STRING_LENGTH];
            char        material[MAX_STRING_LENGTH];
        };

        typedef std::vector<Vertex> VertexList;
        typedef std::vector<Face> FaceList;
        typedef std::vector<SubMesh> SubMeshList;

        struct VertexIndexElement
        {
            VertexIndexElement(uint32_t vi, uint32_t tci, uint32_t noi)
                : m_vertex_index(vi), m_tcoord_index(tci), m_normal_index(noi)
                , m_index(INVALID_INDEX)
            { }

            bool operator < (const VertexIndexElement &other) const
            {
                if (m_vertex_index < other.m_vertex_index) return true;
                if (m_vertex_index == other.m_vertex_index)
                {
                    if (m_tcoord_index < other.m_tcoord_index)
                        return true;

                    return m_tcoord_index == other.m_tcoord_index &&
                            m_normal_index < other.m_normal_index;
                }
                return false;
            }

            uint32_t m_vertex_index;
            uint32_t m_tcoord_index;
            uint32_t m_normal_index;
            uint32_t m_index;
        };

        typedef std::set<VertexIndexElement> VertexSet;
        VertexSet m_vertex_set;

        typedef std::vector<Vector3> VertPosList;
        typedef std::vector<Vector3> VertNormalList;
        typedef std::vector<Vector2> VertTCoordList;

        VertPosList     m_positions;
        VertTCoordList  m_tex_coords;
        VertNormalList  m_normals;

        VertexList      m_vertices;
        FaceList        m_faces;
        SubMeshList     m_sub_meshes;

        uint32_t        m_start_index;

//        AABBox          m_bounding_box;
        bool            m_bounding_box_reset;
    };

} // der

#endif // H_DER_MESH_BUILDER_H

