
#include "SceneRenderer.h"

namespace der
{

    SceneRenderer::SceneRenderer(Scene *scene)
        : m_scene(scene)
    { }

    void SceneRenderer::render()
    {
        // Some pseudo-code
//        GameObject *camera_obj = scene->get_camera();
//        if (!camera_obj) return;
//
//        Camera *camera = camera_obj->get_camera();
//        if (camera)
//        {
//            const Matrix4 proj_mat = camera()->get_projection();
//            const Matrix4 view_mat = camera_obj->get_inv_world_matrix();
//            // -- set matrices --
//
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
//        }
    }

} // der
