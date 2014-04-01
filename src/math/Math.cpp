
#include "Math.h"

#include <iostream>

namespace der
{

    bool equals(float a, float b)
    {
        return equals(a, b, Math::EPSILON);
    }

    bool equals(float a, float b, float epsilon)
    {
        const float d = a - b;
        return -epsilon <= d && d <= epsilon;
    }

    std::ostream& operator << (std::ostream &out, const Vector3 &v)
    { return out << "(" << v.x << "," << v.y << "," << v.z << ")"; }

    std::ostream& operator << (std::ostream &out, const Vector4 &v)
    { return out << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")"; }

    std::ostream& operator << (std::ostream &out, const Quaternion &q)
    { return out << "Q(" << q.x << "," << q.y << "," << q.z << "," << q.w << ")"; }

    std::ostream& operator << (std::ostream &out, const Matrix3 &m)
    {
        return out << "|" << m.m11 << "," << m.m12 << "," << m.m13 << "|\n"
                    << "|" << m.m21 << "," << m.m22 << "," << m.m23 << "|\n"
                    << "|" << m.m31 << "," << m.m32 << "," << m.m33 << "|";
    }

    std::ostream& operator << (std::ostream &out, const Matrix4 &m)
    {
        return out << "|" << m.m11 << "," << m.m12 << "," << m.m13 << "," << m.m14 << "|\n"
                    << "|" << m.m21 << "," << m.m22 << "," << m.m23 << "," << m.m24 << "|\n"
                    << "|" << m.m31 << "," << m.m32 << "," << m.m33 << "," << m.m34 << "|\n"
                    << "|" << m.m41 << "," << m.m42 << "," << m.m43 << "," << m.m44 << "|";
    }

    std::ostream& operator << (std::ostream &out, const Matrix3x4 &m)
    {
        return out << "|" << m.m11 << "," << m.m12 << "," << m.m13 << "," << m.m14 << "|\n"
                    << "|" << m.m21 << "," << m.m22 << "," << m.m23 << "," << m.m24 << "|\n"
                    << "|" << m.m31 << "," << m.m32 << "," << m.m33 << "," << m.m34 << "|\n"
                    << "|" << 0 << "," << 0 << "," << 0 << "," << 1 << "|";
    }

} // der
