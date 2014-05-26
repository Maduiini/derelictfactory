
#ifndef H_DER_CAMERA_CONTROLLER_H
#define H_DER_CAMERA_CONTROLLER_H

#include "InputController.h"

namespace der
{

    class CameraController : public InputController
    {
    public:
        CameraController();

        virtual void update(float delta_time) override;

        void set_rotation_x(float x)
        { m_rot_x = x; }

        float get_rotation_x() const
        { return m_rot_x; }

        void set_rotation_y(float y)
        { m_rot_y = y; }

        float get_rotation_y() const
        { return m_rot_y; }

    protected:
        float m_rot_x;
        float m_rot_y;
        float m_speed_up;
    };

} // der

#endif // H_DER_CAMERA_CONTROLLER_H

