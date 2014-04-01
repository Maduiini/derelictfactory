
#ifndef H_DER_MATRIX4_H
#define H_DER_MATRIX4_H

#include <cmath>

namespace der
{

    // Forward declarations

    bool equals(float, float, float);
    struct Vector3;


    /// A row-major 4x4 matrix, for transformation and projection matrices.
    struct Matrix4
    {
        static const Matrix4 zero;
        static const Matrix4 identity;

        union {
            struct {
                float m11, m12, m13, m14;
                float m21, m22, m23, m24;
                float m31, m32, m33, m34;
                float m41, m42, m43, m44;
            };
            float m[4][4];
        };

        // Constructors

        /// Default constructor leaves the matrix in unitialized state.
        Matrix4() { }

        Matrix4(const Matrix4 &a)
            : m11(a.m11), m12(a.m12), m13(a.m13), m14(a.m14)
            , m21(a.m21), m22(a.m22), m23(a.m23), m24(a.m24)
            , m31(a.m31), m32(a.m32), m33(a.m33), m34(a.m34)
            , m41(a.m41), m42(a.m42), m43(a.m43), m44(a.m44) { }

        Matrix4(float x)
            : m11(x), m12(x), m13(x), m14(x)
            , m21(x), m22(x), m23(x), m24(x)
            , m31(x), m32(x), m33(x), m34(x)
            , m41(x), m42(x), m43(x), m44(x) { }

        Matrix4(float a11, float a12, float a13, float a14,
                float a21, float a22, float a23, float a24,
                float a31, float a32, float a33, float a34,
                float a41, float a42, float a43, float a44)
            : m11(a11), m12(a12), m13(a13), m14(a14)
            , m21(a21), m22(a22), m23(a23), m24(a24)
            , m31(a31), m32(a32), m33(a33), m34(a34)
            , m41(a41), m42(a42), m43(a43), m44(a44) { }

        // Methods

        /// Returns true, if this matrix equals matrix \c a within tolerance of \c epsilon.
        bool equals(const Matrix4 &a, float epsilon) const
        {
            const float e = epsilon;
            return der::equals(m11, a.m11, e) && der::equals(m12, a.m12, e) && der::equals(m13, a.m13, e) && der::equals(m14, a.m14, e) &&
                der::equals(m21, a.m21, e) && der::equals(m22, a.m22, e) && der::equals(m23, a.m23, e) && der::equals(m24, a.m24, e) &&
                der::equals(m31, a.m31, e) && der::equals(m32, a.m32, e) && der::equals(m33, a.m33, e) && der::equals(m34, a.m34, e) &&
                der::equals(m41, a.m41, e) && der::equals(m42, a.m42, e) && der::equals(m43, a.m43, e) && der::equals(m44, a.m44, e);
        }

        bool equals(const Matrix4 &a) const;

        void transpose()
        { *this = transposed(); }

        Matrix4 transposed() const
        {
            return Matrix4(m11, m21, m31, m41,
                           m12, m22, m32, m42,
                           m13, m23, m33, m43,
                           m14, m24, m34, m44);
        }

        // Translation

        void translation(float x, float y, float z)
        {
            m11 = 1.0f; m12 = 0.0f; m13 = 0.0f; m14 = x;
            m21 = 0.0f; m22 = 1.0f; m23 = 0.0f; m24 = y;
            m31 = 0.0f; m32 = 0.0f; m33 = 1.0f; m34 = z;
            m41 = 0.0f; m42 = 0.0f; m43 = 0.0f; m44 = 1.0f;
        }

        void translation(const Vector3 &v);

        // Rotation

        void rotation_x(float theta)
        {
            const float c = std::cos(theta);
            const float s = std::sin(theta);

            m11 = 1.0f; m12 = 0.0f; m13 = 0.0f; m14 = 0.0f;
            m21 = 0.0f; m22 = c;    m23 = -s;   m24 = 0.0f;
            m31 = 0.0f; m32 = s;    m33 = c;    m34 = 0.0f;
            m41 = m42 = m43 = 0.0f; m44 = 1.0f;
        }

        void rotation_y(float theta)
        {
            const float c = std::cos(theta);
            const float s = std::sin(theta);

            m11 = c;    m12 = 0.0f; m13 = s;    m14 = 0.0f;
            m21 = 0.0f; m22 = 1.0f; m23 = 0.0f; m24 = 0.0f;
            m31 = -s;   m32 = 0.0f; m33 = c;    m34 = 0.0f;
            m41 = m42 = m43 = 0.0f; m44 = 1.0f;
        }

