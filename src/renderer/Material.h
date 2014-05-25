
#ifndef H_DER_MATERIAL_H
#define H_DER_MATERIAL_H

#include "GraphicsTypes.h"
#include "../resource/Resource.h"

namespace der
{

    class Graphics;
    class ResourceCache;

    class Program;
    class DepthTexture;

    class Material
    {
    public:
        Material();
        ~Material();

        void set_shaders(ResourceID vert_id, ResourceID frag_id);

        void set_albedo_texture(ResourceID tex_id);
        ResourceID get_albedo_texture() const;

        void set_normal_texture(ResourceID tex_id);
        ResourceID get_normal_texture() const;

        void set_roughness_texture(ResourceID tex_id);
        ResourceID get_roughness_texture() const;

        void set_metallic_texture(ResourceID tex_id);
        ResourceID get_metallic_texture() const;

        void set_env_texture(ResourceID tex_id);
        ResourceID get_env_texture() const;

        void set_cull_mode(CullMode mode);
        CullMode get_cull_mode() const;

        void set_blending_enabled(bool enabled);
        bool is_blending_enabled() const;

        void set_casts_shadows(bool casts);
        bool casts_shadows() const;

        void use(Graphics *graphics, ResourceCache *cache, DepthTexture *shadowmap);
        void use_depth(Graphics *graphics, ResourceCache *cache);

    private:
        void update_program(Program *program);
        void update_program_depth(Program *program);

    private:
        ResourceID m_vert_shader;
        ResourceID m_frag_shader;
        ResourceID m_depth_vert_shader;
        ResourceID m_depth_frag_shader;
        Program *m_program;

        ResourceID m_tex_albedo;
        ResourceID m_tex_normal;
        ResourceID m_tex_roughness;
        ResourceID m_tex_metallic;
        ResourceID m_tex_env;

        CullMode m_cull_mode;
        bool m_blending;
        bool m_casts_shadows;
    };

} // der

#endif // H_DER_MATERIAL_H

