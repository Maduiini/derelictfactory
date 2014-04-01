
#include "Quaternion.h"
#include "Vector.h"
#include "../Log.h"

namespace der
{

    const Quaternion Quaternion::identity(0.0f, 0.0f, 0.0f, 1.0f);

namespace log
{

    void write(const der::Quaternion &q)
    {
        write("Q(");
        write(q.x); write(",");
        write(q.y); write(",");
        write(q.z); write(",");
        write(q.w); write(")");
    }

} // log

    void Quaternion::rotation_from_axis_angle(const Vector3 &axis, float theta)
    { rotation_from_axis_angle(axis.x, axis.y, axis.z, theta); }


    Quaternion lerp(const Quaternion &q1, const Quaternion &q2, const float t)
    {
        const float s = 1.0f - t;
        return s * q1 + t * q2;
    }

    Quaternion slerp(const Quaternion &q1, const Quaternion &q2, const float t)
    {
        float dot = q1.dot(q2);

        const float DOT_EPSILON = 0.9995f;
        if (std::abs(dot) > DOT_EPSILON)
            return lerp(q1, q2, t);

        // clamp dot between -1 and 1
        dot = (dot < -1.0f) ? -1.0f : ((dot > 1.0f) ? 1.0f : dot);

        Quaternion qs = (dot >= 0.0f) ? q1 : -q1;

        const float cos_half_theta = dot;
        const float half_theta = std::acos(cos_half_theta);

        const float theta = half_theta * t;

        Quaternion q = (q2 - qs * dot).normalized();
        return qs * std::cos(theta) + q * std::sin(theta);
    }

} // der
