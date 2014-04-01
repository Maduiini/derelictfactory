
#include "Vector.h"

namespace der
{

    const Vector3 Vector3::zero(0.0f, 0.0f, 0.0f);
    const Vector3 Vector3::unit_x(1.0f, 0.0f, 0.0f);
    const Vector3 Vector3::unit_y(0.0f, 1.0f, 0.0f);
    const Vector3 Vector3::unit_z(0.0f, 0.0f, 1.0f);

    const Vector4 Vector4::zero(0.0f, 0.0f, 0.0f, 0.0f);
    const Vector4 Vector4::origin(0.0f, 0.0f, 0.0f, 1.0f);
    const Vector4 Vector4::unit_x(1.0f, 0.0f, 0.0f, 0.0f);
    const Vector4 Vector4::unit_y(0.0f, 1.0f, 0.0f, 0.0f);
    const Vector4 Vector4::unit_z(0.0f, 0.0f, 1.0f, 0.0f);
    const Vector4 Vector4::unit_w(0.0f, 0.0f, 0.0f, 1.0f);

} // der
