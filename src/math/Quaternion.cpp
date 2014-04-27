
#include "Quaternion.h"
#include "Math.h"

namespace der
{

    const Quaternion Quaternion::identity(0.0f, 0.0f, 0.0f, 1.0f);

    bool Quaternion::equals(const Quaternion &q) const
    { return equals(q, Math::EPSILON); }

    void Quaternion::from_axis_angle(const Vector3 &axis, float theta)
    { from_axis_angle(axis.x, axis.y, axis.z, theta); }

    void Quaternion::from_basis(const Vector3 &right, const Vector3 &up, const Vector3 &forward)
    {
        const float m00 = right.x;
        const float m10 = right.y;
        const float m20 = right.z;
        const float m01 = up.x;
        const float m11 = up.y;
        const float m21 = up.z;
        const float m02 = forward.x;
        const float m12 = forward.y;
        const float m22 = forward.z;

        const float trace = m00 + m11 + m22;
        if (trace > 0.0f)
        {
            const float s = std::sqrt(trace + 1.0f) * 2.0f;
            w = 0.25f * s;
            x = (m21 - m12) / s;
            y = (m02 - m20) / s;
            z = (m10 - m01) / s;
        }
        else if ((m00 > m11)&(m00 > m22))
        {
            const float s = std::sqrt(1.0f + m00 - m11 - m22) * 2.0f;
            w = (m21 - m12) / s;
            x = 0.25f * s;
            y = (m01 + m10) / s;
            z = (m02 + m20) / s;
        }
        else if (m11 > m22)
        {
            const float s = std::sqrt(1.0f + m11 - m00 - m22) * 2.0f;
            w = (m02 - m20) / s;
            x = (m01 + m10) / s;
            y = 0.25f * s;
            z = (m12 + m21) / s;
        }
        else
        {
            const float s = std::sqrt(1.0f + m22 - m00 - m11) * 2.0f;
            w = (m10 - m01) / s;
            x = (m02 + m20) / s;
            y = (m12 + m21) / s;
            z = 0.25f * s;
        }
    }


    Quaternion lerp(const Quaternion &q1, const Quaternion &q2, const float t)
    {
        const float s = 1.0f - t;
        return s * q1 + t * q2;
    }

    Quaternion slerp(const Quaternion &q1, const Quaternion &q2, const float t)
    {
        if (t <= 0.0f) return q1;
        if (t >= 1.0f) return q2;

        float dot = q1.dot(q2);

        const float DOT_EPSILON = 0.9999f;
        if (std::abs(dot) > DOT_EPSILON)
            return lerp(q1, q2, t);

        // Clamp dot between -1 and 1 for acos
        dot = clamp(dot, -1.0f, 1.0f);

        // To select the shortest path of rotation
        const float flip = (dot >= 0.0f) ? 1.0f : -1.0f;
        Quaternion qs = flip * q1;
        dot = flip * dot;

        const float cos_half_theta = dot;
        const float half_theta = std::acos(cos_half_theta);

        const float theta_t = half_theta * t;

        // Normalize to prevent drifting due to floating point imprecision.
        Quaternion q = (q2 - qs * dot).normalized();
        return qs * std::cos(theta_t) + q * std::sin(theta_t);
    }

} // der
