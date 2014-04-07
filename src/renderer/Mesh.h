
#ifndef H_DER_MESH_H
#define H_DER_MESH_H

#include "../Types.h"

namespace der
{

    class VertexBuffer;
    class IndexBuffer;

    struct SubMesh
    {
        size_t m_start_index;
        size_t m_index_count;
    };

    class Mesh
    {
    public:
        static const size_t MAX_SUB_MESHES = 8;
    public:
        Mesh();
        ~Mesh();

        void set_buffers(VertexBuffer *vb, IndexBuffer *ib);
        VertexBuffer* get_vertex_buffer();
        IndexBuffer* get_index_buffer();

        void add_submesh(size_t start_index, size_t index_count);
        const SubMesh& get_submesh(size_t index) const;
        size_t get_submesh_count() const;

    private:
        VertexBuffer *m_vertex_buffer;
        IndexBuffer *m_index_buffer;

        SubMesh m_submeshes[MAX_SUB_MESHES];
        size_t m_submesh_count;
    };

} // der

#endif // H_DER_MESH_H