        void rotation_z(float theta)
        {
            const float c = std::cos(theta);
            const float s = std::sin(theta);

            m11 = c;    m12 = -s;   m13 = 0.0f; m14 = 0.0f;
            m21 = s;    m22 = c;    m23 = 0.0f; m24 = 0.0f;
            m31 = 0.0f; m32 = 0.0f; m33 = 1.0f; m34 = 0.0f;
            m41 = m42 = m43 = 0.0f; m44 = 1.0f;
        }

        void rotation_from_axis_angle(float x, float y, float z, float theta)
        {
            const float c = std::cos(theta);
            const float s = std::sin(theta);
            const float t = 1.0f - c;

            const float xyt = x * y * t;
            const float xzt = x * z * t;
            const float yzt = x * z * t;

            m11 = x * x * t + c;
            m12 = xyt - z * s;
            m13 = xzt + y * s;
            m14 = 0.0f;

            m21 = xyt + z * s;
            m22 = y * y * t + c;
            m23 = yzt - x * s;
            m24 = 0.0f;

            m31 = xzt - y * s;
            m32 = yzt + x * s;
            m33 = z * z * t + c;
            m34 = 0.0f;

            m41 = m42 = m43 = 0.0f; m44 = 1.0f;
        }

        void rotation_from_axis_angle(const Vector3 &axis, float theta);

        // Scale

        void scale(float x, float y, float z)
        {
            m11 = x;    m12 = 0.0f; m13 = 0.0f; m14 = 0.0f;
            m21 = 0.0f; m22 = y;    m23 = 0.0f; m24 = 0.0f;
            m31 = 0.0f; m32 = 0.0f; m33 = z;    m34 = 0.0f;
            m41 = m42 = m43 = 0.0f; m44 = 1.0f;
        }

        void scale(const Vector3 &v);

        // Operators

        bool operator == (const Matrix4 &a) const
        { return equals(a); }

        bool operator != (const Matrix4 &a) const
        { return !equals(a); }

        Matrix4& operator = (const Matrix4 &a)
        {
            m11 = a.m11; m12 = a.m12; m13 = a.m13; m14 = a.m14;
            m21 = a.m21; m22 = a.m22; m23 = a.m23; m24 = a.m24;
            m31 = a.m31; m32 = a.m32; m33 = a.m33; m34 = a.m34;
            m41 = a.m41; m42 = a.m42; m43 = a.m43; m44 = a.m44;
            return *this;
        }

        Matrix4& operator += (const Matrix4 &a)
        {
            m11 += a.m11; m12 += a.m12; m13 += a.m13; m14 += a.m14;
            m21 += a.m21; m22 += a.m22; m23 += a.m23; m24 += a.m24;
            m31 += a.m31; m32 += a.m32; m33 += a.m33; m34 += a.m34;
            m41 += a.m41; m42 += a.m42; m43 += a.m43; m44 += a.m44;
            return *this;
        }

        Matrix4& operator -= (const Matrix4 &a)
        {
            m11 -= a.m11; m12 -= a.m12; m13 -= a.m13; m14 -= a.m14;
            m21 -= a.m21; m22 -= a.m22; m23 -= a.m23; m24 -= a.m24;
            m31 -= a.m31; m32 -= a.m32; m33 -= a.m33; m34 -= a.m34;
            m41 -= a.m41; m42 -= a.m42; m33 -= a.m43; m34 -= a.m44;
            return *this;
        }

        Matrix4& operator *= (float s)
        {
            m11 *= s; m12 *= s; m13 *= s; m14 *= s;
            m21 *= s; m22 *= s; m23 *= s; m24 *= s;
            m31 *= s; m32 *= s; m33 *= s; m34 *= s;
            m41 *= s; m42 *= s; m43 *= s; m44 *= s;
            return *this;
        }

        Matrix4 operator - (int) const
        {
            return Matrix4(-m11, -m12, -m13, -m14,
                           -m21, -m22, -m23, -m24,
                           -m31, -m32, -m33, -m34,
                           -m41, -m42, -m43, -m44);
        }

        friend Matrix4 operator + (const Matrix4 &a, const Matrix4 &b)
        {
            return Matrix4(a.m11 + b.m11, a.m12 + b.m12, a.m13 + b.m13, a.m14 + b.m14,
                           a.m21 + b.m21, a.m22 + b.m22, a.m23 + b.m23, a.m24 + b.m24,
                           a.m31 + b.m31, a.m32 + b.m32, a.m33 + b.m33, a.m34 + b.m34,
                           a.m41 + b.m41, a.m42 + b.m42, a.m43 + b.m43, a.m44 + b.m44);
        }

