
#ifndef H_DER_PROJECTION_H
#define H_DER_PROJECTION_H

#include "Matrix4.h"

namespace der
{

    /// Creates a left-handed perspective projection matrix: maps (+x, +y, +z) to (right, up, forward).
    /// \param fov_y        Field of view angle in degrees
    /// \param aspect_ratio The aspect ratio: screen_w / screen_h
    /// \param near_z       The z-coordinate of the near plane
    /// \param far_z        The z-coordinate of the fat plane
    Matrix4 projection_perspective_lh(float fov_y, float aspect_ratio,
                                      float near_z, float far_z);

    /// Creates a right-handed perspective projection matrix: maps (+x, +y, -z) to (right, up, forward).
    /// \param fov_y        Field of view angle in degrees
    /// \param aspect_ratio The aspect ratio: screen_w / screen_h
    /// \param near_z       The z-coordinate of the near plane
    /// \param far_z        The z-coordinate of the fat plane
    Matrix4 projection_perspective_rh(float fov_y, float aspect_ratio,
                                      float near_z, float far_z);


    /// Creates a left-handed orthogonal projection matrix.
    /// [left, right] maps to [-1, 1]
    /// [bottom, top] maps to [-1, 1]
    /// [near, far] maps to [-1, 1]
    /// \param left     Left of the viewport.
    /// \param right    Right of the viewport.
    /// \param bottom   Bottom of the viewport.
    /// \param top      Top of the viewport.
    /// \param near     The near plane.
    /// \param far      The far plane.
    Matrix4 projection_orthogonal_lh(float left, float right,
                                     float bottom, float top,
                                     float near, float far);


    /// Creates a right-handed orthogonal projection matrix.
    /// [left, right] maps to [-1, 1]
    /// [bottom, top] maps to [-1, 1]
    /// [-near, -far] maps to [-1, 1]
    /// \param left     Left of the viewport.
    /// \param right    Right of the viewport.
    /// \param bottom   Bottom of the viewport.
    /// \param top      Top of the viewport.
    /// \param near     The near plane.
    /// \param far      The far plane.
    Matrix4 projection_orthogonal_rh(float left, float right,
                                     float bottom, float top,
                                     float near, float far);

} // der

#endif // H_DER_PROJECTION_H

