
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

        /// Constructs the quaternion from axis angle. The axis must be normalized.
        /// \param x_       X-coordinate of the axis.
        /// \param y_       Y-coordinate of the axis.
        /// \param z_       Z-coordinate of the axis.
        /// \param theta    The angle of the rotation in radians.
        Quaternion(float x_, float y_, float z_, float theta)
        {
            const float c = std::cos(theta * 0.5f);
            const float s = std::sin(theta * 0.5f);
            x = s * x_; y = s * y_; z = s * z_;
            w = c;
        }

        /// Constructs the quaternion from axis angle. The axis must be normalized.
        /// \param axis     The normalized axis of rotation.
        /// \param theta    The angle of the rotation in radians.
        Quaternion(const Vector3 &axis, float theta);

        Quaternion& operator = (const Quaternion &q)
        {
            x = q.x; y = q.y; z = q.z; w = q.w;
            return *this;
        }

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

        Quaternion conjugated() const
        { return Quaternion(-x, -y, -z, w); }

        // Operators

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

    };

    Quaternion slerp(const Quaternion &q1, const Quaternion &q2, const float t);

} // der

#endif // H_DER_QUATERNION_H

