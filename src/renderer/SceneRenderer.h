
#ifndef H_DER_SCENE_RENDERER_H
#define H_DER_SCENE_RENDERER_H

#include "../Types.h"

namespace der
{

    struct Vector3;

    class ResourceCache;

    class Scene;

    class Renderer;
    class Graphics;
    class GlobalUniformBlock;
    class InstanceUniformBlock;
    class LightUniformBlock;
    class ParamUniformBlock;

    class QuadTreeRenderer;

    class SceneRenderer
    {
    public:
        explicit SceneRenderer(Scene *scene, ResourceCache *cache);
        ~SceneRenderer();

        void render(Graphics *graphics, Renderer *renderer, ResourceCache &cache);
        void render(Renderer *renderer);

        void set_time(float time);
        void set_normalmap_influence(float value);

        void set_frustum_culling_enabled(bool enabled);
        void toggle_frustum_culling_enabled();
        bool is_frustum_culling_enabled() const;

        size_t get_visible_object_count() const;

    private:
        void set_lights(const Vector3 &position);
        void set_lights(Renderer *renderer, const Vector3 &position);

    private:
        Scene *m_scene;
        ResourceCache *m_cache;

        GlobalUniformBlock *    m_global_uniforms;
        InstanceUniformBlock *  m_instance_uniforms;
        LightUniformBlock *     m_light_uniforms;
        ParamUniformBlock *     m_param_uniforms;

        float m_nm_influence;

        bool m_frustum_culling;

        size_t m_visible_object_count;

        QuadTreeRenderer *m_qt_renderer;
    };

} // der

#endif // H_DER_SCENE_RENDERER_H

