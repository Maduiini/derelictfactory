
#include "SSAOEffect.h"

#include "Graphics.h"
#include "ColorFrameBuffer.h"

#include "../resource/ResourceCache.h"

namespace der
{

    SSAOEffect::SSAOEffect()
        : PostProcessingEffect(make_resource("ssao.frag"))
        , m_strength(1.0f)
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
            graphics->set_texture(2, secondary_buffer->get_extra_texture());
            graphics->update_state();

            const int tex_color = program->get_uniform_location("tex_color");
            const int tex_normal = program->get_uniform_location("tex_normal");
            const int tex_extra = program->get_uniform_location("tex_extra");

            const int str_parameter = program->get_uniform_location("strength_parameter");

            program->uniform_sampler2D(tex_color, 0);
            program->uniform_sampler2D(tex_normal, 1);
            program->uniform_sampler2D(tex_extra, 2);

            program->uniform(str_parameter, m_strength);
        }
    }

    void SSAOEffect::set_strength(float str)
    {
        m_strength = str;
    }


    SSAOBlurX::SSAOBlurX()
        : PostProcessingEffect(make_resource("ssao_blur_x.frag"))
    { }


    SSAOBlurY::SSAOBlurY()
        : PostProcessingEffect(make_resource("ssao_blur_y.frag"))
    { }


} // der
