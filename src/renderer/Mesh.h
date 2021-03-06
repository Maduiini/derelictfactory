
#ifndef H_DER_MESH_H
#define H_DER_MESH_H

#include "../Types.h"
#include "../math/Aabb.h"

namespace der
{

    typedef size_t ResourceID;

    class VertexBuffer;
    class IndexBuffer;
    class VertexArrayObject;

    struct SubMesh
    {
        size_t m_start_index;
        size_t m_index_count;
        ResourceID m_material;
    };

    class Mesh
    {
    public:
        static const size_t MAX_SUB_MESHES = 16;
    public:
        Mesh();
        ~Mesh();

        void set_buffers(VertexBuffer *vb, IndexBuffer *ib);
        VertexBuffer* get_vertex_buffer();
        IndexBuffer* get_index_buffer();
        VertexArrayObject* get_vao();

        void add_submesh(size_t start_index, size_t index_count, ResourceID material);
        const SubMesh& get_submesh(size_t index) const;
        size_t get_submesh_count() const;

        void set_bounding_box(const Aabb &aabb);
        const Aabb& get_bounding_box() const;

    private:
        VertexBuffer *m_vertex_buffer;
        IndexBuffer *m_index_buffer;
        VertexArrayObject *m_vao;

        SubMesh m_submeshes[MAX_SUB_MESHES];
        size_t m_submesh_count;

        Aabb m_bounding_box;
    };

} // der

#endif // H_DER_MESH_H

