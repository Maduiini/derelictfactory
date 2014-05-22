
#include "SSAOEffect.h"

#include "Graphics.h"
#include "ColorFrameBuffer.h"

#include "../resource/ResourceCache.h"

namespace der
{

    SSAOEffect::SSAOEffect()
        : PostProcessingEffect(make_resource("ssao.frag"))
    { }

    void SSAOEffect::setup_rendering(Graphics *graphics, ResourceCache &cache, ColorFrameBuffer *secondary_buffer)
    {
        if (!m_enabled)
            return;

        Program *program = cache.get_program(m_vertex_shader, m_fragment_shader);
        if (program)
        {
            program->use();
            graphics->set_texture(0, secondary_buffer->get_color_texture());
            graphics->set_texture(1, secondary_buffer->get_normal_texture());
            graphics->update_state();

            const int tex_color = program->get_uniform_location("tex_color");
            const int tex_normal = program->get_uniform_location("tex_normal");

            program->uniform_sampler2D(tex_color, 0);
            program->uniform_sampler2D(tex_normal, 1);
        }
    }

} // der
