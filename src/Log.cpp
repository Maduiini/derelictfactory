
#include "Log.h"
#include "math/Math.h"

#include <iostream>

namespace der
{
namespace log
{

    static Level g_level =
    #ifdef DER_DEBUG
        LEVEL_DEBUG;
    #else
        LEVEL_INFO;
    #endif

    void set_level(Level level)
    {
        g_level = level;
    }

    Level get_level()
    {
        return g_level;
    }

    void write(const char c)
    {
        std::cout.write(&c, 1);
    }

    void write(const signed char c)
    {
        std::cout.write(reinterpret_cast<const char*>(&c), 1);
    }

    void write(const unsigned char c)
    {
        std::cout.write(reinterpret_cast<const char*>(&c), 1);
    }

    void write(const char *msg)
    {
        while (*msg) write(*msg++);
    }

    void write(const short value)
    {
        std::cout << value;
    }

    void write(const unsigned short value)
    {
        std::cout << value;
    }

    void write(const int value)
    {
        std::cout << value;
    }

    void write(const unsigned int value)
    {
        std::cout << value;
    }

    void write(const long value)
    {
        std::cout << value;
    }

    void write(const unsigned long value)
    {
        std::cout << value;
    }

    void write(const long long value)
    {
        std::cout << value;
    }

    void write(const unsigned long long value)
    {
        std::cout << value;
    }


    void write(const float value)
    {
        std::cout << value;
    }

    void write(const double value)
    {
        std::cout << value;
    }

    //void write(const long double value)
    //{
    //    std::cout << value;
    //}

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


    void write(const Quaternion &q)
    {
        write("Q(");
        write(q.x); write(",");
        write(q.y); write(",");
        write(q.z); write(",");
        write(q.w); write(")");
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
