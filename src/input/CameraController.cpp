
#include "CameraController.h"
#include "../scene/GameObject.h"

namespace der
{

    CameraController::CameraController()
        : m_object(nullptr)
        , m_rot_x(0.0f)
        , m_rot_y(0.0f)
    { }

    void CameraController::update(float delta_time)
    {
        const Matrix3 rot = m_object->get_rotation_matrix();

        Vector3 right, up, forward;
        rot.get_basis(right, up, forward);

        const float move_speed = 2.5f;
        const float rotate_speed = 1.0f;

        Vector3 delta = Vector3::zero;

        if (key_down(Key::W))
            delta += forward;
        if (key_down(Key::S))
            delta -= forward;
        if (key_down(Key::D))
            delta += right;
        if (key_down(Key::A))
            delta -= right;
        if (key_down(Key::Space))
            delta += up;
        if (key_down(Key::Left_Control))
            delta -= up;

        m_object->move(delta * move_speed * delta_time);

        if (key_pressed(Key::Left_Alt))
            toggle_mouse_captured();

        if (is_mouse_captured())
        {
            const Vector2 mouse_delta = get_mouse_delta() * rotate_speed * delta_time;
            m_rot_x += mouse_delta.y;
            m_rot_y += mouse_delta.x;
        }

        Quaternion rot_x, rot_y;
        rot_x.rotation_x(m_rot_x);
        rot_y.rotation_y(m_rot_y);
        m_object->set_rotation(rot_y * rot_x);
    }

    void CameraController::set_object(GameObject *object)
    { m_object = object; }

    GameObject* CameraController::get_object()
    { return m_object; }

} // der
