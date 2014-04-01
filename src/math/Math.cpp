
#include "Math.h"
#include "../Log.h"

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


    namespace log
    {

        void write(const Vector3 &v)
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


        void write(const Matrix3 &m)
        {
            write('|'); write(m.m11); write("  "); write(m.m12); write("  "); write(m.m13); write("|\n");
            write('|'); write(m.m21); write("  "); write(m.m22); write("  "); write(m.m23); write("|\n");
            write('|'); write(m.m31); write("  "); write(m.m32); write("  "); write(m.m33); write('|');
        }

        void write(const Matrix4 &m)
        {
            write('|'); write(m.m11); write("  "); write(m.m12); write("  "); write(m.m13); write("  "); write(m.m14); write("|\n");
            write('|'); write(m.m21); write("  "); write(m.m22); write("  "); write(m.m23); write("  "); write(m.m24); write("|\n");
            write('|'); write(m.m31); write("  "); write(m.m32); write("  "); write(m.m33); write("  "); write(m.m34); write("|\n");
            write('|'); write(m.m41); write("  "); write(m.m42); write("  "); write(m.m43); write("  "); write(m.m44); write('|');
        }

        void write(const Matrix3x4 &m)
        {
            write('|'); write(m.m11); write("  "); write(m.m12); write("  "); write(m.m13); write("  "); write(m.m14); write("|\n");
            write('|'); write(m.m21); write("  "); write(m.m22); write("  "); write(m.m23); write("  "); write(m.m24); write("|\n");
            write('|'); write(m.m31); write("  "); write(m.m32); write("  "); write(m.m33); write("  "); write(m.m34); write("|\n");
            write('|'); write(0.0f); write("  "); write(0.0f); write("  "); write(0.0f); write("  "); write(1.0f); write('|');
        }

    } // log

} // der
