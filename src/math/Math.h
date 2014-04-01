
#ifndef H_DER_MATH_H
#define H_DER_MATH_H

#include "Vector.h"
#include "Matrix3.h"
#include "Matrix4.h"
#include "Quaternion.h"

namespace der
{

    /// Math constants
    struct Math
    {
        static constexpr float PI = 3.14159265358979323846f;
        static constexpr float E = 2.7182818284590452354f;

        static constexpr float DEG_2_RAD = PI / 180.0f;
        static constexpr float RAD_2_DEG = 180.0f / PI;

        static constexpr float EPSILON = 0.0001f;
    };

    /// Tests equality of \c a and \c b within tolerance of \c Math::EPSILON.
    bool equals(float a, float b);
    /// Tests equality of \c a and \c b within tolerance of given \c epsilon.
    bool equals(float a, float b, float epsilon);

    template <class T>
    T clamp(T x, T a, T b)
    {
        return x < a ? a : (x > b ? b : x);
    }

    template <class T>
    T lerp(T a, T b, float t)
    {
        return a * (1.0f - t) + b * t;
    }

    // Vector3 rotation and transformation by matrices and quaternion.

    inline Vector3 rotate_vec(const Vector3 &v, const Matrix3 &m)
    {
        return Vector3(m.m11 * v.x + m.m12 * v.y + m.m13 * v.z,
                       m.m21 * v.x + m.m22 * v.y + m.m23 * v.z,
                       m.m31 * v.x + m.m32 * v.y + m.m33 * v.z);
    }

    inline Vector3 rotate_vec(const Vector3 &v, const Matrix4 &m)
    {
        return Vector3(m.m11 * v.x + m.m12 * v.y + m.m13 * v.z,
                       m.m21 * v.x + m.m22 * v.y + m.m23 * v.z,
                       m.m31 * v.x + m.m32 * v.y + m.m33 * v.z);
    }

    inline Vector3 rotate_vec(const Vector3 &v, const Matrix3x4 &m)
    {
        return Vector3(m.m11 * v.x + m.m12 * v.y + m.m13 * v.z,
                       m.m21 * v.x + m.m22 * v.y + m.m23 * v.z,
                       m.m31 * v.x + m.m32 * v.y + m.m33 * v.z);
    }

    inline Vector3 rotate_vec(const Vector3 &v, const Quaternion &q)
    {
        Vector3 n(q.x, q.y, q.z);
        return v + (2.0f * n).cross(n.cross(v) + q.w * v);
    }

    inline Vector3 transform_vec(const Vector3 &v, const Matrix4 &m)
    {
        return Vector3(m.m11 * v.x + m.m12 * v.y + m.m13 * v.z + m.m14 * 1.0f,
                       m.m21 * v.x + m.m22 * v.y + m.m23 * v.z + m.m24 * 1.0f,
                       m.m31 * v.x + m.m32 * v.y + m.m33 * v.z + m.m34 * 1.0f);
    }

    inline Vector3 transform_vec(const Vector3 &v, const Matrix3x4 &m)
    {
        return Vector3(m.m11 * v.x + m.m12 * v.y + m.m13 * v.z + m.m14 * 1.0f,
                       m.m21 * v.x + m.m22 * v.y + m.m23 * v.z + m.m24 * 1.0f,
                       m.m31 * v.x + m.m32 * v.y + m.m33 * v.z + m.m34 * 1.0f);
    }

    /// Multiplication of 3x3 matrix m and 3d-vector v. The vector is treated as a column vector.
    inline Vector3 operator * (const Matrix3 &m, const Vector3 &v)
    { return rotate_vec(v, m); }

    /// Multiplication of 4x4 matrix m and 3d-vector v. The vector is treated as a column vector
    /// with implicit fourth coordinate w=1.
    inline Vector3 operator * (const Matrix4 &m, const Vector3 &v)
    { return transform_vec(v, m); }

