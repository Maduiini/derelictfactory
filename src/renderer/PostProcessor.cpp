
#include "PostProcessor.h"

#include "PostProcessingEffect.h"
#include "ColorFrameBuffer.h"
#include "VertexBuffer.h"
#include "VertexArrayObject.h"
#include "Graphics.h"

#include "../math/Vector.h"

#include "../Debug.h"
#include "../Log.h"

namespace der
{

    VertexArrayObject *PostProcessor::m_vao = nullptr;
    VertexBuffer *PostProcessor::m_vbuffer = nullptr;

    PostProcessor::PostProcessor()
        : m_primary_buffer(nullptr)
        , m_secondary_buffer(nullptr)
        , m_effects()
        , m_enabled(false)
    {
        build_buffers();
    }

    PostProcessor::~PostProcessor()
    {
        for (PostProcessingEffect *effect : m_effects)
        {
            delete effect;
        }
    }

    void PostProcessor::resize(const int width, const int height)
    {
        if (m_primary_buffer)
            delete m_primary_buffer;

        if (m_secondary_buffer)
            delete m_secondary_buffer;

        m_primary_buffer = new ColorFrameBuffer(width, height);
        m_secondary_buffer = new ColorFrameBuffer(width, height);

        if (!m_primary_buffer->is_complete())
            log::error("Framebuffer is not complete.");
    }

    void PostProcessor::add_effect(PostProcessingEffect *effect)
    {
        m_effects.push_back(effect);
    }

    void PostProcessor::begin_scene()
    {
        if (!m_enabled)
            return;

        DER_ASSERT(m_primary_buffer);
        m_primary_buffer->bind();
//        m_primary_buffer->set_draw_buffers();
    }

    void PostProcessor::post_process(Graphics *graphics, ResourceCache &cache)
    {
        if (!m_enabled)
            return;

        m_vao->bind();
        for (size_t i=0; i<m_effects.size(); i++)
        {
            swap_buffers();

            if (i == m_effects.size()-1)
            {
                // This is the last effect, bind the default buffer
                FrameBuffer::bind_default_buffer();
            }
            else
            {
                m_primary_buffer->bind();
//                m_primary_buffer->set_draw_buffers();
            }

            PostProcessingEffect *effect = m_effects[i];

            effect->setup_rendering(graphics, cache, m_secondary_buffer);
            graphics->clear();
            graphics->draw_triangle_strip(0, 4);
        }
    }

    void PostProcessor::swap_buffers()
    {
        ColorFrameBuffer *temp = m_primary_buffer;
        m_primary_buffer = m_secondary_buffer;
        m_secondary_buffer = temp;
    }

    void PostProcessor::build_buffers()
    {
        if (m_vbuffer)
            return;

        struct Vertex
        {
            Vector2 position;
            Vector2 uv;
        } vertices[] = {
            {{ -1.0f, -1.0f }, { 0.0f, 0.0f }},
            {{  1.0f, -1.0f }, { 1.0f, 0.0f }},
            {{ -1.0f,  1.0f }, { 0.0f, 1.0f }},
            {{  1.0f,  1.0f }, { 1.0f, 1.0f }}
        };

        m_vbuffer = new VertexBuffer();
        m_vbuffer->bind();
        m_vbuffer->resize(sizeof(vertices), false);
        m_vbuffer->write(0, sizeof(vertices), &vertices);

        m_vao = new VertexArrayObject();
        m_vao->bind();
        m_vao->set_attribute(VertexAttrib::Position, 0, sizeof(Vertex), 2);
        m_vao->set_attribute(VertexAttrib::TexCoord, sizeof(Vector2), sizeof(Vertex), 2);
    }

} // der
