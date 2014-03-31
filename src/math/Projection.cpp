
#include "Projection.h"
#include "Math.h"

namespace der
{

    Matrix4 projection_perspective_lh(float fov_y, float aspect_ratio,
                                      float near_z, float far_z)
    {
        const float tan_half_fov_y = std::tan(fov_y * 0.5f * Math::DEG_2_RAD);
        const float f = 1.0f / tan_half_fov_y;
        const float df = 1.0f / (near_z - far_z);

        const float A = f / aspect_ratio;
        const float B = (far_z + near_z) * df;
        const float C = (2.0f * far_z * near_z) * df;

        return Matrix4(
             A,   0.0f,  0.0f,  0.0f,
            0.0f,  f,    0.0f,  0.0f,
            0.0f, 0.0f,   B,     C,
            0.0f, 0.0f, 1.0f,  0.0f
        );
    }


    Matrix4 projection_perspective_rh(float fov_y, float aspect_ratio,
                                      float near_z, float far_z)
    {
        const float tan_half_fov_y = std::tan(fov_y * 0.5f * Math::DEG_2_RAD);
        const float f = 1.0f / tan_half_fov_y;
        const float df = 1.0f / (near_z - far_z);

        const float A = f / aspect_ratio;
        const float B = (far_z + near_z) * df;
        const float C = (2.0f * far_z * near_z) * df;

        return Matrix4(
             A,   0.0f,  0.0f,  0.0f,
            0.0f,  f,    0.0f,  0.0f,
            0.0f, 0.0f,   B,     C,
            0.0f, 0.0f, -1.0f,  0.0f
        );
    }


    Matrix4 projection_orthogonal_lh(float left, float right,
                                     float bottom, float top,
                                     float near, float far)
    {
        const float w = right - left;
        const float h = top - bottom;
        const float d = far - near;

        const float tx = -(right + left) / w;
        const float ty = -(top + bottom) / h;
        const float tz = -(far + near) / d;

        return Matrix4(
            2.0f / w, 0.0f,     0.0f,     tx,
            0.0f,     2.0f / h, 0.0f,     ty,
            0.0f,     0.0f,     2.0f / d, tz,
            0.0f,     0.0f,     0.0f,     1.0f
        );
    }


    Matrix4 projection_orthogonal_rh(float left, float right,
                                     float bottom, float top,
                                     float near, float far)
    {
        const float w = right - left;
        const float h = top - bottom;
        const float d = far - near;

        const float tx = -(right + left) / w;
        const float ty = -(top + bottom) / h;
        const float tz = -(far + near) / d;

        return Matrix4(
            2.0f / w, 0.0f,     0.0f,     tx,
            0.0f,     2.0f / h, 0.0f,     ty,
            0.0f,     0.0f,    -2.0f / d, tz,
            0.0f,     0.0f,     0.0f,     1.0f
        );
    }

} // der
