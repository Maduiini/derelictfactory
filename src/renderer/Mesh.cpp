
#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArrayObject.h"
#include "../Debug.h"

namespace der
{

    Mesh::Mesh()
        : m_vertex_buffer(nullptr)
        , m_index_buffer(nullptr)
        , m_vao(nullptr)
        , m_submesh_count(0)
    {
        m_vao = new VertexArrayObject();
    }

    Mesh::~Mesh()
    {
        delete m_vertex_buffer;
        delete m_index_buffer;
        delete m_vao;
    }

    void Mesh::set_buffers(VertexBuffer *vb, IndexBuffer *ib)
    {
        m_vertex_buffer = vb;
        m_index_buffer = ib;
    }

    VertexBuffer* Mesh::get_vertex_buffer()
    { return m_vertex_buffer; }

    IndexBuffer* Mesh::get_index_buffer()
    { return m_index_buffer; }

    VertexArrayObject* Mesh::get_vao()
    { return m_vao; }

    void Mesh::add_submesh(size_t start_index, size_t index_count)
    {
        DER_ASSERT(m_submesh_count < MAX_SUB_MESHES);
        m_submeshes[m_submesh_count].m_start_index = start_index;
        m_submeshes[m_submesh_count].m_index_count = index_count;
        m_submesh_count++;
    }

    const SubMesh& Mesh::get_submesh(size_t index) const
    {
        DER_ASSERT(index < m_submesh_count);
        return m_submeshes[index];
    }

    size_t Mesh::get_submesh_count() const
    { return m_submesh_count; }

} // der
