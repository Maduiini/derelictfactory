
#ifndef H_DER_QUATERNION_H
#define H_DER_QUATERNION_H

#include <cmath>

namespace der
{

    // Forward declarations

    bool equals(float, float, float);

    struct Vector3;


    /// Quaternion that represent rotations in 3d-space.
    struct Quaternion
    {
        static const Quaternion identity;

        float x, y, z, w;

        // Constructors

        /// The default constructor does not initialize the quaternion.
        Quaternion() { }

        Quaternion(const Quaternion &q)
            : x(q.x), y(q.y), z(q.z), w(q.w) { }

        /// Constructs the quaternion with the vector and scalar parts.
        /// \param x_   X-coordinate of the vector part.
        /// \param y_   Y-coordinate of the vector part.
        /// \param z_   Z-coordinate of the vector part.
        /// \param w_   The scalar part of the quaternion.
        Quaternion(float x_, float y_, float z_, float w_)
            : x(x_), y(y_), z(z_), w(w_) { }

        // Methods

        /// Returns true, if this quaternion and the quaternion \c q are the same within
        /// the tolerance of \c epsilon.
        bool equals(const Quaternion &q, float epsilon) const
        {
            return der::equals(x, q.x, epsilon) &&
                der::equals(y, q.y, epsilon) &&
                der::equals(z, q.z, epsilon) &&
                der::equals(w, q.w, epsilon);
        }

        /// Squared length of the (x, y, z) part of the vector.
        float length2() const
        { return x * x + y * y + z * z + w * w; }

        /// Length of the (x, y, z) part of the vector.
        float length() const
        { return std::sqrt(length2()); }

        /// Normalizes this quaternion.
        void normalize()
        { *this = normalized(); }

        /// Returns the normalized (unit length) version of this quaternion.
        Quaternion normalized() const
        {
            const float l = length();
            return Quaternion(x / l, y / l, z / l, w / l);
        }

        Quaternion conjugated() const
        { return Quaternion(-x, -y, -z, w); }

        float dot(const Quaternion &q) const
        {
            return x * q.x + y * q.y + z * q.z + w * q.w;
        }

        // Operators

        Quaternion& operator = (const Quaternion &q)
        {
            x = q.x; y = q.y; z = q.z; w = q.w;
            return *this;
        }

        Quaternion& operator += (const Quaternion &q)
        {
            x += q.x; y += q.y; z += q.z; w += q.w;
            return *this;
        }

        Quaternion& operator -= (const Quaternion &q)
        {
            x -= q.x; y -= q.y; z -= q.z; w -= q.w;
            return *this;
        }

        Quaternion& operator *= (float s)
        {
            x *= s; y *= s; z *= s; w *= s;
            return *this;
        }

        Quaternion& operator *= (const Quaternion &q)
        {
            const float dot = x * q.x + y * q.y + z * q.z;
            const float tx = x;
            const float ty = y;
            const float tz = z;
            x = q.x * w + tx * q.w + ty * q.z - q.y * tz;
            y = q.y * w + ty * q.w + tz * q.x - q.z * tx;
            z = q.z * w + tz * q.w + tx * q.y - q.x * ty;
            w = w * q.w - dot;
            return *this;
        }

        Quaternion operator - () const
        {
            return Quaternion(-x, -y, -z, -w);
        }

        friend Quaternion operator + (const Quaternion &q1, const Quaternion &q2)
        {
            return Quaternion(q1.x + q2.x, q1.y + q2.y, q1.z + q2.z, q1.w + q2.w);
        }

        friend Quaternion operator - (const Quaternion &q1, const Quaternion &q2)
        {
            return Quaternion(q1.x - q2.x, q1.y - q2.y, q1.z - q2.z, q1.w - q2.w);
        }

        friend Quaternion operator * (float s, const Quaternion &q)
        {
            return Quaternion(q.x * s, q.y * s, q.z * s, q.w * s);
        }

        friend Quaternion operator * (const Quaternion &q, float s)
        {
            return Quaternion(q.x * s, q.y * s, q.z * s, q.w * s);
        }

        friend Quaternion operator * (const Quaternion &q1, const Quaternion &q2)
        {
            Quaternion q(q1);
            q *= q2;
            return q;
        }

        // Rotation

        /// Constructs the quaternion from axis angle. The axis must be normalized.
        /// \param x_       X-coordinate of the axis.
        /// \param y_       Y-coordinate of the axis.
        /// \param z_       Z-coordinate of the axis.
        /// \param theta    The angle of the rotation in radians.
        void rotation_from_axis_angle(float x_, float y_, float z_, float theta)
        {
            const float c = std::cos(theta * 0.5f);
            const float s = std::sin(theta * 0.5f);
            x = s * x_; y = s * y_; z = s * z_;
            w = c;
        }

        /// Makes this quaternion into rotation from axis angle. The axis must be normalized.
        /// \param axis     The normalized axis of rotation.
        /// \param theta    The angle of the rotation in radians.
        void rotation_from_axis_angle(const Vector3 &axis, float theta);

    };

    /// Linearly interpolates quaternions \c q1 and \c q2. Works ok, when the quaternions
    /// are almost equal.
    Quaternion lerp(const Quaternion &q1, const Quaternion &q2, const float t);

    /// Spherically interpolates quaternions \c q1 and \c q2.
    Quaternion slerp(const Quaternion &q1, const Quaternion &q2, const float t);

} // der

#endif // H_DER_QUATERNION_H

