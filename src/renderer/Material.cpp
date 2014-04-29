
#include "Material.h"
#include "Graphics.h"

#include "../resource/ResourceCache.h"

namespace der
{

    Material::Material()
        : m_vert_shader(InvalidResource)
        , m_frag_shader(InvalidResource)
        , m_tex_albedo(InvalidResource)
        , m_tex_normal(InvalidResource)
        , m_cull_mode(CullMode::BackFace)
    {
        m_vert_shader = make_resource("pbs.vert");
        m_frag_shader = make_resource("pbs.frag");
    }

    void Material::set_albedo_texture(ResourceID tex_id)
    { m_tex_albedo = tex_id; }

    ResourceID Material::get_albedo_texture() const
    { return m_tex_albedo; }

    void Material::set_normal_texture(ResourceID tex_id)
    { m_tex_normal = tex_id; }

    ResourceID Material::get_normal_texture() const
    { return m_tex_normal; }

    void Material::set_cull_mode(CullMode mode)
    { m_cull_mode = mode; }

    CullMode Material::get_cull_mode() const
    { return m_cull_mode; }


    void Material::use(Graphics *graphics, ResourceCache &cache)
    {
        Texture *albedo = cache.get<Texture2D>(get_albedo_texture());
        graphics->set_texture(0, albedo);

        Texture *normal = cache.get<Texture2D>(get_normal_texture());
        graphics->set_texture(1, normal);

        Program *program = cache.get_program(m_vert_shader, m_frag_shader);
        if (program)
        {
            program->use();
            const int tex_albedo = program->get_uniform_location("tex_albedo");
            const int tex_normal = program->get_uniform_location("tex_normal");
            const int tex_roughness = program->get_uniform_location("tex_roughness");

            program->uniform_sampler2D(tex_albedo, 0);
            program->uniform_sampler2D(tex_normal, 1);
            program->uniform_sampler2D(tex_roughness, 2);
        }

        graphics->set_cull_mode(get_cull_mode());
        graphics->update_state();
    }

} // der