        friend Matrix4 operator - (const Matrix4 &a, const Matrix4 &b)
        {
            return Matrix4(a.m11 - b.m11, a.m12 - b.m12, a.m13 - b.m13, a.m14 - b.m14,
                           a.m21 - b.m21, a.m22 - b.m22, a.m23 - b.m23, a.m24 - b.m24,
                           a.m31 - b.m31, a.m32 - b.m32, a.m33 - b.m33, a.m34 - b.m34,
                           a.m41 - b.m41, a.m42 - b.m42, a.m43 - b.m43, a.m44 - b.m44);
        }

        friend Matrix4 operator * (float s, const Matrix4 &a)
        {
            return Matrix4(a.m11 * s, a.m12 * s, a.m13 * s, a.m14 * s,
                           a.m21 * s, a.m22 * s, a.m23 * s, a.m24 * s,
                           a.m31 * s, a.m32 * s, a.m33 * s, a.m34 * s,
                           a.m41 * s, a.m42 * s, a.m43 * s, a.m44 * s);
        }

        friend Matrix4 operator * (const Matrix4 &a, float s)
        {
            return Matrix4(a.m11 * s, a.m12 * s, a.m13 * s, a.m14 * s,
                           a.m21 * s, a.m22 * s, a.m23 * s, a.m24 * s,
                           a.m31 * s, a.m32 * s, a.m33 * s, a.m34 * s,
                           a.m41 * s, a.m42 * s, a.m43 * s, a.m44 * s);
        }

        friend Matrix4 operator * (const Matrix4 &a, const Matrix4 &b)
        {
            return Matrix4(a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31 + a.m14 * b.m41,   // (a_r1|b_c1)
                           a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32 + a.m14 * b.m42,   // (a_r1|b_c2)
                           a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33 + a.m14 * b.m43,   // (a_r1|b_c3)
                           a.m11 * b.m14 + a.m12 * b.m24 + a.m13 * b.m34 + a.m14 * b.m44,   // (a_r1|b_c4)

                           a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31 + a.m24 * b.m41,   // (a_r2|b_c1)
                           a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32 + a.m24 * b.m42,   // (a_r2|b_c2)
                           a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33 + a.m24 * b.m43,   // (a_r2|b_c3)
                           a.m21 * b.m14 + a.m22 * b.m24 + a.m23 * b.m34 + a.m24 * b.m44,   // (a_r2|b_c4)

                           a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31 + a.m34 * b.m41,   // (a_r3|b_c1)
                           a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32 + a.m34 * b.m42,   // (a_r3|b_c2)
                           a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33 + a.m34 * b.m43,   // (a_r3|b_c3)
                           a.m31 * b.m14 + a.m32 * b.m24 + a.m33 * b.m34 + a.m34 * b.m44,   // (a_r3|b_c4)

                           a.m41 * b.m11 + a.m42 * b.m21 + a.m43 * b.m31 + a.m44 * b.m41,   // (a_r4|b_c1)
                           a.m41 * b.m12 + a.m42 * b.m22 + a.m43 * b.m32 + a.m44 * b.m42,   // (a_r4|b_c2)
                           a.m41 * b.m13 + a.m42 * b.m23 + a.m43 * b.m33 + a.m44 * b.m43,   // (a_r4|b_c3)
                           a.m41 * b.m14 + a.m42 * b.m24 + a.m43 * b.m34 + a.m44 * b.m44);  // (a_r4|b_c4)
        }
    };


    /// A space efficient row-major 4x4 matrix saved as 3x4 rows, with implicit fourth row as |0 0 0 1|.
    /// Used for transformations. It is preferred to use a 3x4 matrix over 4x4 matrix, if possible.
    struct Matrix3x4
    {
        static const Matrix3x4 zero;
        static const Matrix3x4 identity;

        union {
            struct {
                float m11, m12, m13, m14;
                float m21, m22, m23, m24;
                float m31, m32, m33, m34;
            };
            float m[3][4];
        };

        // Constructors

        /// Default constructor leaves the matrix in unitialized state.
        Matrix3x4() { }

        Matrix3x4(const Matrix3x4 &a)
            : m11(a.m11), m12(a.m12), m13(a.m13), m14(a.m14)
            , m21(a.m21), m22(a.m22), m23(a.m23), m24(a.m24)
            , m31(a.m31), m32(a.m32), m33(a.m33), m34(a.m34) { }

        Matrix3x4(float x)
            : m11(x), m12(x), m13(x), m14(x)
            , m21(x), m22(x), m23(x), m24(x)
            , m31(x), m32(x), m33(x), m34(x) { }

