
#ifndef H_DER_SCENE_RENDERER_H
#define H_DER_SCENE_RENDERER_H

#include "../Types.h"

#include <vector>

namespace der
{

    struct Vector3;
    struct Aabb;

    class ResourceCache;

    class Scene;
    class GameObject;

    class Renderer;
    class Graphics;
    class GlobalUniformBlock;
    class InstanceUniformBlock;
    class LightUniformBlock;
    class ParamUniformBlock;
    class DepthOnlyFrameBuffer;

    class QuadTreeRenderer;

    class SceneRenderer
    {
    public:
        explicit SceneRenderer(Scene *scene, ResourceCache *cache);
        ~SceneRenderer();

        void render_immediate(Renderer *renderer);
        void render(Renderer *renderer);

        void update_shadowmap(Renderer *renderer, Graphics *graphics);

        void set_time(float time);
        void set_normalmap_influence(float value);

        void set_frustum_culling_enabled(bool enabled);
        void toggle_frustum_culling_enabled();
        bool is_frustum_culling_enabled() const;

        void set_debug_draw_enabled(bool enabled);
        void toggle_debug_draw_enabled();
        bool is_debug_draw_enabled() const;

        void set_shadow_map_enabled(bool enabled);
        void toggle_shadow_map_enabled();
        bool is_shadow_map_enabled() const;

        size_t get_visible_object_count() const;

    private:
        void set_lights(Renderer *renderer, const Aabb &aabb);

        void get_objects_in_view(std::vector<GameObject*> &objects);

    private:
        Scene *m_scene;
        ResourceCache *m_cache;

        DepthOnlyFrameBuffer *m_sm_buffer;

        float m_time;
        float m_nm_influence;

        bool m_frustum_culling;
        bool m_debug_draw;
        bool m_shadow_mapping;
        size_t m_visible_object_count;

        QuadTreeRenderer *m_qt_renderer;
    };

} // der

#endif // H_DER_SCENE_RENDERER_H

