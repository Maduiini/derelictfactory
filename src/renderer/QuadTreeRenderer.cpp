
#include "QuadTreeRenderer.h"
#include "Graphics.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArrayObject.h"
#include "Shader.h"
#include "UniformBuffer.h"

#include "../scene/QuadTree.h"

#include "../resource/ResourceCache.h"

namespace der
{

    ResourceID QuadTreeRenderer::m_vert_shader = make_resource("color.vert");
    ResourceID QuadTreeRenderer::m_frag_shader = make_resource("color.frag");
    VertexArrayObject *QuadTreeRenderer::m_vao = nullptr;
    VertexBuffer *QuadTreeRenderer::m_vbuffer = nullptr;
    IndexBuffer *QuadTreeRenderer::m_ibuffer = nullptr;


    QuadTreeRenderer::QuadTreeRenderer()
    {
        build();
    }

    void QuadTreeRenderer::render(Graphics *graphics, ResourceCache &cache, InstanceUniformBlock *uniforms, const QuadTree *qt)
    {
        m_vao->bind();
        Program *program = cache.get_program(m_vert_shader, m_frag_shader);
        if (program)
        {
            program->use();
            graphics->set_blend_enabled(false);
            graphics->update_state();
            const QuadTreeNode *root = qt->get_root();
            render_node(graphics, cache, uniforms, root, 0);
        }
    }

    void QuadTreeRenderer::render(Renderer *renderer, QuadTree *qt)
    {

    }

    // static
    void QuadTreeRenderer::render_node(Graphics *graphics, ResourceCache &cache, InstanceUniformBlock *uniforms,
                                       const QuadTreeNode *node, size_t level)
    {
        if (node->has_objects()) // || node->is_leaf())
        {
            const float radius = node->get_radius();
            const Vector2 center = node->get_center();

            Matrix4 mat_t, mat_s;
            mat_t.translation(center.x, 0.0f, center.y);
            mat_s.scale(radius, 10.0f, radius);

            uniforms->set_model_mat(mat_t * mat_s);
            uniforms->bind_uniforms();
            graphics->draw_lines(m_ibuffer, 0, 12 * 2);
        }

//        if (level < 4 && !node->is_leaf())
        if (!node->is_leaf())
        {
            for (size_t i = 0; i < 4; i++)
            {
                if (node->get_child(i))
                    render_node(graphics, cache, uniforms, node->get_child(i), level + 1);
            }
        }
    }

    // static
    void QuadTreeRenderer::build()
    {
        if (m_vbuffer) return;

        struct Vertex
        {
            Vector3 position;
            Vector3 color;
        } vertices[] = {
            {{ -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 0.0f }},
            {{ 1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 0.0f }},
            {{ 1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, 0.0f }},
            {{ -1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, 0.0f }},
            {{ -1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f }},
            {{ 1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f }},
            {{ 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f }},
            {{ -1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 0.0f }}
        };

        m_vbuffer = new VertexBuffer();
        m_vbuffer->bind();
        m_vbuffer->resize(8 * sizeof(Vertex), false);

        m_vbuffer->write(0, 8 * sizeof(Vertex), &vertices);

        m_vao = new VertexArrayObject();
        m_vao->bind();
        m_vao->set_attribute(VertexAttrib::Position, 0, sizeof(Vertex), 3);
        m_vao->set_attribute(VertexAttrib::Color, sizeof(Vector3), sizeof(Vertex), 3);

        uint16_t indices[] = {
            0, 1,
            1, 2,
            2, 3,
            3, 0,

            4, 5,
            5, 6,
            6, 7,
            7, 4,

            0, 4,
            1, 5,
            2, 6,
            3, 7
        };

        m_ibuffer = new IndexBuffer();
        m_ibuffer->set_index_type(IndexBuffer::Index16);
        m_ibuffer->bind();
        m_ibuffer->resize(24 * sizeof(uint16_t), false);

        m_ibuffer->write(0, 24 * sizeof(uint16_t), &indices);
    }

} // der
