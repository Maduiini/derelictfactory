
#ifndef H_DER_VECTOR_H
#define H_DER_VECTOR_H

#include <cmath> // for std::sqrt

namespace der
{

    // Forward declarations

    bool equals(float, float, float);


    /// A 2d vector.
    /// \note The default constructor does not initialize the vector.
    struct Vector2
    {
        static const Vector2 zero;      /// Vector (0, 0)
        static const Vector2 unit_x;    /// Unit vector in x-direction
        static const Vector2 unit_y;    /// Unit vector in y-direction

        float x, y;

        // Constructors

        /// Default constructor leaves the vector in uninitialized state.
        Vector2() { }

        Vector2(const Vector2 &v)
            : x(v.x), y(v.y) { }

        Vector2(float x_, float y_)
            : x(x_), y(y_) { }

        explicit Vector2(float x_)
            : x(x_), y(x_) { }

        // Methods

        bool equals(const Vector2 &v, float epsilon) const
        {
            return der::equals(x, v.x, epsilon) &&
                der::equals(y, v.y, epsilon);
        }

        bool equals(const Vector2 &v) const;

        /// Squared length of the vector.
        float length2() const
        { return x * x + y * y; }

        /// Length of the vector.
        float length() const
        { return std::sqrt(length2()); }

        /// Returns the normalized (unit length) version of this vector.
        void normalize()
        { *this = normalized(); }

        /// Returns the normalized (unit length) version of this vector.
        Vector2 normalized() const
        {
            const float l = length();
            return Vector2(x / l, y / l);
        }

        /// Dot product of this a vector and \c v vector.
        float dot(const Vector2 &v) const
        { return x * v.x + y * v.y; }

        /// Projects a copy of this vector onto the \c v vector.
        Vector2 projected_onto(const Vector2 &v) const
        {
            Vector2 p(*this);
            return p.dot(v) / (p.length() + v.length()) * v;
        }

        // Operators

        bool operator == (const Vector2 &v) const
        { return equals(v); }

        bool operator != (const Vector2 &v) const
        { return !equals(v); }

        Vector2& operator = (const Vector2 &v)
        {
            x = v.x; y = v.y;
            return *this;
        }

        Vector2& operator += (const Vector2 &v)
        {
            x += v.x; y += v.y;
            return *this;
        }

        Vector2& operator -= (const Vector2 &v)
        {
            x -= v.x; y -= v.y;
            return *this;
        }

        Vector2& operator *= (float s)
        {
            x *= s; y *= s;
            return *this;
        }

        Vector2& operator /= (float s)
        {
            x /= s; y /= s;
            return *this;
        }

        Vector2 operator - () const
        { return Vector2(-x, -y); }

        friend Vector2 operator + (const Vector2 &u, const Vector2 &v)
        { return Vector2(u.x + v.x, u.y + v.y); }

        friend Vector2 operator - (const Vector2 &u, const Vector2 &v)
        { return Vector2(u.x - v.x, u.y - v.y); }

        friend Vector2 operator * (float s, const Vector2 &v)
        { return Vector2(v.x * s, v.y * s); }

        friend Vector2 operator * (const Vector2 &v, float s)
        { return Vector2(v.x * s, v.y * s); }

    };

    /// Vector2 linear interpolation.
    Vector2 lerp(const Vector2 &v1, const Vector2 &v2, float t);


    /// A 3d vector.
    /// \note The default constructor does not initialize the vector.
    struct Vector3
    {
        static const Vector3 zero;      /// Vector (0, 0, 0)
        static const Vector3 unit_x;    /// Unit vector in x-direction
        static const Vector3 unit_y;    /// Unit vector in y-direction
        static const Vector3 unit_z;    /// Unit vector in z-direction

        float x, y, z; //, w;

        // Constructors

        /// Default constructor leaves the vector in uninitialized state.
        Vector3() { }

        Vector3(const Vector3 &v)
            : x(v.x), y(v.y), z(v.z) { }

        Vector3(float x_, float y_, float z_)
            : x(x_), y(y_), z(z_) { }

