
#include "Matrix3.h"
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

    void Matrix3::from_quaternion(const Quaternion &quat)
    {
        const float xx = quat.x * quat.x;
        const float xy = quat.x * quat.y;
        const float xz = quat.x * quat.z;
        const float xw = quat.x * quat.w;

        const float yy = quat.y * quat.y;
        const float yz = quat.y * quat.z;
        const float yw = quat.y * quat.w;

        const float zz = quat.z * quat.z;
        const float zw = quat.z * quat.w;

        m11 = 1.0f - 2.0f * (yy + zz);  m12 = 2.0f * (xy - zw);         m13 = 2.0f * (xz + yw);
        m21 = 2.0f * (xy + zw);         m22 = 1.0f - 2.0f * (xx + zz);  m23 = 2.0f * (yz - xw);
        m31 = 2.0f * (xz - yw);         m32 = 2.0f * (yz + xw);         m33 = 1.0f - 2.0f * (xx + yy);
    }

    void Matrix3::scale(const Vector3 &v)
    { scale(v.x, v.y, v.z); }

} // der
