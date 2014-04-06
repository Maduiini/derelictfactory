
#ifndef H_DER_MESH_H
#define H_DER_MESH_H

namespace der
{

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

        void set_buffers(VertexBuffer *vb, IndexBuffer *ib);

        void add_sub_mesh(size_t start_index, size_t index_count);
        const SubMesh& get_sub_mesh(size_t index) const;
        size_t get_submesh_count() const;

    private:
        VertexBuffer *m_vertex_buffer;
        IndexBuffer *m_index_buffer;

        SubMesh m_sub_meshes[MAX_SUB_MESHES];
        size_t m_submesh_count;
    };

} // der

#endif // H_DER_MESH_H

