
#include "MeshRenderer.h"
#include "Mesh.h"

#include "IndexBuffer.h"
#include "VertexArrayObject.h"

namespace der
{

    MeshRenderer::MeshRenderer()
        : m_mesh(0)
    { }

    void MeshRenderer::render()
    {
        IndexBuffer *ib = m_mesh->get_index_buffer();
        m_mesh->get_vao()->bind();
        ib->bind();
        for (size_t i = 0; i < m_mesh->get_submesh_count(); i++)
        {
            const SubMesh &submesh = m_mesh->get_submesh(i);

            ib->draw_triangles(submesh.m_start_index, submesh.m_index_count);
        }
    }

    void MeshRenderer::set_mesh(Mesh *mesh)
    { m_mesh = mesh; }

    Mesh* MeshRenderer::get_mesh()
    { return m_mesh; }

} // der
