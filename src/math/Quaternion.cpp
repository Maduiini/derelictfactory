
#include "Quaternion.h"
#include "Vector.h"

namespace der
{

    const Quaternion Quaternion::identity(0.0f, 0.0f, 0.0f, 1.0f);


    Quaternion::Quaternion(const Vector3 &axis, float theta)
        : Quaternion(axis.x, axis.y, axis.z, theta)
    { }

    Quaternion slerp(const Quaternion &q1, const Quaternion &q2, const float t)
    {
        // TODO: implement
        return Quaternion();
    }

} // der