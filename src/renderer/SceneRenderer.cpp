
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
#include "DepthOnlyFrameBuffer.h"

#include "../math/Projection.h"

namespace der
{

    SceneRenderer::SceneRenderer(Scene *scene, ResourceCache *cache)
        : m_scene(scene)
        , m_cache(cache)
        , m_sm_buffer(nullptr)
        , m_time(0.0f)
        , m_nm_influence(1.0f)
        , m_frustum_culling(true)
        , m_debug_draw(false)
        , m_visible_object_count(0)
    {
//        m_sm_buffer = new DepthOnlyFrameBuffer(2048);
        m_sm_buffer = new DepthOnlyFrameBuffer(4096);
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
//            update_shadowmap(renderer);

            const Matrix4 proj_mat = camera->get_projection();
            const Matrix4 view_mat = camera_obj->get_inv_world_matrix();
            const Vector3 camera_pos = camera_obj->get_position();
            renderer->set_projection_matrix(proj_mat);
            renderer->set_view_matrix(view_mat);
            renderer->set_camera_pos(camera_pos);
            renderer->set_time(m_time);
            renderer->set_normalmap_influence(m_nm_influence);
            renderer->bind_global_uniforms();

            std::vector<GameObject*> objects;
            if (is_frustum_culling_enabled())
            {
                Frustum frustum = camera->extract_frustum(proj_mat * view_mat);
                QuadTree *quad_tree = m_scene->get_quad_tree();
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

                set_lights(renderer, object->get_bounding_box());

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
//            update_shadowmap(renderer);

            const Matrix4 proj_mat = camera->get_projection();
            const Matrix4 view_mat = camera_obj->get_inv_world_matrix();
            const Vector3 camera_pos = camera_obj->get_position();
            renderer->set_projection_matrix(proj_mat);
            renderer->set_view_matrix(view_mat);
            renderer->set_camera_pos(camera_pos);
            renderer->set_time(m_time);
            renderer->set_normalmap_influence(m_nm_influence);

            std::vector<GameObject*> objects;
            if (is_frustum_culling_enabled())
            {
                Frustum frustum = camera->extract_frustum(proj_mat * view_mat);
                QuadTree *quad_tree = m_scene->get_quad_tree();
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

                set_lights(renderer, object->get_bounding_box());

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

    void SceneRenderer::set_lights(Renderer *renderer, const Aabb &aabb)
    {
        const GameObject *sun = m_scene->get_sun();
        const Light *sun_light = sun->get_light();

        Vector3 r, direction, f;
        sun->get_world_matrix().get_basis(r, direction, f);

        renderer->set_light_position(0, sun->get_position(), sun_light->get_type());
        renderer->set_light_direction(0, direction);
        renderer->set_light_color(0, sun_light->get_color(), sun_light->get_energy());
        renderer->set_light_radius(0, sun_light->get_radius());
        renderer->set_light_spot_angle(0, sun_light->get_spot_angle());


        std::vector<GameObject*> objects;
        m_scene->get_light_objects(aabb, objects);

        const size_t light_count = min(objects.size(), LightUniformBlock::MAX_LIGHTS);
        size_t lt = 0;

        for (size_t i = 0; i < light_count; i++)
        {
            const GameObject *object = objects[i];
            if (object == sun)
            {
                lt = 1;
                continue;
            }

            const size_t ind = i + 1 - lt;

            Vector3 r, direction, f;
            object->get_world_matrix().get_basis(r, direction, f);

            const Light *light = object->get_light();
            renderer->set_light_position(ind, object->get_position(), light->get_type());
            renderer->set_light_direction(ind, direction);
            renderer->set_light_color(ind, light->get_color(), light->get_energy());
            renderer->set_light_radius(ind, light->get_radius());
            renderer->set_light_spot_angle(ind, light->get_spot_angle());
        }
        renderer->set_light_count(light_count + 1 - lt);
    }

    void SceneRenderer::get_objects_in_view(std::vector<GameObject*> &objects)
    {
        GameObject *camera_obj = m_scene->get_camera_object();
        if (!camera_obj) return;

        Camera *camera = camera_obj->get_camera();
        if (camera)
        {
            const Matrix4 proj_mat = camera->get_projection();
            const Matrix4 view_mat = camera_obj->get_inv_world_matrix();
            if (is_frustum_culling_enabled())
            {
                Frustum frustum = camera->extract_frustum(proj_mat * view_mat);
                QuadTree *quad_tree = m_scene->get_quad_tree();
                quad_tree->get_objects_by_frustum(frustum, objects);
            }
            else
            {
                m_scene->get_all_objects(objects);
            }
        }
    }

    void SceneRenderer::update_shadowmap(Renderer *renderer, Graphics *graphics)
    {
        GameObject *camera_obj = m_scene->get_camera_object();
        if (!camera_obj) return;

        Camera *camera = camera_obj->get_camera();
        if (!camera) return;

        Vector3 pts[5];
        camera->get_frustum_points(pts, camera_obj->get_world_matrix());
        Aabb light_aabb(pts[0], pts[0]);
        light_aabb.add_point(pts[1]);
        light_aabb.add_point(pts[2]);
        light_aabb.add_point(pts[3]);
        light_aabb.add_point(pts[4]);

        std::vector<GameObject*> view_objects;
        get_objects_in_view(view_objects);

//        Aabb light_aabb;
        Aabb view_aabb(Vector3(-10.0f, -10.0f, -100.0f), Vector3(10.0f, 10.0f, 100.0f));
        for (GameObject *object : view_objects)
        {
            MeshRenderer *renderer = object->get_renderer();
            if (renderer && renderer->casts_shadows())
            {
                Aabb obj_aabb = object->get_bounding_box();
                view_aabb.add_point(obj_aabb.m_min);
                view_aabb.add_point(obj_aabb.m_max);
            }
        }

//        if (view_aabb.is_degenerate())
//            view_aabb = Aabb(Vector3(-10.0f, -10.0f, -100.0f), Vector3(10.0f, 10.0f, 100.0f));
//        if (light_aabb.is_degenerate())
//            view_aabb = Aabb(Vector3(-10.0f, -10.0f, -100.0f), Vector3(10.0f, 10.0f, 100.0f));

//        light_aabb = view_aabb;

        light_aabb.m_min.x = max(light_aabb.m_min.x, view_aabb.m_min.x);
        light_aabb.m_min.y = max(light_aabb.m_min.y, view_aabb.m_min.y);
        light_aabb.m_min.z = max(light_aabb.m_min.z, view_aabb.m_min.z);

        light_aabb.m_max.x = min(light_aabb.m_max.x, view_aabb.m_max.x);
        light_aabb.m_max.y = min(light_aabb.m_max.y, view_aabb.m_max.y);
        light_aabb.m_max.z = min(light_aabb.m_max.z, view_aabb.m_max.z);

//        light_aabb.m_max.z = min(light_aabb.m_max.z, 100.0f);


        GameObject *sun = m_scene->get_sun();
//        Light *light = sun->get_light();

//        sun->set_position(Vector3::zero);
//        Matrix4 view = sun->get_inv_world_matrix();
//        // change of basis as the local up-axis is the light direction in blender
//        Vector3 right, up, forward;
//        view.get_basis(right, forward, up);
//        view.from_basis(right, -up, forward);

        Matrix4 sun_w = sun->get_world_matrix();
        // change of basis as the local up-axis is the light direction in blender
        Vector3 right, up, forward;
        sun_w.get_basis(right, up, forward);

        Matrix4 view;
        view.from_basis(right, forward, -up);
        view.transpose();

        light_aabb.transform(view);

        const Matrix4 proj = projection_orthogonal_lh(light_aabb.m_min.x, light_aabb.m_max.x,
                                                      light_aabb.m_min.y, light_aabb.m_max.y,
                                                      light_aabb.m_min.z, light_aabb.m_max.z);
//        const Matrix4 proj = projection_orthogonal_lh(light_aabb.m_min.x, light_aabb.m_max.x, light_aabb.m_min.y, light_aabb.m_max.y, -100, 100);
//        const Matrix4 proj = projection_orthogonal_lh(-100, 100, -100, 100, -100, 100);


        const Matrix4 view_proj = proj * view;

        const Matrix4 bias(0.5f, 0.0f, 0.0f, 0.5f,
                           0.0f, 0.5f, 0.0f, 0.5f,
                           0.0f, 0.0f, 0.5f, 0.5f,
                           0.0f, 0.0f, 0.0f, 1.0f);
//        , bias_scale;
//        bias_scale.scale(0.5f, 0.5f, 0.5f);
//        bias.translation(0.5f, 0.5f, 0.5f);
//        bias = bias * bias_scale;
        const Matrix4 light_mat = bias * view_proj;
        renderer->set_light_matrix(light_mat);

        renderer->set_projection_matrix(proj);
        renderer->set_view_matrix(view);

        std::vector<GameObject*> objects;
        if (is_frustum_culling_enabled())
        {
            Frustum frustum;
            frustum.extract(view_proj);
            QuadTree *quad_tree = m_scene->get_quad_tree();
            quad_tree->get_objects_by_frustum(frustum, objects);
        }
        else
        {
            m_scene->get_all_objects(objects);
        }

        for (GameObject *object : objects)
        {
            MeshRenderer *obj_renderer = object->get_renderer();
            if (!obj_renderer) continue;

            const Matrix4 model_mat = object->get_world_matrix();
            renderer->set_model_matrix(model_mat);

            obj_renderer->render(renderer, m_cache);
        }

        m_sm_buffer->bind();
        m_sm_buffer->set_viewport();
        graphics->clear_depth();
        renderer->render_depth();

        FrameBuffer::bind_default_buffer();

        renderer->set_shadowmap(m_sm_buffer->get_depth_texture());
    }

} // der
