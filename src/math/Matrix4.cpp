
#include "Matrix4.h"
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

    void Matrix4::decompose(Vector3 &pos, Quaternion &rot, Vector3 &scale) const
    {
        Vector3 col1(m11, m21, m31);
        Vector3 col2(m12, m22, m32);
        Vector3 col3(m13, m23, m33);
        pos = Vector3(m14, m24, m34);
        scale = Vector3(col1.length(), col2.length(), col3.length());
        col1 /= scale.x;
        col2 /= scale.y;
        col3 /= scale.z;
        rot.from_basis(col1, col2, col3);
    }

    void Matrix4::translation(const Vector3 &v)
    { translation(v.x, v.y, v.z); }

    void Matrix4::from_axis_angle(const Vector3 &axis, float theta)
    { from_axis_angle(axis.x, axis.y, axis.z, theta); }

    void Matrix4::from_quaternion(const Quaternion &quat)
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

        m14 = m24 = m34 = 0.0f;
        m41 = m42 = m43 = 0.0f; m44 = 1.0f;
    }

    void Matrix4::scale(const Vector3 &v)
    { scale(v.x, v.y, v.z); }


    // Matrix3x4

    const Matrix3x4 Matrix3x4::zero(0.0f);
    const Matrix3x4 Matrix3x4::identity(1.0f, 0.0f, 0.0f, 0.0f,
                                        0.0f, 1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f, 0.0f);


    bool Matrix3x4::equals(const Matrix3x4 &a) const
    { return equals(a, Math::EPSILON); }

    void Matrix3x4::decompose(Vector3 &pos, Quaternion &rot, Vector3 &scale) const
    {
        Vector3 col1(m11, m21, m31);
        Vector3 col2(m12, m22, m32);
        Vector3 col3(m13, m23, m33);
        pos = Vector3(m14, m24, m34);
        scale = Vector3(col1.length(), col2.length(), col3.length());
        col1 /= scale.x;
        col2 /= scale.y;
        col3 /= scale.z;
        rot.from_basis(col1, col2, col3);
    }

    void Matrix3x4::translation(const Vector3 &v)
    { translation(v.x, v.y, v.z); }

    void Matrix3x4::from_axis_angle(const Vector3 &axis, float theta)
    { from_axis_angle(axis.x, axis.y, axis.z, theta); }

    void Matrix3x4::from_quaternion(const Quaternion &quat)
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

        m14 = m24 = m34 = 0.0f;
    }

    void Matrix3x4::scale(const Vector3 &v)
    { scale(v.x, v.y, v.z); }

} // der
