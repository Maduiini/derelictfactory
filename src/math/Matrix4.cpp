
#include "Matrix4.h"
#include "Vector.h"
#include "Math.h"

namespace der
{

    // Matrix4

    const Matrix4 Matrix4::zero(0.0f);
    const Matrix4 Matrix4::identity(1.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f);


    bool Matrix4::equals(const Matrix4 &a) const
    { return equals(a, Math::EPSILON); }

    void Matrix4::translation(const Vector3 &v)
    { translation(v.x, v.y, v.z); }

    void Matrix4::rotation_from_axis_angle(const Vector3 &axis, float theta)
    { rotation_from_axis_angle(axis.x, axis.y, axis.z, theta); }

    void Matrix4::scale(const Vector3 &v)
    { scale(v.x, v.y, v.z); }


    // Matrix3x4

    const Matrix3x4 Matrix3x4::zero(0.0f);
    const Matrix3x4 Matrix3x4::identity(1.0f, 0.0f, 0.0f, 0.0f,
                                        0.0f, 1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f, 0.0f);

    void Matrix3x4::translation(const Vector3 &v)
    { translation(v.x, v.y, v.z); }

    void Matrix3x4::rotation_from_axis_angle(const Vector3 &axis, float theta)
    { rotation_from_axis_angle(axis.x, axis.y, axis.z, theta); }

    void Matrix3x4::scale(const Vector3 &v)
    { scale(v.x, v.y, v.z); }

    bool Matrix3x4::equals(const Matrix3x4 &a) const
    { return equals(a, Math::EPSILON); }

} // der
