
#ifndef H_DER_MESH_RENDERER_H
#define H_DER_MESH_RENDERER_H

#include "../Types.h"

namespace der
{

    class Renderer;
    class Graphics;
    class ResourceCache;

    class Mesh;

    class MeshRenderer
    {
    public:
        MeshRenderer();

        void render_immediate(Renderer *rendere, ResourceCache *cache);
        void render(Renderer *renderer, ResourceCache *cache);

        void render_depth(Renderer *renderer, ResourceCache *cache);

        void set_mesh(ResourceID mesh);
        ResourceID get_mesh() const;

        void set_material(ResourceID material);
        ResourceID get_material() const;

        bool casts_shadows() const;

    private:
        ResourceID m_mesh;
        ResourceID m_override_material;

        bool m_casts_shadows;
    };

} // der

#endif // H_DER_MESH_RENDERER_H

