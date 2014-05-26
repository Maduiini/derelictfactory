
#include "FPSController.h"
#include "../scene/GameObject.h"

#include <cmath>

namespace der
{

    FPSController::FPSController()
        : m_rot_x(0.0f)
        , m_rot_y(0.0f)
        , m_jump_y(0.0f)
        , m_jump_velocity(0.0f)
        , m_crouch_y(0.0f)
        , m_head_bobble_state(0.0f)
        , m_speed(0.0f)
    { }

    void FPSController::update(float delta_time)
    {
        const float rotate_speed = 0.002f; // rad/pixel

        const float gravity = -9.81f;
        const float jump_power = key_down(Key::Left_Shift) ? 6.0f : 3.0f;

        const float crouch_speed = 4.0f;
        const float crouch_depth = 0.8f;
        const float crouch_speed_modifier = 0.2f + 0.8f * (-crouch_depth - m_crouch_y) / -crouch_depth;

        const float acceleration = 0.3f;
        const float walk_speed = 3.0f;
        const float run_speed = 10.0f;
        const float friction = 0.95f;
        const float max_speed = crouch_speed_modifier * (key_down(Key::Left_Shift) ? run_speed : walk_speed);
        const Matrix3 rot = get_object()->get_rotation_matrix();

        Vector3 right3, up3, forward3;
        rot.get_basis(right3, up3, forward3);

        forward3.y = 0.0f;
        right3.y = 0.0f;
        forward3.normalize();
        right3.normalize();

        Vector2 forward = Vector2(forward3.x, forward3.z);
        Vector2 right = Vector2(right3.x, right3.z);

        Vector2 delta = Vector2::zero;

        if (key_down(Key::W))
            delta += forward;
        if (key_down(Key::S))
            delta -= forward;
        if (key_down(Key::D))
            delta += right;
        if (key_down(Key::A))
            delta -= right;

        bool overspeed = m_speed.length2() > max_speed * max_speed;

        if (!overspeed)
            m_speed += delta * acceleration;

        if (overspeed || delta == Vector2::zero)
            m_speed *= friction;

        m_head_bobble_state += m_speed.length() * delta_time;
        Vector3 speed3 = Vector3(m_speed.x, 0.0f, m_speed.y);
        get_object()->move(speed3 * delta_time);

        // Gravity and jumping
        if (m_jump_y > 0.0f)
        {
            m_jump_velocity += gravity * delta_time;
            m_jump_y += m_jump_velocity * delta_time;
        }
        else
        {
            m_jump_y = 0.0f; // Prevent falling too low

            if (key_down(Key::Space))
            {
                m_jump_velocity = jump_power;
                m_jump_y += m_jump_velocity * delta_time;
            }
        }

        // Crouching
        if (key_down(Key::Left_Control) && m_crouch_y > -crouch_depth)
            m_crouch_y += (-crouch_depth - m_crouch_y) * crouch_speed * delta_time;
        if (!key_down(Key::Left_Control) && m_crouch_y < 0.0f)
            m_crouch_y += -m_crouch_y * crouch_speed * delta_time;

        // Set y-position
        const float head_bobble = m_jump_y > 0.0f? 0.0f : sin(m_head_bobble_state * 1.5f) * 0.05f;
        Vector3 position = get_object()->get_position();
        position.y = eye_level + m_crouch_y + m_jump_y + head_bobble;
        get_object()->set_position(position);


        if (key_pressed(Key::Left_Alt))
            toggle_mouse_captured();

        if (is_mouse_captured())
        {
            const Vector2 mouse_delta = get_mouse_delta() * rotate_speed;
            m_rot_x += mouse_delta.y;
            m_rot_y += mouse_delta.x;
        }

        Quaternion rot_x, rot_y;
        rot_x.rotation_x(m_rot_x);
        rot_y.rotation_y(m_rot_y);
        get_object()->set_rotation(rot_y * rot_x);
    }

} // der
