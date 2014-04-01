
#include "Matrix3.h"
#include "Vector.h"
#include "Math.h"

namespace der
{

    const Matrix3 Matrix3::zero(0.0f);
    const Matrix3 Matrix3::identity(1.0f, 0.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f);


    bool Matrix3::equals(const Matrix3 &a) const
    { return equals(a, Math::EPSILON); }

    void Matrix3::rotation_from_axis_angle(const Vector3 &axis, float theta)
    { rotation_from_axis_angle(axis.x, axis.y, axis.z, theta); }

    void Matrix3::scale(const Vector3 &v)
    { scale(v.x, v.y, v.z); }

} // der
