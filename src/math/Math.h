
#ifndef H_DER_MATH_H
#define H_DER_MATH_H

#include "Vector.h"
#include "Matrix3.h"
#include "Matrix4.h"

namespace der
{

    struct Math
    {
        static constexpr float PI = 3.14159265358979323846f;
        static constexpr float E = 2.7182818284590452354f;

        static constexpr float DEG_2_RAD = PI / 180.0f;
        static constexpr float RAD_2_DEG = 180.0f / PI;

        static constexpr float EPSILON = 0.0001f;
    };

    bool equals(float a, float b);
    bool equals(float a, float b, float epsilon);

    // Vector3 rotation and transformation by matrices.

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

    inline Vector3 operator * (const Matrix3 &m, const Vector3 &v)
    { return rotate_vec(v, m); }

    inline Vector3 operator * (const Matrix4 &m, const Vector3 &v)
    { return transform_vec(v, m); }

    inline Vector3 operator * (const Matrix3x4 &m, const Vector3 &v)
    { return transform_vec(v, m); }


    // Vector4 rotation and transformation by matrices.

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

    inline Vector4 operator * (const Matrix3 &m, const Vector4 &v)
    { return rotate_vec(v, m); }

    inline Vector4 operator * (const Matrix4 &m, const Vector4 &v)
    { return transform_vec(v, m); }

    inline Vector4 operator * (const Matrix3x4 &m, const Vector4 &v)
    { return transform_vec(v, m); }


    namespace log
    {
        /// Writing vectors to log
        void write(const Vector3 &v);
        void write(const Vector4 &v);

        /// Writing matrices to log
        void write(const Matrix3 &m);
        void write(const Matrix4 &m);
        void write(const Matrix3x4 &m);
    } // log

} // der

#endif // H_DER_MATH_H

