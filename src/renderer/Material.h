
#ifndef H_DER_MATERIAL_H
#define H_DER_MATERIAL_H

#include "GraphicsTypes.h"
#include "../resource/Resource.h"

namespace der
{

    class Graphics;
    class ResourceCache;

    class Material
    {
    public:
        Material();

        void set_albedo_texture(ResourceID tex_id);
        ResourceID get_albedo_texture() const;

        void set_normal_texture(ResourceID tex_id);
        ResourceID get_normal_texture() const;

        void set_cull_mode(CullMode mode);
        CullMode get_cull_mode() const;

        void use(Graphics *graphics, ResourceCache &cache);

    private:
        ResourceID m_vert_shader;
        ResourceID m_frag_shader;

        ResourceID m_tex_albedo;
        ResourceID m_tex_normal;
        CullMode m_cull_mode;
    };

} // der

#endif // H_DER_MATERIAL_H

