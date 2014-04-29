
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

    protected:
        float m_rot_x;
        float m_rot_y;
    };

} // der

#endif // H_DER_CAMERA_CONTROLLER_H

