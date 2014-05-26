
#ifndef H_DER_FPS_CONTROLLER_H
#define H_DER_FPS_CONTROLLER_H

#include "InputController.h"

namespace der
{

    class FPSController : public InputController
    {
    public:
        FPSController();

        virtual void update(float delta_time) override;

        void set_rotation_x(float x)
        { m_rot_x = x; }

        float get_rotation_x() const
        { return m_rot_x; }

        void set_rotation_y(float y)
        { m_rot_y = y; }

        float get_rotation_y() const
        { return m_rot_y; }

        void set_jump_y(float y)
        { m_jump_y = y; }

        void set_speed(Vector2 speed)
        { m_speed = speed; }

        void set_jump_velocity(float velocity)
        { m_jump_velocity = velocity; }

    public:
        constexpr static float eye_level = 2.5f;

    private:
        float m_rot_x;
        float m_rot_y;
        float m_jump_y;
        float m_jump_velocity;
        float m_crouch_y;
        float m_head_bobble_state;
        Vector2 m_speed;

    };


} // der

#endif // H_DER_FPS_CONTROLLER_H
