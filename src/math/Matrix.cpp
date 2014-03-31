
#include "Matrix3.h"
#include "Matrix4.h"
#include "../Log.h"

namespace der
{

    const Matrix3 Matrix3::zero(0.0f);
    const Matrix3 Matrix3::identity(1.0f, 0.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f);

    const Matrix4 Matrix4::zero(0.0f);
    const Matrix4 Matrix4::identity(1.0f, 0.0f, 0.0f, 0.0f,
                                    0.0f, 1.0f, 0.0f, 0.0f,
                                    0.0f, 0.0f, 1.0f, 0.0f,
                                    0.0f, 0.0f, 0.0f, 1.0f);

    const Matrix3x4 Matrix3x4::zero(0.0f);
    const Matrix3x4 Matrix3x4::identity(1.0f, 0.0f, 0.0f, 0.0f,
                                        0.0f, 1.0f, 0.0f, 0.0f,
                                        0.0f, 0.0f, 1.0f, 0.0f);

    namespace log
    {

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
