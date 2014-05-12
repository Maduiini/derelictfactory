
#include "TransformRenderer.h"
#include "Graphics.h"
#include "Renderer.h"
#include "VertexArrayObject.h"
#include "VertexBuffer.h"

#include "../resource/ResourceCache.h"

#include "../math/Vector.h"

namespace der
{

    ResourceID TransformRenderer::m_vert_shader = make_resource("color.vert");
    ResourceID TransformRenderer::m_frag_shader = make_resource("color.frag");
    VertexArrayObject *TransformRenderer::m_vao = nullptr;
    VertexBuffer *TransformRenderer::m_vbuffer = nullptr;


    TransformRenderer::TransformRenderer()
    {
        build();
    }

    void TransformRenderer::render(Graphics *graphics, ResourceCache &cache)
    {
        m_vao->bind();
        Program *program = cache.get_program(m_vert_shader, m_frag_shader);
        if (program)
        {
            program->use();
            graphics->draw_lines(0, 6);
        }
    }

    void TransformRenderer::render(Renderer *renderer)
    {
        renderer->set_vao(m_vao);
        renderer->set_primitive_type(PrimitiveType::Lines);
        renderer->set_indices(0, 0, 6);
    }

    // static
    void TransformRenderer::build()
    {
        if (m_vbuffer) return;

        struct Vertex
        {
            Vector3 position;
            Vector3 color;
        } vertices[] = {
            {{ 0.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }}, // X-axis: red
            {{ 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }},
            {{ 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }}, // Y-axis: green
            {{ 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }},
            {{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }}, // Z-axis: blue
            {{ 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }}
        };

        m_vbuffer = new VertexBuffer();
        m_vbuffer->bind();
        m_vbuffer->resize(sizeof(vertices), false);

        m_vbuffer->write(0, sizeof(vertices), &vertices);

        m_vao = new VertexArrayObject();
        m_vao->bind();
        m_vao->set_attribute(VertexAttrib::Position, 0, sizeof(Vertex), 3);
        m_vao->set_attribute(VertexAttrib::Color, sizeof(Vector3), sizeof(Vertex), 3);
    }

} // der
