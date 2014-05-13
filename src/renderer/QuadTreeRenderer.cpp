
#include "QuadTreeRenderer.h"
#include "Graphics.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArrayObject.h"
#include "Shader.h"
#include "UniformBuffer.h"

#include "../scene/QuadTree.h"

#include "../resource/ResourceCache.h"

namespace der
{

//    ResourceID QuadTreeRenderer::m_vert_shader = make_resource("color.vert");
//    ResourceID QuadTreeRenderer::m_frag_shader = make_resource("color.frag");
//    VertexArrayObject *QuadTreeRenderer::m_vao = nullptr;
//    VertexBuffer *QuadTreeRenderer::m_vbuffer = nullptr;
//    IndexBuffer *QuadTreeRenderer::m_ibuffer = nullptr;


    QuadTreeRenderer::QuadTreeRenderer()
        : m_vert_shader(make_resource("color.vert"))
        , m_frag_shader(make_resource("color.frag"))
        , m_material(make_resource("Color.material"))
        , m_vao(nullptr)
        , m_vbuffer(nullptr)
        , m_ibuffer(nullptr)
    {
        build();
    }

    void QuadTreeRenderer::render_immediate(Renderer *renderer, const QuadTree *qt)
    {
        renderer->set_material(m_material);
        renderer->set_vao(m_vao);
        renderer->set_primitive_type(PrimitiveType::Lines);

        const QuadTreeNode *root = qt->get_root();
        render_node_immediate(renderer, root, 0);
    }

    void QuadTreeRenderer::render(Renderer *renderer, const QuadTree *qt)
    {
        renderer->set_material(m_material);
        renderer->set_vao(m_vao);
        renderer->set_primitive_type(PrimitiveType::Lines);

        const QuadTreeNode *root = qt->get_root();
        render_node(renderer, root, 0);
    }

    void QuadTreeRenderer::render_node_immediate(Renderer *renderer, const QuadTreeNode *node, size_t level)
    {
        if (node->has_objects())
        {
            const float radius = node->get_radius();
            const Vector2 center = node->get_center();

            Matrix4 mat_t, mat_s;
            mat_t.translation(center.x, 0.0f, center.y);
            mat_s.scale(radius, 10.0f, radius);

            renderer->set_model_matrix(mat_t * mat_s);
            renderer->set_indices(m_ibuffer, 0, 12 * 2);
            renderer->render_command();
        }

        if (!node->is_leaf())
        {
            for (size_t i = 0; i < 4; i++)
            {
                render_node_immediate(renderer, node->get_child(i), level + 1);
            }
        }
    }

    void QuadTreeRenderer::render_node(Renderer *renderer, const QuadTreeNode *node, size_t level)
    {
        if (node->has_objects())
        {
            const float radius = node->get_radius();
            const Vector2 center = node->get_center();

            Matrix4 mat_t, mat_s;
            mat_t.translation(center.x, 0.0f, center.y);
            mat_s.scale(radius, 10.0f, radius);

            renderer->set_model_matrix(mat_t * mat_s);
            renderer->set_indices(m_ibuffer, 0, 12 * 2);
            renderer->emit_command();
        }

        if (!node->is_leaf())
        {
            for (size_t i = 0; i < 4; i++)
            {
                render_node(renderer, node->get_child(i), level + 1);
            }
        }
    }

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
