
#include "SceneRenderer.h"
#include "../scene/Scene.h"
#include "../scene/GameObject.h"
#include "../scene/Camera.h"
#include "../scene/QuadTree.h"

#include "Graphics.h"
#include "Renderer.h"
#include "UniformBuffer.h"
#include "MeshRenderer.h"
#include "TransformRenderer.h"
#include "QuadTreeRenderer.h"

namespace der
{

    SceneRenderer::SceneRenderer(Scene *scene, ResourceCache *cache)
        : m_scene(scene)
        , m_cache(cache)
        , m_time(0.0f)
        , m_nm_influence(1.0f)
        , m_frustum_culling(true)
        , m_debug_draw(false)
        , m_visible_object_count(0)
    {
        m_qt_renderer = new QuadTreeRenderer();
    }

    SceneRenderer::~SceneRenderer()
    {
        delete m_qt_renderer;
    }

    void SceneRenderer::render_immediate(Renderer *renderer)
    {
        m_visible_object_count = 0;

        GameObject *camera_obj = m_scene->get_camera_object();
        if (!camera_obj) return;

        Camera *camera = camera_obj->get_camera();
        if (camera)
        {
            const Matrix4 proj_mat = camera->get_projection();
            const Matrix4 view_mat = camera_obj->get_inv_world_matrix();
            const Vector3 camera_pos = camera_obj->get_position();
            renderer->set_projection_matrix(proj_mat);
            renderer->set_view_matrix(view_mat);
            renderer->set_camera_pos(camera_pos);
            renderer->set_time(m_time);
            renderer->set_normalmap_influence(m_nm_influence);
            renderer->bind_global_uniforms();

            QuadTree *quad_tree = m_scene->get_quad_tree();

            std::vector<GameObject*> objects;
            if (is_frustum_culling_enabled())
            {
                Frustum frustum = camera->extract_frustum(proj_mat * view_mat);
                quad_tree->get_objects_by_frustum(frustum, objects);
            }
            else
            {
                m_scene->get_all_objects(objects);
            }

            m_visible_object_count = objects.size();

            for (GameObject *object : objects)
            {
                MeshRenderer *obj_renderer = object->get_renderer();
                if (!obj_renderer) continue;

                const Matrix4 model_mat = object->get_world_matrix();
                renderer->set_model_matrix(model_mat);

                set_lights(renderer, object->get_position());

                obj_renderer->render_immediate(renderer, m_cache);
            }

            if (m_debug_draw)
            {
                for (GameObject *object : objects)
                {
                    TransformRenderer *tr_renderer = object->get_tr_renderer();
                    if (!tr_renderer) continue;

                    const Matrix4 model_mat = object->get_world_matrix();
                    renderer->set_model_matrix(model_mat);

                    tr_renderer->render_immediate(renderer);
                }
                m_qt_renderer->render_immediate(renderer, m_scene->get_quad_tree());
            }
        }
    }

    void SceneRenderer::render(Renderer *renderer)
    {
        m_visible_object_count = 0;

        GameObject *camera_obj = m_scene->get_camera_object();
        if (!camera_obj) return;

        Camera *camera = camera_obj->get_camera();
        if (camera)
        {
            const Matrix4 proj_mat = camera->get_projection();
            const Matrix4 view_mat = camera_obj->get_inv_world_matrix();
            const Vector3 camera_pos = camera_obj->get_position();
            renderer->set_projection_matrix(proj_mat);
            renderer->set_view_matrix(view_mat);
            renderer->set_camera_pos(camera_pos);
            renderer->set_time(m_time);
            renderer->set_normalmap_influence(m_nm_influence);

            QuadTree *quad_tree = m_scene->get_quad_tree();

            std::vector<GameObject*> objects;
            if (is_frustum_culling_enabled())
            {
                Frustum frustum = camera->extract_frustum(proj_mat * view_mat);
                quad_tree->get_objects_by_frustum(frustum, objects);
            }
            else
            {
                m_scene->get_all_objects(objects);
            }

            m_visible_object_count = objects.size();

            for (GameObject *object : objects)
            {
                MeshRenderer *obj_renderer = object->get_renderer();
                if (!obj_renderer) continue;

                const Matrix4 model_mat = object->get_world_matrix();
                renderer->set_model_matrix(model_mat);

                set_lights(renderer, object->get_position());

                obj_renderer->render(renderer, m_cache);
            }

            if (m_debug_draw)
            {
                for (GameObject *object : objects)
                {
                    TransformRenderer *tr_renderer = object->get_tr_renderer();
                    if (!tr_renderer) continue;

                    const Matrix4 model_mat = object->get_world_matrix();
                    renderer->set_model_matrix(model_mat);

                    tr_renderer->render(renderer);
                }
                m_qt_renderer->render(renderer, m_scene->get_quad_tree());
            }
        }
        renderer->render();
    }

    void SceneRenderer::set_time(float time)
    { m_time = time; }

    void SceneRenderer::set_normalmap_influence(float value)
    { m_nm_influence = value; }

    void SceneRenderer::set_frustum_culling_enabled(bool enabled)
    { m_frustum_culling = enabled; }

    void SceneRenderer::toggle_frustum_culling_enabled()
    { m_frustum_culling = !m_frustum_culling; }

    bool SceneRenderer::is_frustum_culling_enabled() const
    { return m_frustum_culling; }

    void SceneRenderer::set_debug_draw_enabled(bool enabled)
    { m_debug_draw = enabled; }

    void SceneRenderer::toggle_debug_draw_enabled()
    { m_debug_draw = !m_debug_draw; }

    bool SceneRenderer::is_debug_draw_enabled() const
    { return m_debug_draw; }

    size_t SceneRenderer::get_visible_object_count() const
    { return m_visible_object_count; }

    void SceneRenderer::set_lights(Renderer *renderer, const Vector3 &position)
    {
        std::vector<GameObject*> objects;
        m_scene->get_light_objects(position, objects);

        size_t light_count = (objects.size() < LightUniformBlock::MAX_LIGHTS) ?
            objects.size() : LightUniformBlock::MAX_LIGHTS;

        renderer->set_light_count(light_count);
        for (size_t i = 0; i < light_count; i++)
        {
            GameObject *object = objects[i];
            Light *light = object->get_light();
            renderer->set_light_position(i, object->get_position(), light->get_type());
            renderer->set_light_color(i, light->get_color(), light->get_energy());
            renderer->set_light_radius(i, light->get_radius());
        }
    }

} // der
