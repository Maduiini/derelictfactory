
#include "SceneRenderer.h"
#include "../scene/Scene.h"
#include "../scene/GameObject.h"
#include "../scene/Camera.h"

#include "Graphics.h"
#include "UniformBuffer.h"
#include "MeshRenderer.h"

namespace der
{

    SceneRenderer::SceneRenderer(Scene *scene)
        : m_scene(scene)
        , m_global_uniforms(nullptr)
        , m_instance_uniforms(nullptr)
    {
        m_global_uniforms = new GlobalUniformBlock();
        m_instance_uniforms = new InstanceUniformBlock();
    }

    SceneRenderer::~SceneRenderer()
    {
        delete m_global_uniforms;
        delete m_instance_uniforms;
    }

    void SceneRenderer::render(Graphics *graphics, ResourceCache &cache)
    {
        GameObject *camera_obj = m_scene->get_camera_object();
        if (!camera_obj) return;

        Camera *camera = camera_obj->get_camera();
        if (camera)
        {
            const Matrix4 proj_mat = camera->get_projection();
            const Matrix4 view_mat = camera_obj->get_inv_world_matrix();
            m_global_uniforms->set_projection_mat(proj_mat);
            m_global_uniforms->set_view_mat(view_mat);
            m_global_uniforms->update();
            m_global_uniforms->bind_base(0);

            std::vector<GameObject*> objects;
            m_scene->get_visible_objects(objects);
            for (GameObject *object : objects)
            {
                MeshRenderer *renderer = object->get_renderer();
                if (!renderer) continue;

                const Matrix4 model_mat = object->get_world_matrix();
                m_instance_uniforms->set_model_mat(model_mat);
                m_instance_uniforms->update();
                m_instance_uniforms->bind_base(1);

                graphics->update_state();
                renderer->render(graphics, cache);
            }

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

} // der