    /// Multiplication of 3x4 matrix m and 3d-vector v. The vector is treated as a column vector
    /// with implicit fourth coordinate w=1.
    inline Vector3 operator * (const Matrix3x4 &m, const Vector3 &v)
    { return transform_vec(v, m); }


    // Vector4 rotation and transformation by matrices and quaternion.

    inline Vector4 rotate_vec(const Vector4 &v, const Matrix3 &m)
    {
        return Vector4(m.m11 * v.x + m.m12 * v.y + m.m13 * v.z,
                       m.m21 * v.x + m.m22 * v.y + m.m23 * v.z,
                       m.m31 * v.x + m.m32 * v.y + m.m33 * v.z,
                       v.w);
    }

    inline Vector4 rotate_vec(const Vector4 &v, const Matrix4 &m)
    {
        return Vector4(m.m11 * v.x + m.m12 * v.y + m.m13 * v.z,
                       m.m21 * v.x + m.m22 * v.y + m.m23 * v.z,
                       m.m31 * v.x + m.m32 * v.y + m.m33 * v.z,
                       v.w);
    }

    inline Vector4 rotate_vec(const Vector4 &v, const Matrix3x4 &m)
    {
        return Vector4(m.m11 * v.x + m.m12 * v.y + m.m13 * v.z,
                       m.m21 * v.x + m.m22 * v.y + m.m23 * v.z,
                       m.m31 * v.x + m.m32 * v.y + m.m33 * v.z,
                       v.w);
    }

    inline Vector4 rotate_vec(const Vector4 &v, const Quaternion &q)
    {
        return Vector4( rotate_vec(v.xyz(), q), v.w );
    }

    inline Vector4 transform_vec(const Vector4 &v, const Matrix4 &m)
    {
        return Vector4(m.m11 * v.x + m.m12 * v.y + m.m13 * v.z + m.m14 * v.w,
                       m.m21 * v.x + m.m22 * v.y + m.m23 * v.z + m.m24 * v.w,
                       m.m31 * v.x + m.m32 * v.y + m.m33 * v.z + m.m34 * v.w,
                       m.m41 * v.x + m.m42 * v.y + m.m43 * v.z + m.m44 * v.w);
    }

    inline Vector4 transform_vec(const Vector4 &v, const Matrix3x4 &m)
    {
        return Vector4(m.m11 * v.x + m.m12 * v.y + m.m13 * v.z + m.m14 * v.w,
                       m.m21 * v.x + m.m22 * v.y + m.m23 * v.z + m.m24 * v.w,
                       m.m31 * v.x + m.m32 * v.y + m.m33 * v.z + m.m34 * v.w,
                       1.0f * v.w);
    }

    /// Multiplication of 3x3 matrix m and homogenous 3d-vector v. The vector is treated as a column vector.
    inline Vector4 operator * (const Matrix3 &m, const Vector4 &v)
    { return rotate_vec(v, m); }

    /// Multiplication of 4x4 matrix m and homogenous 3d-vector v. The vector is treated as a column vector.
    inline Vector4 operator * (const Matrix4 &m, const Vector4 &v)
    { return transform_vec(v, m); }

    /// Multiplication of 3x4 matrix m and homogenous 3d-vector v. The vector is treated as a column vector.
    inline Vector4 operator * (const Matrix3x4 &m, const Vector4 &v)
    { return transform_vec(v, m); }

} // der

#include <iosfwd>

namespace der
{

    std::ostream& operator << (std::ostream &out, const Vector3 &v);
    std::ostream& operator << (std::ostream &out, const Vector4 &v);
    std::ostream& operator << (std::ostream &out, const Quaternion &q);
    std::ostream& operator << (std::ostream &out, const Matrix3 &m);
    std::ostream& operator << (std::ostream &out, const Matrix4 &m);
    std::ostream& operator << (std::ostream &out, const Matrix3x4 &m);

} // der

#endif // H_DER_MATH_H

