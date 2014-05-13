
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

        void render_immediate(Renderer *renderer);
        void render(Renderer *renderer);

        void set_time(float time);
        void set_normalmap_influence(float value);

        void set_frustum_culling_enabled(bool enabled);
        void toggle_frustum_culling_enabled();
        bool is_frustum_culling_enabled() const;

        void set_debug_draw_enabled(bool enabled);
        void toggle_debug_draw_enabled();
        bool is_debug_draw_enabled() const;

        size_t get_visible_object_count() const;

    private:
        void set_lights(Renderer *renderer, const Vector3 &position);

    private:
        Scene *m_scene;
        ResourceCache *m_cache;

        float m_time;
        float m_nm_influence;

        bool m_frustum_culling;
        bool m_debug_draw;
        size_t m_visible_object_count;

        QuadTreeRenderer *m_qt_renderer;
    };

} // der

#endif // H_DER_SCENE_RENDERER_H

