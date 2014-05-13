
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

        Vector3 pos, scale;
        Matrix3 rot;
        world_mat.decompose(pos, rot, scale);

        const float tan_half_fov_y = std::tan(0.5f * fov);
        const float near_h = tan_half_fov_y * near;

        const Vector3 near_center = pos + forward * near;
        const Vector3 far_center  = pos + forward * far;
        const Vector3 right_half  = right * near_h * aspect;
        const Vector3 up_half     = up * near_h;

        m_planes[Near] = Vector4(forward, -forward.dot(near_center));
        m_planes[Far] = Vector4(-forward, forward.dot(far_center));

        m_planes[Right]  = make_plane(near_center, pos, right_half, -up);
        m_planes[Left]   = make_plane(near_center, pos, -right_half, up);
        m_planes[Top]    = make_plane(near_center, pos, up_half, right);
        m_planes[Bottom] = make_plane(near_center, pos, -up_half, -right);
    }

    static void normalize_plane(Vector4 &plane)
    {
        const float len = plane.xyz().length();
        plane *= 1.0f / len;
    }

    void Frustum::extract(const Matrix4 &view_proj)
    {
        m_planes[Near].x = view_proj.m41 + view_proj.m31;
        m_planes[Near].y = view_proj.m42 + view_proj.m32;
        m_planes[Near].z = view_proj.m43 + view_proj.m33;
        m_planes[Near].w = view_proj.m44 + view_proj.m34;
        normalize_plane(m_planes[Near]);

        m_planes[Far].x = view_proj.m41 + view_proj.m31;
        m_planes[Far].y = view_proj.m42 + view_proj.m32;
        m_planes[Far].z = view_proj.m43 + view_proj.m33;
        m_planes[Far].w = view_proj.m44 + view_proj.m34;
        normalize_plane(m_planes[Far]);

        m_planes[Left].x = view_proj.m41 - view_proj.m11;
        m_planes[Left].y = view_proj.m42 - view_proj.m12;
        m_planes[Left].z = view_proj.m43 - view_proj.m13;
        m_planes[Left].w = view_proj.m44 - view_proj.m14;
        normalize_plane(m_planes[Left]);

        m_planes[Right].x = view_proj.m41 + view_proj.m11;
        m_planes[Right].y = view_proj.m42 + view_proj.m12;
        m_planes[Right].z = view_proj.m43 + view_proj.m13;
        m_planes[Right].w = view_proj.m44 + view_proj.m14;
        normalize_plane(m_planes[Right]);

        m_planes[Bottom].x = view_proj.m41 - view_proj.m21;
        m_planes[Bottom].y = view_proj.m42 - view_proj.m22;
        m_planes[Bottom].z = view_proj.m43 - view_proj.m23;
        m_planes[Bottom].w = view_proj.m44 - view_proj.m24;
        normalize_plane(m_planes[Bottom]);

        m_planes[Top].x = view_proj.m41 + view_proj.m21;
        m_planes[Top].y = view_proj.m42 + view_proj.m22;
        m_planes[Top].z = view_proj.m43 + view_proj.m23;
        m_planes[Top].w = view_proj.m44 + view_proj.m24;
        normalize_plane(m_planes[Top]);




//        Vector3 near(view_proj.m41 + view_proj.m31,
//                     view_proj.m42 + view_proj.m32,
//                     view_proj.m43 + view_proj.m33);
//        const float l = near.length();
//        near.normalize();
//        m_planes[Near].x = near.x;
//        m_planes[Near].y = near.y;
//        m_planes[Near].z = near.z;
//        m_planes[Near].w = (view_proj.m44 + view_proj.m34) / l;
//
//        Vector3 far(view_proj.m41 - view_proj.m31,
//                     view_proj.m42 - view_proj.m32,
//                     view_proj.m43 - view_proj.m33);
//        far.normalize();
//        m_planes[Far].x = far.x;
//        m_planes[Far].y = far.y;
//        m_planes[Far].z = far.z;
//        m_planes[Far].w = view_proj.m44 - view_proj.m34;
//
//        Vector3 left(view_proj.m41 + view_proj.m11,
//                     view_proj.m42 + view_proj.m12,
//                     view_proj.m43 + view_proj.m13);
//        left.normalize();
//        m_planes[Left].x = left.x;
//        m_planes[Left].y = left.y;
//        m_planes[Left].z = left.z;
//        m_planes[Left].w = view_proj.m44 + view_proj.m14;
//
//        Vector3 right(view_proj.m41 - view_proj.m11,
//                     view_proj.m42 - view_proj.m12,
//                     view_proj.m43 - view_proj.m13);
//        right.normalize();
//        m_planes[Right].x = right.x;
//        m_planes[Right].y = right.y;
//        m_planes[Right].z = right.z;
//        m_planes[Right].w = view_proj.m44 - view_proj.m14;
//
//        Vector3 bottom(view_proj.m41 + view_proj.m21,
//                     view_proj.m42 + view_proj.m22,
//                     view_proj.m43 + view_proj.m23);
//        bottom.normalize();
//        m_planes[Bottom].x = bottom.x;
//        m_planes[Bottom].y = bottom.y;
//        m_planes[Bottom].z = bottom.z;
//        m_planes[Bottom].w = view_proj.m44 + view_proj.m24;
//
//        Vector3 top(view_proj.m41 - view_proj.m21,
//                     view_proj.m42 - view_proj.m22,
//                     view_proj.m43 - view_proj.m23);
//        top.normalize();
//        m_planes[Top].x = top.x;
//        m_planes[Top].y = top.y;
//        m_planes[Top].z = top.z;
//        m_planes[Top].w = view_proj.m44 - view_proj.m24;
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
