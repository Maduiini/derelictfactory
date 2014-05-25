
#ifndef H_DER_FRUSTUM_H
#define H_DER_FRUSTUM_H

#include "Vector.h"

namespace der
{

    struct Aabb;
    struct Matrix4;

    struct Frustum
    {
        enum Face
        {
            Near, Far, Left, Right, Bottom, Top
        };
        Vector4 m_planes[6];

        static Vector4 make_plane(const Vector3 &center, const Vector3 &pos, const Vector3 &delta, const Vector3 &t);

        // windows.h or some other header defines these.. ;(
        #undef far
        #undef near

        /// Constructs the frustum from transform and perspective projection parameters.
        void construct(const Matrix4 &world_mat, float fov, float aspect, float near, float far);

        static void get_points(Vector3 (&pts)[5], const Matrix4 &world_mat,
                               float fov, float aspect, float near, float far);

        void extract(const Matrix4 &view_proj);

        bool intersects_sphere(const Vector3 &position, const float radius) const;
        bool intersects_aabb(const Aabb &aabb) const;
    };

} // der

#endif // H_DER_FRUSTUM_H

