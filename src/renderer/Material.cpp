
#include "Material.h"
#include "Graphics.h"

#include "../resource/ResourceCache.h"

namespace der
{

    Material::Material()
        : m_vert_shader(InvalidResource)
        , m_frag_shader(InvalidResource)
        , m_program(nullptr)
        , m_tex_albedo(InvalidResource)
        , m_tex_normal(InvalidResource)
        , m_cull_mode(CullMode::BackFace)
    {
        m_vert_shader = make_resource("pbs.vert");
        m_frag_shader = make_resource("pbs.frag");
    }

    Material::~Material()
    { }

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
        Texture *normal = cache.get<Texture2D>(get_normal_texture());

        graphics->set_texture(0, albedo);
        graphics->set_texture(1, normal);

        Program *program = cache.get_program(m_vert_shader, m_frag_shader);
        update_program(program);

        graphics->set_cull_mode(get_cull_mode());
        graphics->update_state();
    }

    void Material::update_program(Program *program)
    {
        if (program != m_program)
        {
            m_program = program;
            if (m_program)
            {
                m_program->use();
                const int tex_albedo = m_program->get_uniform_location("tex_albedo");
                const int tex_normal = m_program->get_uniform_location("tex_normal");
                const int tex_roughness = m_program->get_uniform_location("tex_roughness");
                const int tex_metallic = m_program->get_uniform_location("tex_metallic");
                const int tex_env = m_program->get_uniform_location("tex_env");

                m_program->uniform_sampler2D(tex_albedo, 0);
                m_program->uniform_sampler2D(tex_normal, 1);
                m_program->uniform_sampler2D(tex_roughness, 2);
                m_program->uniform_sampler2D(tex_metallic, 3);
                m_program->uniform_sampler2D(tex_env, 4);
            }
        }
    }

} // der
