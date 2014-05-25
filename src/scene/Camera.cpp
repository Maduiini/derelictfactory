
#include "Camera.h"
#include "../math/Projection.h"

namespace der
{

    Camera::Camera()
        :
//            m_projection(Matrix4::identity)
          m_fov(60.0f)
        , m_aspect(1.33f)
        , m_near(0.01f)
        , m_far(1000.0f)
    { }

    void Camera::set_params(float fov_degrees, float w, float h, float near, float far)
    { set_params(fov_degrees, w / h, near, far); }

    void Camera::set_params(float fov_degrees, float aspect, float near, float far)
    {
        m_fov = fov_degrees;
        m_aspect = aspect;
        m_near = near;
        m_far = far;
    }

    void Camera::set_fov(float fov_degrees)
    { m_fov = fov_degrees; }

    float Camera::get_fov() const
    { return m_fov; }

    void Camera::set_aspect_ratio(float aspect)
    { m_aspect = aspect; }

    float Camera::get_aspect_ratio() const
    { return m_aspect; }

    void Camera::set_depth_range(float near, float far)
    {
        m_near = near;
        m_far = far;
    }

    float Camera::get_near() const
    { return m_near; }

    float Camera::get_far() const
    { return m_far; }

    Matrix4 Camera::get_projection() const
    {
        return projection_perspective_lh(m_fov, m_aspect, m_near, m_far);
    }

    Frustum Camera::construct_frustum(const Matrix4 &world_mat) const
    {
        Frustum frustum;
        frustum.construct(world_mat, m_fov, m_aspect, m_near, m_far);
        return frustum;
    }

    Frustum Camera::extract_frustum(const Matrix4 &view_proj) const
    {
        Frustum frustum;
        frustum.extract(view_proj);
        return frustum;
    }

    void Camera::get_frustum_points(Vector3 (&pts)[5], const Matrix4 &world_mat) const
    {
        Frustum::get_points(pts, world_mat, m_fov, m_aspect, m_near, m_far);
    }

} // der