        Matrix3x4(float a11, float a12, float a13, float a14,
                  float a21, float a22, float a23, float a24,
                  float a31, float a32, float a33, float a34)
            : m11(a11), m12(a12), m13(a13), m14(a14)
            , m21(a21), m22(a22), m23(a23), m24(a24)
            , m31(a31), m32(a32), m33(a33), m34(a34) { }

        // Methods

        /// Returns true, if this matrix equals matrix \c a within tolerance of \c epsilon.
        bool equals(const Matrix3x4 &a, float epsilon) const
        {
            const float e = epsilon;
            return der::equals(m11, a.m11, e) && der::equals(m12, a.m12, e) && der::equals(m13, a.m13, e) && der::equals(m14, a.m14, e) &&
                der::equals(m21, a.m21, e) && der::equals(m22, a.m22, e) && der::equals(m23, a.m23, e) && der::equals(m24, a.m24, e) &&
                der::equals(m31, a.m31, e) && der::equals(m32, a.m32, e) && der::equals(m33, a.m33, e) && der::equals(m34, a.m34, e);
        }

        bool equals(const Matrix3x4 &a) const;

        Matrix4 transposed() const
        {
            return Matrix4(m11, m21, m31, 0.0f,
                           m12, m22, m32, 0.0f,
                           m13, m23, m33, 0.0f,
                           m14, m24, m34, 1.0f);
        }

        // Translation

        void translation(float x, float y, float z)
        {
            m11 = 1.0f; m12 = 0.0f; m13 = 0.0f; m14 = x;
            m21 = 0.0f; m22 = 1.0f; m23 = 0.0f; m24 = y;
            m31 = 0.0f; m32 = 0.0f; m33 = 1.0f; m34 = z;
        }

        void translation(const Vector3 &v);

        // Rotation

        void rotation_x(float theta)
        {
            const float c = std::cos(theta);
            const float s = std::sin(theta);

            m11 = 1.0f; m12 = 0.0f; m13 = 0.0f; m14 = 0.0f;
            m21 = 0.0f; m22 = c;    m23 = -s;   m24 = 0.0f;
            m31 = 0.0f; m32 = s;    m33 = c;    m34 = 0.0f;
        }

        void rotation_y(float theta)
        {
            const float c = std::cos(theta);
            const float s = std::sin(theta);

            m11 = c;    m12 = 0.0f; m13 = s;    m14 = 0.0f;
            m21 = 0.0f; m22 = 1.0f; m23 = 0.0f; m24 = 0.0f;
            m31 = -s;   m32 = 0.0f; m33 = c;    m34 = 0.0f;
        }

        void rotation_z(float theta)
        {
            const float c = std::cos(theta);
            const float s = std::sin(theta);

            m11 = c;    m12 = -s;   m13 = 0.0f; m14 = 0.0f;
            m21 = s;    m22 = c;    m23 = 0.0f; m24 = 0.0f;
            m31 = 0.0f; m32 = 0.0f; m33 = 1.0f; m34 = 0.0f;
        }

        void rotation_from_axis_angle(float x, float y, float z, float theta)
        {
            const float c = std::cos(theta);
            const float s = std::sin(theta);
            const float t = 1.0f - c;

            const float xyt = x * y * t;
            const float xzt = x * z * t;
            const float yzt = x * z * t;

            m11 = x * x * t + c;
            m12 = xyt - z * s;
            m13 = xzt + y * s;
            m14 = 0.0f;

            m21 = xyt + z * s;
            m22 = y * y * t + c;
            m23 = yzt - x * s;
            m24 = 0.0f;

            m31 = xzt - y * s;
            m32 = yzt + x * s;
            m33 = z * z * t + c;
            m34 = 0.0f;
        }

        void rotation_from_axis_angle(const Vector3 &axis, float theta);

        // Scale

        void scale(float x, float y, float z)
        {
            m11 = x;    m12 = 0.0f; m13 = 0.0f; m14 = 0.0f;
            m21 = 0.0f; m22 = y;    m23 = 0.0f; m24 = 0.0f;
            m31 = 0.0f; m32 = 0.0f; m33 = z;    m34 = 0.0f;
        }

        void scale(const Vector3 &v);

        // Operators

        bool operator == (const Matrix3x4 &a) const
        { return equals(a); }

        bool operator != (const Matrix3x4 &a) const
        { return !equals(a); }

        Matrix3x4& operator = (const Matrix3x4 &a)
        {
            m11 = a.m11; m12 = a.m12; m13 = a.m13; m14 = a.m14;
            m21 = a.m21; m22 = a.m22; m23 = a.m23; m24 = a.m24;
            m31 = a.m31; m32 = a.m32; m33 = a.m33; m34 = a.m34;
            return *this;
        }

