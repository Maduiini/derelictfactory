
#include "PostProcessingEffect.h"

#include "Graphics.h"
#include "Shader.h"
#include "ColorFrameBuffer.h"

#include "../resource/ResourceCache.h"

namespace der
{

    PostProcessingEffect::PostProcessingEffect(ResourceID fragment_shader)
        : m_fragment_shader(fragment_shader)
        , m_enabled(true)
    { }

    PostProcessingEffect::~PostProcessingEffect()
    { }

    void PostProcessingEffect::setup_rendering(Graphics *graphics, ResourceCache &cache, ColorFrameBuffer *secondary_buffer)
    {
        if (!m_enabled)
            return;

        Program *program = cache.get_program(m_vertex_shader, m_fragment_shader);
        if (program)
        {
            program->use();
            graphics->set_texture(0, secondary_buffer->get_color_texture());
            graphics->set_texture(1, secondary_buffer->get_normal_texture());
            graphics->set_texture(2, secondary_buffer->get_extra_texture());
            graphics->update_state();

            const int tex_color = program->get_uniform_location("tex_color");
            const int tex_normal = program->get_uniform_location("tex_normal");
            const int tex_extra = program->get_uniform_location("tex_extra");

            program->uniform_sampler2D(tex_color, 0);
            program->uniform_sampler2D(tex_normal, 1);
            program->uniform_sampler2D(tex_extra, 2);
        }
    }

} // der
