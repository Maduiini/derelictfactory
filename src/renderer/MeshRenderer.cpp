
#include "MeshRenderer.h"
#include "Mesh.h"
#include "Graphics.h"
#include "Renderer.h"

#include "IndexBuffer.h"
#include "VertexArrayObject.h"

#include "Material.h"
#include "../resource/ResourceCache.h"

namespace der
{

    MeshRenderer::MeshRenderer()
        : m_mesh(InvalidResource)
        , m_override_material(InvalidResource)
    { }

    void MeshRenderer::render_immediate(Renderer *renderer, ResourceCache *cache)
    {
        Mesh *mesh = cache->get<Mesh>(m_mesh);
        if (!mesh) return;

        renderer->set_primitive_type(PrimitiveType::Triangles);
        renderer->set_vao(mesh->get_vao());
        IndexBuffer *ib = mesh->get_index_buffer();
        for (size_t i = 0; i < mesh->get_submesh_count(); i++)
        {
            const SubMesh &submesh = mesh->get_submesh(i);

            renderer->set_material(submesh.m_material);
            renderer->set_indices(ib, submesh.m_start_index, submesh.m_index_count);
            renderer->render_command();
        }
    }

    void MeshRenderer::render(Renderer *renderer, ResourceCache *cache)
    {
        Mesh *mesh = cache->get<Mesh>(m_mesh);
        if (!mesh) return;

        renderer->set_primitive_type(PrimitiveType::Triangles);
        renderer->set_vao(mesh->get_vao());
        IndexBuffer *ib = mesh->get_index_buffer();

        const bool has_override_mat = (m_override_material != InvalidResource);
        for (size_t i = 0; i < mesh->get_submesh_count(); i++)
        {
            const SubMesh &submesh = mesh->get_submesh(i);
//            renderer->set_material(submesh.m_material);
            renderer->set_material(has_override_mat ? m_override_material : submesh.m_material);
            renderer->set_indices(ib, submesh.m_start_index, submesh.m_index_count);
            renderer->emit_command();
        }
    }

    void MeshRenderer::set_mesh(ResourceID mesh)
    { m_mesh = mesh; }

    ResourceID MeshRenderer::get_mesh() const
    { return m_mesh; }

    void MeshRenderer::set_material(ResourceID material)
    { m_override_material = material; }

    ResourceID MeshRenderer::get_material() const
    { return m_override_material; }

} // der
