
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
        , m_global_uniforms(nullptr)
        , m_instance_uniforms(nullptr)
        , m_light_uniforms(nullptr)
        , m_frustum_culling(true)
        , m_visible_object_count(0)
    {
        m_global_uniforms = new GlobalUniformBlock();
        m_instance_uniforms = new InstanceUniformBlock();
        m_light_uniforms = new LightUniformBlock();
        m_qt_renderer = new QuadTreeRenderer();
    }

    SceneRenderer::~SceneRenderer()
    {
        delete m_global_uniforms;
        delete m_instance_uniforms;
        delete m_light_uniforms;
        delete m_qt_renderer;
    }

    void SceneRenderer::render(Graphics *graphics, ResourceCache &cache)
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
            m_global_uniforms->set_projection_mat(proj_mat);
            m_global_uniforms->set_view_mat(view_mat);
            m_global_uniforms->set_camera_pos(camera_pos);
            m_global_uniforms->bind_uniforms();

            QuadTree *quad_tree = m_scene->get_quad_tree();

            std::vector<GameObject*> objects;
            if (is_frustum_culling_enabled())
            {
                Frustum frustum = camera->construct_frustum(camera_obj->get_world_matrix());
                quad_tree->get_objects_by_frustum(frustum, objects);
            }
            else
            {
                m_scene->get_all_objects(objects);
            }

            m_visible_object_count = objects.size();

            for (GameObject *object : objects)
            {
                MeshRenderer *renderer = object->get_renderer();
                if (!renderer) continue;


                const Matrix4 model_mat = object->get_world_matrix();
                m_instance_uniforms->set_model_mat(model_mat);
                m_instance_uniforms->bind_uniforms();

                set_lights(object->get_position());

                renderer->render(graphics, &cache);

                TransformRenderer *tr_renderer = object->get_tr_renderer();
                if (tr_renderer)
                    tr_renderer->render(graphics, cache);
            }

            m_qt_renderer->render(graphics, cache, m_instance_uniforms, m_scene->get_quad_tree());

//            AccelerationStructure *acc_struct = m_scene->get_acceleration_structure();
//
//            const Frustum frustum = camera->construct_frustum(camera_obj->get_world_matrix());
//            FrustumQuery frustum_query(frustum);
//            acc_struct->query(frustum_query);
//
//            for (GameObject *object : frustum_query.get_objects())
//            {
//                MeshRenderer *renderer = object->get_renderer();
//                // -- set model matrix --
//                renderer->render();
//            }
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

            QuadTree *quad_tree = m_scene->get_quad_tree();

            std::vector<GameObject*> objects;
            if (is_frustum_culling_enabled())
            {
                Frustum frustum = camera->construct_frustum(camera_obj->get_world_matrix());
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

                TransformRenderer *tr_renderer = object->get_tr_renderer();
                if (tr_renderer)
                    tr_renderer->render(renderer);
            }
        }
        renderer->render();
    }

    void SceneRenderer::set_time(float time)
    { m_global_uniforms->set_time(time); }

    void SceneRenderer::set_frustum_culling_enabled(bool enabled)
    { m_frustum_culling = enabled; }

    void SceneRenderer::toggle_frustum_culling_enabled()
    { m_frustum_culling = !m_frustum_culling; }

    bool SceneRenderer::is_frustum_culling_enabled() const
    { return m_frustum_culling; }

    size_t SceneRenderer::get_visible_object_count() const
    { return m_visible_object_count; }

    void SceneRenderer::set_lights(const Vector3 &position)
    {
        std::vector<GameObject*> objects;
        m_scene->get_light_objects(position, objects);

        size_t light_count = (objects.size() < LightUniformBlock::MAX_LIGHTS) ?
            objects.size() : LightUniformBlock::MAX_LIGHTS;

        m_light_uniforms->set_light_count(light_count);
        for (size_t i = 0; i < light_count; i++)
        {
            GameObject *object = objects[i];
            Light *light = object->get_light();
            m_light_uniforms->set_position(i, object->get_position(), light->get_type());
            m_light_uniforms->set_color(i, light->get_color(), light->get_energy());
            m_light_uniforms->set_radius(i, light->get_radius());
        }
        m_light_uniforms->bind_uniforms();
    }

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
