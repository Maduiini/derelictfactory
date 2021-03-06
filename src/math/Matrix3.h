
#ifndef H_DER_MATRIX3_H
#define H_DER_MATRIX3_H

#include <cmath>

namespace der
{

    // Forward declarations

    bool equals(float, float, float);
    struct Vector3;
    struct Quaternion;


    /// A row-major 3x3 matrix, mostly for rotations.
    struct Matrix3
    {
        static const Matrix3 zero;
        static const Matrix3 identity;

        union {
            struct {
                float m11, m12, m13;
                float m21, m22, m23;
                float m31, m32, m33;
            };
            float m[3][3];
        };

        // Constructors

        /// Default constructor leaves the matrix in unitialized state.
        Matrix3() { }

        Matrix3(const Matrix3 &a)
            : m11(a.m11), m12(a.m12), m13(a.m13)
            , m21(a.m21), m22(a.m22), m23(a.m23)
            , m31(a.m31), m32(a.m32), m33(a.m33) { }

        explicit Matrix3(float x)
            : m11(x), m12(x), m13(x)
            , m21(x), m22(x), m23(x)
            , m31(x), m32(x), m33(x) { }

        Matrix3(float a11, float a12, float a13,
                float a21, float a22, float a23,
                float a31, float a32, float a33)
            : m11(a11), m12(a12), m13(a13)
            , m21(a21), m22(a22), m23(a23)
            , m31(a31), m32(a32), m33(a33) { }

        // Methods

        /// Returns true, if this matrix equals matrix \c a within tolerance of \c epsilon.
        bool equals(const Matrix3 &a, float epsilon) const
        {
            return der::equals(m11, a.m11, epsilon) && der::equals(m12, a.m12, epsilon) && der::equals(m13, a.m13, epsilon) &&
                der::equals(m21, a.m21, epsilon) && der::equals(m22, a.m22, epsilon) && der::equals(m23, a.m23, epsilon) &&
                der::equals(m31, a.m31, epsilon) && der::equals(m32, a.m32, epsilon) && der::equals(m33, a.m33, epsilon);
        }

        bool equals(const Matrix3 &a) const;

        /// Returns the basis vectors of the rotation matrix.
        void get_basis(Vector3 &right, Vector3 &up, Vector3 &forward) const;
        /// Sets the rotation matrix from basis vectors.
        void from_basis(const Vector3 &right, const Vector3 &up, const Vector3 &forward);

        void transpose()
        { *this = transposed(); }

        Matrix3 transposed() const
        {
            return Matrix3(m11, m21, m31,
                           m12, m22, m32,
                           m13, m23, m33);
        }

        // Rotation

        void rotation_x(float theta)
        {
            const float c = std::cos(theta);
            const float s = std::sin(theta);

            m11 = 1.0f; m12 = 0.0f; m13 = 0.0f;
            m21 = 0.0f; m22 = c;    m23 = -s;
            m31 = 0.0f; m32 = s;    m33 = c;
        }

        void rotation_y(float theta)
        {
            const float c = std::cos(theta);
            const float s = std::sin(theta);

            m11 = c;    m12 = 0.0f; m13 = s;
            m21 = 0.0f; m22 = 1.0f; m23 = 0.0f;
            m31 = -s;   m32 = 0.0f; m33 = c;
        }

        void rotation_z(float theta)
        {
            const float c = std::cos(theta);
            const float s = std::sin(theta);

            m11 = c;    m12 = -s;   m13 = 0.0f;
            m21 = s;    m22 = c;    m23 = 0.0f;
            m31 = 0.0f; m32 = 0.0f; m33 = 1.0f;
        }

        void from_axis_angle(float x, float y, float z, float theta)
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

            m21 = xyt + z * s;
            m22 = y * y * t + c;
            m23 = yzt - x * s;

            m31 = xzt - y * s;
            m32 = yzt + x * s;
            m33 = z * z * t + c;
        }

        void from_axis_angle(const Vector3 &axis, float theta);

        void from_quaternion(const Quaternion &quat);

        // Scale

        void scale(float x, float y, float z)
        {
            m11 = x;    m12 = 0.0f; m13 = 0.0f;
            m21 = 0.0f; m22 = y;    m23 = 0.0f;
            m31 = 0.0f; m32 = 0.0f; m33 = z;
        }

        void scale(const Vector3 &v);

        // Operators

        bool operator == (const Matrix3 &a) const
        { return equals(a); }

        bool operator != (const Matrix3 &a) const
        { return !equals(a); }

