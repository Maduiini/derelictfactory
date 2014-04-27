
#include "Math.h"

#include <iostream>
#include <iomanip>

namespace der
{

    constexpr float Math::PI;
    constexpr float Math::E;
    constexpr float Math::DEG_2_RAD;
    constexpr float Math::RAD_2_DEG;
    constexpr float Math::EPSILON;


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

    std::ostream& fw(std::ostream &out, float x, size_t w = 6)
    {
        return out << std::setw(w) << x;
    }

    std::ostream& operator << (std::ostream &out, const Matrix3 &m)
    {
        const std::streamsize p = out.precision();
        out.precision(4);
        out << std::fixed;
        out << "|"; fw(out, m.m11); out << "  "; fw(out, m.m12); out << "  "; fw(out, m.m13); out << "|\n";
        out << "|"; fw(out, m.m21); out << "  "; fw(out, m.m22); out << "  "; fw(out, m.m23); out << "|\n";
        out << "|"; fw(out, m.m31); out << "  "; fw(out, m.m32); out << "  "; fw(out, m.m33); out << "|";
        out.precision(p);
        out << std::scientific;
        return out;
    }

    std::ostream& operator << (std::ostream &out, const Matrix4 &m)
    {
        const std::streamsize p = out.precision();
        out.precision(4);
        out << std::fixed;
        out << "|"; fw(out, m.m11); out << "  "; fw(out, m.m12); out << "  "; fw(out, m.m13); out << "  "; fw(out, m.m14); out << "|\n";
        out << "|"; fw(out, m.m21); out << "  "; fw(out, m.m22); out << "  "; fw(out, m.m23); out << "  "; fw(out, m.m24); out << "|\n";
        out << "|"; fw(out, m.m31); out << "  "; fw(out, m.m32); out << "  "; fw(out, m.m33); out << "  "; fw(out, m.m34); out << "|\n";
        out << "|"; fw(out, m.m41); out << "  "; fw(out, m.m42); out << "  "; fw(out, m.m43); out << "  "; fw(out, m.m44); out << "|";
        out.precision(p);
        out << std::scientific;
        return out;
    }

    std::ostream& operator << (std::ostream &out, const Matrix3x4 &m)
    {
        const std::streamsize p = out.precision();
        out.precision(4);
        out << std::fixed;
        out << "|"; fw(out, m.m11); out << "  "; fw(out, m.m12); out << "  "; fw(out, m.m13); out << "  "; fw(out, m.m14); out << "|\n";
        out << "|"; fw(out, m.m21); out << "  "; fw(out, m.m22); out << "  "; fw(out, m.m23); out << "  "; fw(out, m.m24); out << "|\n";
        out << "|"; fw(out, m.m31); out << "  "; fw(out, m.m32); out << "  "; fw(out, m.m33); out << "  "; fw(out, m.m34); out << "|\n";
        out << "|"; fw(out, 0.0f);  out << "  "; fw(out, 0.0f);  out << "  "; fw(out, 0.0f);  out << "  "; fw(out, 1.0f);  out << "|";
        out.precision(p);
        out << std::scientific;
        return out;
    }

} // der