        Matrix3x4& operator += (const Matrix3x4 &a)
        {
            m11 += a.m11; m12 += a.m12; m13 += a.m13; m14 += a.m14;
            m21 += a.m21; m22 += a.m22; m23 += a.m23; m24 += a.m24;
            m31 += a.m31; m32 += a.m32; m33 += a.m33; m34 += a.m34;
            return *this;
        }

        Matrix3x4& operator -= (const Matrix3x4 &a)
        {
            m11 -= a.m11; m12 -= a.m12; m13 -= a.m13; m14 -= a.m14;
            m21 -= a.m21; m22 -= a.m22; m23 -= a.m23; m24 -= a.m24;
            m31 -= a.m31; m32 -= a.m32; m33 -= a.m33; m34 -= a.m34;
            return *this;
        }

        Matrix3x4& operator *= (float s)
        {
            m11 *= s; m12 *= s; m13 *= s; m14 *= s;
            m21 *= s; m22 *= s; m23 *= s; m24 *= s;
            m31 *= s; m32 *= s; m33 *= s; m34 *= s;
            return *this;
        }

        Matrix3x4 operator - (int) const
        {
            return Matrix3x4(-m11, -m12, -m13, -m14,
                             -m21, -m22, -m23, -m24,
                             -m31, -m32, -m33, -m34);
        }

        friend Matrix3x4 operator + (const Matrix3x4 &a, const Matrix3x4 &b)
        {
            return Matrix3x4(a.m11 + b.m11, a.m12 + b.m12, a.m13 + b.m13, a.m14 + b.m14,
                             a.m21 + b.m21, a.m22 + b.m22, a.m23 + b.m23, a.m24 + b.m24,
                             a.m31 + b.m31, a.m32 + b.m32, a.m33 + b.m33, a.m34 + b.m34);
        }

        friend Matrix3x4 operator - (const Matrix3x4 &a, const Matrix3x4 &b)
        {
            return Matrix3x4(a.m11 - b.m11, a.m12 - b.m12, a.m13 - b.m13, a.m14 - b.m14,
                             a.m21 - b.m21, a.m22 - b.m22, a.m23 - b.m23, a.m24 - b.m24,
                             a.m31 - b.m31, a.m32 - b.m32, a.m33 - b.m33, a.m34 - b.m34);
        }

        friend Matrix3x4 operator * (float s, const Matrix3x4 &a)
        {
            return Matrix3x4(a.m11 * s, a.m12 * s, a.m13 * s, a.m14 * s,
                             a.m21 * s, a.m22 * s, a.m23 * s, a.m24 * s,
                             a.m31 * s, a.m32 * s, a.m33 * s, a.m34 * s);
        }

        friend Matrix3x4 operator * (const Matrix3x4 &a, float s)
        {
            return Matrix3x4(a.m11 * s, a.m12 * s, a.m13 * s, a.m14 * s,
                             a.m21 * s, a.m22 * s, a.m23 * s, a.m24 * s,
                             a.m31 * s, a.m32 * s, a.m33 * s, a.m34 * s);
        }

        friend Matrix3x4 operator * (const Matrix3x4 &a, const Matrix3x4 &b)
        {
            return Matrix3x4(a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31 + a.m14 * 0.0f,   // (a_r1|b_c1)
                             a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32 + a.m14 * 0.0f,   // (a_r1|b_c2)
                             a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33 + a.m14 * 0.0f,   // (a_r1|b_c3)
                             a.m11 * b.m14 + a.m12 * b.m24 + a.m13 * b.m34 + a.m14 * 1.0f,   // (a_r1|b_c4)

                             a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31 + a.m24 * 0.0f,   // (a_r2|b_c1)
                             a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32 + a.m24 * 0.0f,   // (a_r2|b_c2)
                             a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33 + a.m24 * 0.0f,   // (a_r2|b_c3)
                             a.m21 * b.m14 + a.m22 * b.m24 + a.m23 * b.m34 + a.m24 * 1.0f,   // (a_r2|b_c4)

                             a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31 + a.m34 * 0.0f,   // (a_r3|b_c1)
                             a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32 + a.m34 * 0.0f,   // (a_r3|b_c2)
                             a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33 + a.m34 * 0.0f,   // (a_r3|b_c3)
                             a.m31 * b.m14 + a.m32 * b.m24 + a.m33 * b.m34 + a.m34 * 1.0f);  // (a_r3|b_c4)
        }
    };

} // der

#endif // H_DER_MATRIX4_H

