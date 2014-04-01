
#include "Vector.h"
#include "Math.h"

namespace der
{

    // Vector3

    const Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);
    const Vector3 Vector3::unit_x(1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::unit_y(0.0f, 1.0f, 0.0f);
    const Vector3 Vector3::unit_z(0.0f, 0.0f, 1.0f);


    bool Vector3::equals(const Vector3 &v) const
    { return equals(v, Math::EPSILON); }

    Vector3 lerp(const Vector3 &v1, const Vector3 &v2, float t)
    { return v1 * (1.0f - t) + v2 * t; }

    // Vector4

    const Vector4 Vector4::zero(0.0f, 0.0f, 0.0f, 0.0f);
    const Vector4 Vector4::origin(0.0f, 0.0f, 0.0f, 1.0f);
    const Vector4 Vector4::unit_x(1.0f, 0.0f, 0.0f, 0.0f);
    const Vector4 Vector4::unit_y(0.0f, 1.0f, 0.0f, 0.0f);
    const Vector4 Vector4::unit_z(0.0f, 0.0f, 1.0f, 0.0f);
    const Vector4 Vector4::unit_w(0.0f, 0.0f, 0.0f, 1.0f);


    bool Vector4::equals(const Vector4 &v) const
    { return equals(v, Math::EPSILON); }

    Vector4 lerp(const Vector4 &v1, const Vector4 &v2, float t)
    { return v1 * (1.0f - t) + v2 * t; }

} // der
