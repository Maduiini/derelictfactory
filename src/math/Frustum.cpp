
#include "Frustum.h"
#include "Aabb.h"
#include "Math.h"

namespace der
{

    // static
    Vector4 Frustum::make_plane(const Vector3 &center, const Vector3 &pos, const Vector3 &delta, const Vector3 &t)
    {
        const Vector3 x = center + delta - pos;
        Vector3 n = x.normalized().cross(t);
        n.normalize();
        return Vector4(n, -n.dot(pos));
    }

    void Frustum::construct(const Matrix4 &world_mat, float fov, float aspect, float near, float far)
    {
        Vector3 right, up, forward;
        world_mat.get_basis(right, up, forward);
        forward = -forward;

        Vector3 pos, scale;
        Matrix3 rot;
        world_mat.decompose(pos, rot, scale);

        const float tan_half_fov_y = std::tan(0.5f * fov);
        const float near_h = tan_half_fov_y * near;

        const Vector3 near_center = pos + forward * near;
        const Vector3 far_center  = pos + forward * far;
        const Vector3 right_half  = right * near_h * aspect;
        const Vector3 up_half     = up * near_h;


        m_planes[Near] = Vector4(forward, forward.dot(far_center));
        m_planes[Far] = Vector4(-forward, forward.dot(near_center));

        m_planes[Right]  = make_plane(near_center, pos, right_half, -up);
        m_planes[Left]   = make_plane(near_center, pos, -right_half, up);
        m_planes[Top]    = make_plane(near_center, pos, up_half, right);
        m_planes[Bottom] = make_plane(near_center, pos, -up_half, -right);
    }

    bool Frustum::intersects_sphere(const Vector3 &position, const float radius) const
    {
        const float d0 = m_planes[0].xyz().dot(position) + m_planes[0].w;
        const float d1 = m_planes[1].xyz().dot(position) + m_planes[1].w;
        const float d2 = m_planes[2].xyz().dot(position) + m_planes[2].w;
        const float d3 = m_planes[3].xyz().dot(position) + m_planes[3].w;
        const float d4 = m_planes[4].xyz().dot(position) + m_planes[4].w;
        const float d5 = m_planes[5].xyz().dot(position) + m_planes[5].w;

        const bool out = (d0 < -radius) || (d1 < -radius) ||
                        (d2 < -radius) || (d3 < -radius) ||
                        (d4 < -radius) || (d5 < -radius);

        return !out;
    }

    bool Frustum::intersects_aabb(const Aabb &aabb) const
    {
        const Vector3 &m = aabb.m_min;
        const Vector3 &M = aabb.m_max;

//         Most likely testing the near plane would not be needed.
        const float d0 = max(m.x * m_planes[0].x, M.x * m_planes[0].x)
            + max(m.y * m_planes[0].y, M.y * m_planes[0].y)
            + max(m.z * m_planes[0].z, M.z * m_planes[0].z)
            + m_planes[0].w;
        const float d1 = max(m.x * m_planes[1].x, M.x * m_planes[1].x)
            + max(m.y * m_planes[1].y, M.y * m_planes[1].y)
            + max(m.z * m_planes[1].z, M.z * m_planes[1].z)
            + m_planes[1].w;
        const float d2 = max(m.x * m_planes[2].x, M.x * m_planes[2].x)
            + max(m.y * m_planes[2].y, M.y * m_planes[2].y)
            + max(m.z * m_planes[2].z, M.z * m_planes[2].z)
            + m_planes[2].w;
        const float d3 = max(m.x * m_planes[3].x, M.x * m_planes[3].x)
            + max(m.y * m_planes[3].y, M.y * m_planes[3].y)
            + max(m.z * m_planes[3].z, M.z * m_planes[3].z)
            + m_planes[3].w;
        const float d4 = max(m.x * m_planes[4].x, M.x * m_planes[4].x)
            + max(m.y * m_planes[4].y, M.y * m_planes[4].y)
            + max(m.z * m_planes[4].z, M.z * m_planes[4].z)
            + m_planes[4].w;
        const float d5 = max(m.x * m_planes[5].x, M.x * m_planes[5].x)
            + max(m.y * m_planes[5].y, M.y * m_planes[5].y)
            + max(m.z * m_planes[5].z, M.z * m_planes[5].z)
            + m_planes[5].w;

        const bool inside = (d0 > 0.0f) && (d1 > 0.0f) && (d2 > 0.0f) && (d3 > 0.0f) && (d4 > 0.0f) && (d5 > 0.0f);
        return inside;
    }

} // der
