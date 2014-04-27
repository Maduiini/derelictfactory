
#ifndef H_DER_QUATERNION_H
#define H_DER_QUATERNION_H

#include "Vector.h"
#include <cmath>

namespace der
{

    // Forward declarations

    bool equals(float, float, float);


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

        /// Constructs the quaternion with the vector and scalar parts.
        /// \param v_   The vector part of the quaternion.
        /// \param w_   The scalar part of the quaternion.
        Quaternion(const Vector3 &v, float w_)
            : x(v.x), y(v.y), z(v.z), w(w_) { }

        // Methods

        Vector3 xyz() const { return Vector3(x, y, z); }

        /// Returns true, if this quaternion and the quaternion \c q are the same within
        /// the tolerance of \c epsilon.
        bool equals(const Quaternion &q, float epsilon) const
        {
            return der::equals(x, q.x, epsilon) &&
                der::equals(y, q.y, epsilon) &&
                der::equals(z, q.z, epsilon) &&
                der::equals(w, q.w, epsilon);
        }

        bool equals(const Quaternion &q) const;

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

        bool operator == (const Quaternion &q) const
        { return equals(q); }

        bool operator != (const Quaternion &q) const
        { return !equals(q); }

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

        /// Sets the quaternion to rotation about x-axis.
        /// \param theta    The angle of the rotation in radians.
        void rotation_x(float theta)
        {
            const float c = std::cos(theta * 0.5f);
            const float s = std::sin(theta * 0.5f);
            x = s; y = 0.0f; z = 0.0f;
            w = c;
        }

        /// Sets the quaternion to rotation about y-axis.
        /// \param theta    The angle of the rotation in radians.
        void rotation_y(float theta)
        {
            const float c = std::cos(theta * 0.5f);
            const float s = std::sin(theta * 0.5f);
            x = 0.0f; y = s; z = 0.0f;
            w = c;
        }

        /// Sets the quaternion to rotation about z-axis.
        /// \param theta    The angle of the rotation in radians.
        void rotation_z(float theta)
        {
            const float c = std::cos(theta * 0.5f);
            const float s = std::sin(theta * 0.5f);
            x = 0.0f; y = 0.0f; z = s;
            w = c;
        }

        /// Constructs the quaternion from axis angle. The axis must be normalized.
        /// \param x_       X-coordinate of the axis.
        /// \param y_       Y-coordinate of the axis.
        /// \param z_       Z-coordinate of the axis.
        /// \param theta    The angle of the rotation in radians.
        void from_axis_angle(float x_, float y_, float z_, float theta)
        {
            const float c = std::cos(theta * 0.5f);
            const float s = std::sin(theta * 0.5f);
            x = s * x_; y = s * y_; z = s * z_;
            w = c;
        }

        /// Makes this quaternion into rotation from axis angle. The axis must be normalized.
        /// \param axis     The normalized axis of rotation.
        /// \param theta    The angle of the rotation in radians.
        void from_axis_angle(const Vector3 &axis, float theta);

        /// Extracts the axis and angle that this rotation repersents.
        void get_axis_angle(float &x_, float &y_, float &z_, float &theta)
        {
            const float half_theta = std::acos(w);
            const float s = std::sin(half_theta); // s = sin half theta
            x_ = x / s; y_ = y / s; z_ = z / s;
            theta = half_theta * 2.0f;
        }

        /// Extracts the axis and angle that this rotation repersents.
        void get_axis_angle(Vector3 &axis, float &theta)
        { get_axis_angle(axis.x, axis.y, axis.z, theta); }

        /// Sets the rotation from orthogonal basis vectors.
        void from_basis(const Vector3 &right, const Vector3 &up, const Vector3 &forward);

    };

    /// Linearly interpolates quaternions \c q1 and \c q2.
    Quaternion lerp(const Quaternion &q1, const Quaternion &q2, const float t);

    /// Spherical linear interpolation between quaternions \c q1 and \c q2.
    /// If t <= 0, returns q1. If t >= 1, returns q2. Selects the shortest path of rotation.
    Quaternion slerp(const Quaternion &q1, const Quaternion &q2, const float t);

} // der

#endif // H_DER_QUATERNION_H

