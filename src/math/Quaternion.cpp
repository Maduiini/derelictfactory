
#include "Quaternion.h"
#include "Math.h"

namespace der
{

    const Quaternion Quaternion::identity(0.0f, 0.0f, 0.0f, 1.0f);

    bool Quaternion::equals(const Quaternion &q) const
    { return equals(q, Math::EPSILON); }

    void Quaternion::rotation_from_axis_angle(const Vector3 &axis, float theta)
    { rotation_from_axis_angle(axis.x, axis.y, axis.z, theta); }


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
        Quaternion qs = (dot >= 0.0f) ? q1 : -q1;

        const float cos_half_theta = dot;
        const float half_theta = std::acos(cos_half_theta);

        const float theta = half_theta * t;

        Quaternion q = (q2 - qs * dot).normalized();
        return qs * std::cos(theta) + q * std::sin(theta);
    }

} // der
