
#include "CameraController.h"
#include "../scene/GameObject.h"

namespace der
{

    CameraController::CameraController()
        : m_rot_x(0.0f)
        , m_rot_y(0.0f)
        , m_speed_up(1.0f)
    { }

    void CameraController::update(float delta_time)
    {
        const Matrix3 rot = get_object()->get_rotation_matrix();

        Vector3 right, up, forward;
        rot.get_basis(right, up, forward);

        const float move_speed = 2.5f * (key_down(Key::Left_Shift) ? 4.0f : 1.0f); // meters/second
        const float rotate_speed = 0.002f; // rad/pixel

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

        get_object()->move(delta * move_speed * m_speed_up * delta_time);

        if (key_pressed(Key::Left_Alt))
            toggle_mouse_captured();

        if (is_mouse_captured())
        {
            const Vector2 mouse_delta = get_mouse_delta() * rotate_speed;
            m_rot_x += mouse_delta.y;
            m_rot_y += mouse_delta.x;

            m_speed_up += get_delta_scroll() * 0.5f;
        }

        Quaternion rot_x, rot_y;
        rot_x.rotation_x(m_rot_x);
        rot_y.rotation_y(m_rot_y);
        get_object()->set_rotation(rot_y * rot_x);
    }

} // der
