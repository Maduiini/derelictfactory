
#include "Vector.h"
#include "../Log.h"

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

namespace log
{

    void write(const der::Vector3 &v)
    {
        write("(");
        write(v.x); write(",");
        write(v.y); write(",");
        write(v.z); write(")");
    }

    void write(const Vector4 &v)
    {
        write("(");
        write(v.x); write(",");
        write(v.y); write(",");
        write(v.z); write(",");
        write(v.w); write(")");
    }

} // log

} // der
