
#include "Aabb.h"
#include "Math.h"

namespace der
{

    bool Aabb::intersects_sphere(const Vector3 &center, const float radius) const
    {
        float d = 0.0f;

        const Vector3 c_min = center - m_min;
        const Vector3 c_max = center - m_max;

//        if (c_min.x < 0.0f)
//            d += c_min.x * c_min.x;
//        else if (c_max.x > 0.0f)
//            d += c_max.x * c_max.x;
//
//        if (c_min.y < 0.0f)
//            d += c_min.y * c_min.y;
//        else if (c_max.y > 0.0f)
//            d += c_max.y * c_max.y;
//
//        if (c_min.z < 0.0f)
//            d += c_min.z * c_min.z;
//        else if (c_max.z > 0.0f)
//            d += c_max.z * c_max.z;

        d += (c_min.x < 0.0f) * c_min.x * c_min.x;
        d += (c_max.x > 0.0f) * c_max.x * c_max.x;
        d += (c_min.y < 0.0f) * c_min.y * c_min.y;
        d += (c_max.y > 0.0f) * c_max.y * c_max.y;
        d += (c_min.z < 0.0f) * c_min.z * c_min.z;
        d += (c_max.z > 0.0f) * c_max.z * c_max.z;

        return d <= radius * radius;
    }

    void Aabb::transform(const Matrix4 &mat)
    {
        const float m03 = mat.m[0][3];
        const float m13 = mat.m[1][3];
        const float m23 = mat.m[2][3];
        const Vector3 translation(m03, m13, m23);

        if (!is_degenerate())
        {
            const float min_x_00 = mat.m[0][0] * m_min.x;
            const float min_x_10 = mat.m[1][0] * m_min.x;
            const float min_x_20 = mat.m[2][0] * m_min.x;

            const float min_y_01 = mat.m[0][1] * m_min.y;
            const float min_y_11 = mat.m[1][1] * m_min.y;
            const float min_y_21 = mat.m[2][1] * m_min.y;

            const float min_z_02 = mat.m[0][2] * m_min.z;
            const float min_z_12 = mat.m[1][2] * m_min.z;
            const float min_z_22 = mat.m[2][2] * m_min.z;

            const float max_x_00 = mat.m[0][0] * m_max.x;
            const float max_x_10 = mat.m[1][0] * m_max.x;
            const float max_x_20 = mat.m[2][0] * m_max.x;

            const float max_y_01 = mat.m[0][1] * m_max.y;
            const float max_y_11 = mat.m[1][1] * m_max.y;
            const float max_y_21 = mat.m[2][1] * m_max.y;

            const float max_z_02 = mat.m[0][2] * m_max.z;
            const float max_z_12 = mat.m[1][2] * m_max.z;
            const float max_z_22 = mat.m[2][2] * m_max.z;


            const float v0X = min_x_00 + min_y_01 + min_z_02;
            const float v0Y = min_x_10 + min_y_11 + min_z_12;
            const float v0Z = min_x_20 + min_y_21 + min_z_22;

            const float v1X = max_x_00 + min_y_01 + min_z_02;
            const float v1Y = max_x_10 + min_y_11 + min_z_12;
            const float v1Z = max_x_20 + min_y_21 + min_z_22;

            const float v2X = max_x_00 + max_y_01 + min_z_02;
            const float v2Y = max_x_10 + max_y_11 + min_z_12;
            const float v2Z = max_x_20 + max_y_21 + min_z_22;

            const float v3X = min_x_00 + max_y_01 + min_z_02;
            const float v3Y = min_x_10 + max_y_11 + min_z_12;
            const float v3Z = min_x_20 + max_y_21 + min_z_22;

            const float v4X = min_x_00 + min_y_01 + max_z_02;
            const float v4Y = min_x_10 + min_y_11 + max_z_12;
            const float v4Z = min_x_20 + min_y_21 + max_z_22;

            const float v5X = max_x_00 + min_y_01 + max_z_02;
            const float v5Y = max_x_10 + min_y_11 + max_z_12;
            const float v5Z = max_x_20 + min_y_21 + max_z_22;

            const float v6X = max_x_00 + max_y_01 + max_z_02;
            const float v6Y = max_x_10 + max_y_11 + max_z_12;
            const float v6Z = max_x_20 + max_y_21 + max_z_22;

            const float v7X = min_x_00 + max_y_01 + max_z_02;
            const float v7Y = min_x_10 + max_y_11 + max_z_12;
            const float v7Z = min_x_20 + max_y_21 + max_z_22;

            m_min = m_max = Vector3(v0X, v0Y, v0Z);
            add_point(v1X, v1Y, v1Z);
            add_point(v2X, v2Y, v2Z);
            add_point(v3X, v3Y, v3Z);
            add_point(v4X, v4Y, v4Z);
            add_point(v5X, v5Y, v5Z);
            add_point(v6X, v6Y, v6Z);
            add_point(v7X, v7Y, v7Z);
            m_min += translation;
            m_max += translation;
        }
        else
        {
            m_min = translation;
            m_max = translation;
        }
    }

} // der
