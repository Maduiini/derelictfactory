
#include "MeshRenderer.h"
#include "Mesh.h"

#include "IndexBuffer.h"
#include "VertexArrayObject.h"

#include "Material.h"
#include "../resource/ResourceCache.h"

namespace der
{

    MeshRenderer::MeshRenderer()
        : m_mesh(0)
    { }

    void MeshRenderer::render(Graphics *graphics, ResourceCache &cache)
    {
        Mesh *mesh = cache.get<Mesh>(m_mesh);
        if (!mesh) return;

        IndexBuffer *ib = mesh->get_index_buffer();
        mesh->get_vao()->bind();
        ib->bind();
        for (size_t i = 0; i < mesh->get_submesh_count(); i++)
        {
            const SubMesh &submesh = mesh->get_submesh(i);

            Material *material = cache.get<Material>(submesh.m_material);

            if (material)
            {
                material->use(graphics, cache);
                ib->draw_triangles(submesh.m_start_index, submesh.m_index_count);
            }
        }
    }

    void MeshRenderer::set_mesh(ResourceID mesh)
    { m_mesh = mesh; }

    ResourceID MeshRenderer::get_mesh()
    { return m_mesh; }

} // der