        explicit Vector3(float x_)
            : x(x_), y(x_), z(x_) { }

        // Methods

        bool equals(const Vector3 &v, float epsilon) const
        {
            return der::equals(x, v.x, epsilon) &&
                der::equals(y, v.y, epsilon) &&
                der::equals(z, v.z, epsilon);
        }

        bool equals(const Vector3 &v) const;

        /// Squared length of the vector.
        float length2() const
        { return x * x + y * y + z * z; }

        /// Length of the vector.
        float length() const
        { return std::sqrt(length2()); }

        /// Returns the normalized (unit length) version of this vector.
        void normalize()
        { *this = normalized(); }

        /// Returns the normalized (unit length) version of this vector.
        Vector3 normalized() const
        {
            const float l = length();
            return Vector3(x / l, y / l, z / l);
        }

        /// Dot product of this a vector and \c v vector.
        float dot(const Vector3 &v) const
        { return x * v.x + y * v.y + z * v.z; }

        /// Cross product of this a vector and \c v vector.
        Vector3 cross(const Vector3 &v) const
        {
            return Vector3(y * v.z - v.y * z,
                           z * v.x - v.z * x,
                           x * v.y - v.x * y);
        }

        /// Projects a copy of this vector onto the \c v vector.
        Vector3 projected_onto(const Vector3 &v) const
        {
            Vector3 p(*this);
            return p.dot(v) / (p.length() + v.length()) * v;
        }

        // Operators

        bool operator == (const Vector3 &v) const
        { return equals(v); }

        bool operator != (const Vector3 &v) const
        { return !equals(v); }

        Vector3& operator = (const Vector3 &v)
        {
            x = v.x; y = v.y; z = v.z;
            return *this;
        }

        Vector3& operator += (const Vector3 &v)
        {
            x += v.x; y += v.y; z += v.z;
            return *this;
        }

        Vector3& operator -= (const Vector3 &v)
        {
            x -= v.x; y -= v.y; z -= v.z;
            return *this;
        }

        Vector3& operator *= (float s)
        {
            x *= s; y *= s; z *= s;
            return *this;
        }

        Vector3& operator /= (float s)
        {
            x /= s; y /= s; z /= s;
            return *this;
        }

        Vector3 operator - () const
        { return Vector3(-x, -y, -z); }

        friend Vector3 operator + (const Vector3 &u, const Vector3 &v)
        { return Vector3(u.x + v.x, u.y + v.y, u.z + v.z); }

        friend Vector3 operator - (const Vector3 &u, const Vector3 &v)
        { return Vector3(u.x - v.x, u.y - v.y, u.z - v.z); }

        friend Vector3 operator * (float s, const Vector3 &v)
        { return Vector3(v.x * s, v.y * s, v.z * s); }

        friend Vector3 operator * (const Vector3 &v, float s)
        { return Vector3(v.x * s, v.y * s, v.z * s); }

    };

    /// Vector3 linear interpolation.
    Vector3 lerp(const Vector3 &v1, const Vector3 &v2, float t);


    /// A homogenous 3d vector.
    /// \note The default constructor does not initialize the vector.
    ///     Also, most of the operators or operations on the vector do not
    ///     take into account the homogenous w-coordinate. The w-coordinate
    ///     is initialized to 1, when using any explicit constructor that
    ///     does not set the w-coordinate.
    struct Vector4
    {
        static const Vector4 zero;      /// Vector (0, 0, 0, 0)
        static const Vector4 origin;    /// Point (0, 0, 0, 1)
        static const Vector4 unit_x;    /// Unit vector in x-direction
        static const Vector4 unit_y;    /// Unit vector in y-direction
        static const Vector4 unit_z;    /// Unit vector in z-direction
        static const Vector4 unit_w;    /// Unit vector in w-direction

        float x, y, z, w;

        // Constructors

        /// Default constructor leaves the vector in uninitialized state.
        Vector4() { }

        Vector4(const Vector4 &v)
            : x(v.x), y(v.y), z(v.z), w(v.w) { }