        Matrix3& operator = (const Matrix3 &a)
        {
            m11 = a.m11; m12 = a.m12; m13 = a.m13;
            m21 = a.m21; m22 = a.m22; m23 = a.m23;
            m31 = a.m31; m32 = a.m32; m33 = a.m33;
            return *this;
        }

        Matrix3& operator += (const Matrix3 &a)
        {
            m11 += a.m11; m12 += a.m12; m13 += a.m13;
            m21 += a.m21; m22 += a.m22; m23 += a.m23;
            m31 += a.m31; m32 += a.m32; m33 += a.m33;
            return *this;
        }

        Matrix3& operator -= (const Matrix3 &a)
        {
            m11 -= a.m11; m12 -= a.m12; m13 -= a.m13;
            m21 -= a.m21; m22 -= a.m22; m23 -= a.m23;
            m31 -= a.m31; m32 -= a.m32; m33 -= a.m33;
            return *this;
        }

        Matrix3& operator *= (float s)
        {
            m11 *= s; m12 *= s; m13 *= s;
            m21 *= s; m22 *= s; m23 *= s;
            m31 *= s; m32 *= s; m33 *= s;
            return *this;
        }

        Matrix3 operator - (int) const
        {
            return Matrix3(-m11, -m12, -m13,
                           -m21, -m22, -m23,
                           -m31, -m32, -m33);
        }

        friend Matrix3 operator + (const Matrix3 &a, const Matrix3 &b)
        {
            return Matrix3(a.m11 + b.m11, a.m12 + b.m12, a.m13 + b.m13,
                           a.m21 + b.m21, a.m22 + b.m22, a.m23 + b.m23,
                           a.m31 + b.m31, a.m32 + b.m32, a.m33 + b.m33);
        }

        friend Matrix3 operator - (const Matrix3 &a, const Matrix3 &b)
        {
            return Matrix3(a.m11 - b.m11, a.m12 - b.m12, a.m13 - b.m13,
                           a.m21 - b.m21, a.m22 - b.m22, a.m23 - b.m23,
                           a.m31 - b.m31, a.m32 - b.m32, a.m33 - b.m33);
        }

        friend Matrix3 operator * (float s, const Matrix3 &a)
        {
            return Matrix3(a.m11 * s, a.m12 * s, a.m13 * s,
                           a.m21 * s, a.m22 * s, a.m23 * s,
                           a.m31 * s, a.m32 * s, a.m33 * s);
        }

        friend Matrix3 operator * (const Matrix3 &a, float s)
        {
            return Matrix3(a.m11 * s, a.m12 * s, a.m13 * s,
                           a.m21 * s, a.m22 * s, a.m23 * s,
                           a.m31 * s, a.m32 * s, a.m33 * s);
        }

        friend Matrix3 operator * (const Matrix3 &a, const Matrix3 &b)
        {
//            Matrix result;
//            for (int i = 0; i < 3; i++)
//            {
//                for (int j = 0; j < 3; j++)
//                {
//                    float dot = 0.0f;
//                    for (int k = 0; k < 3; k++)
//                        dot += a.m[i][k] * b.m[k][j];
//                    result.m[i][j] = dot;
//                }
//            }
//            return result;

            return Matrix3(a.m11 * b.m11 + a.m12 * b.m21 + a.m13 * b.m31,   // (a_r1|b_c1)
                           a.m11 * b.m12 + a.m12 * b.m22 + a.m13 * b.m32,   // (a_r1|b_c2)
                           a.m11 * b.m13 + a.m12 * b.m23 + a.m13 * b.m33,   // (a_r1|b_c3)

                           a.m21 * b.m11 + a.m22 * b.m21 + a.m23 * b.m31,   // (a_r2|b_c1)
                           a.m21 * b.m12 + a.m22 * b.m22 + a.m23 * b.m32,   // (a_r2|b_c2)
                           a.m21 * b.m13 + a.m22 * b.m23 + a.m23 * b.m33,   // (a_r2|b_c3)

                           a.m31 * b.m11 + a.m32 * b.m21 + a.m33 * b.m31,   // (a_r3|b_c1)
                           a.m31 * b.m12 + a.m32 * b.m22 + a.m33 * b.m32,   // (a_r3|b_c2)
                           a.m31 * b.m13 + a.m32 * b.m23 + a.m33 * b.m33);  // (a_r3|b_c3)
        }

    };

} // der

#endif // H_DER_MATRIX3_H

