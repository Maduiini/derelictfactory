
#ifndef H_DER_CAMERA_H
#define H_DER_CAMERA_H

#include "../math/Math.h"
#include "../math/Frustum.h"

namespace der
{

    class Camera
    {
    public:
        Camera();

        void set_params(float fov_degrees, float w, float h, float near, float far);
        void set_params(float fov_degrees, float aspect, float near, float far);

        void set_fov(float fov_degrees);
        float get_fov() const;

        void set_aspect_ratio(float aspect);
        float get_aspect_ratio() const;

        void set_depth_range(float near, float far);
        float get_near() const;
        float get_far() const;

        Matrix4 get_projection() const;

        Frustum construct_frustum(const Matrix4 &world_mat) const;
        Frustum extract_frustum(const Matrix4 &view_proj) const;

    private:
//        Matrix4 m_projection;
        float m_fov;
        float m_aspect;
        float m_near;
        float m_far;
    };

} // der

#endif // H_DER_CAMERA_H