        Vector4(float x_, float y_, float z_, float w_ = 1.0f)
            : x(x_), y(y_), z(z_), w(w_) { }

        explicit Vector4(float x_)
            : x(x_), y(x_), z(x_), w(1.0f) { }

        explicit Vector4(const Vector3 &v, float w)
            : x(v.x), y(v.x), z(v.x), w(w) { }

        // Methods

        /// Conversion to Vector3
        Vector3 xyz() const
        { return Vector3(x, y, z); }

        bool equals(const Vector4 &v, float epsilon) const
        {
            return der::equals(x, v.x, epsilon) &&
                der::equals(y, v.y, epsilon) &&
                der::equals(z, v.z, epsilon);
        }

        bool equals(const Vector4 &v) const;

        /// Squared length of the (x, y, z) part of the vector.
        float length2() const
        { return x * x + y * y + z * z; }

        /// Length of the (x, y, z) part of the vector.
        float length() const
        { return std::sqrt(length2()); }

        /// Normalizes this vector.
        void normalize()
        { *this = normalized(); }

        /// Returns the normalized (unit length) version of this vector.
        Vector4 normalized() const
        {
            const float l = length();
            return Vector4(x / l, y / l, z / l, w);
        }

        /// Dot product of two vectors. The vectors are treated as 3-dimensional,
        /// so the w-coordinate does not affect the result.
        float dot(const Vector4 &v) const
        { return x * v.x + y * v.y + z * v.z; }

        /// Cross product of two vectors. The vectors are treated as 3-dimensional,
        /// so the w-coordinate does not affect the result.
        /// The w-coordinate of the result is set to zero.
        Vector4 cross(const Vector4 &v) const
        {
            return Vector4(y * v.z - v.y * z,
                           z * v.x - v.z * x,
                           x * v.y - v.x * y, 0.0f);
        }

        /// Projects a copy of this vector onto the \c v vector. The vectors are
        /// treated as 3-dimensional, so the w-coordinate does not affect the result.
        /// The w-coordinate of the result is set to zero;
        Vector4 projected_onto(const Vector4 &v) const
        {
            Vector4 p(*this);
            return p.dot(v) / (p.length() + v.length()) * Vector4(v.xyz(), 0.0f);
        }

        // Operators

        Vector4& operator = (const Vector4 &v)
        {
            x = v.x; y = v.y; z = v.z; w = v.w;
            return *this;
        }

        bool operator == (const Vector4 &v) const
        { return equals(v); }

        bool operator != (const Vector4 &v) const
        { return !equals(v); }

        Vector4& operator += (const Vector4 &v)
        {
            x += v.x; y += v.y; z += v.z; w += v.w;
            return *this;
        }

        Vector4& operator -= (const Vector4 &v)
        {
            x -= v.x; y -= v.y; z -= v.z; w -= v.w;
            return *this;
        }

        Vector4& operator *= (float s)
        {
            x *= s; y *= s; z *= s; w *= s;
            return *this;
        }

        Vector4& operator /= (float s)
        {
            x /= s; y /= s; z /= s; w /= s;
            return *this;
        }

        Vector4 operator - () const
        { return Vector4(-x, -y, -z, -w); }

        friend Vector4 operator + (const Vector4 &u, const Vector4 &v)
        { return Vector4(u.x + v.x, u.y + v.y, u.z + v.z, u.w + v.w); }

        friend Vector4 operator - (const Vector4 &u, const Vector4 &v)
        { return Vector4(u.x - v.x, u.y - v.y, u.z - v.z, u.w - v.w); }

        friend Vector4 operator * (float s, const Vector4 &v)
        { return Vector4(v.x * s, v.y * s, v.z * s, v.w * s); }

        friend Vector4 operator * (const Vector4 &v, float s)
        { return Vector4(v.x * s, v.y * s, v.z * s, v.w * s); }

    };

    /// Vector4 linear interpolation.
    Vector4 lerp(const Vector4 &v1, const Vector4 &v2, float t);

} // der

#endif // H_DER_VECTOR_H

