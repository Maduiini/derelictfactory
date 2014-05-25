
#ifndef H_DER_AABB_H
#define H_DER_AABB_H

#include "Vector.h"

namespace der
{

    struct Matrix4;

    struct Aabb
    {
        Vector3 m_min, m_max;

        // Initiaizes the aabb as degenerate
        Aabb() : m_min(1.0f), m_max(-1.0f) { }
        Aabb(const Vector3 &min, const Vector3 &max)
            : m_min(min), m_max(max)
        { }

        bool is_degenerate() const
        { return m_max.x < m_min.x; }

        void reset()
        {
            m_min = Vector3(1.0f);
            m_max = Vector3(-1.0f);
        }

        friend Aabb operator + (const Aabb &a, const Aabb &b)
        {
            if (a.is_degenerate())
            {
                return b;
            }
            else if (b.is_degenerate())
            {
                return a;
            }
            Aabb aabb(a);
            aabb.add_point(b.m_max);
            aabb.add_point(b.m_min);
            return aabb;
        }

        friend Aabb operator + (const Aabb &a, const Vector3 &v)
        {
            Aabb aabb(a);
            aabb.m_min += v;
            aabb.m_min += v;
            return aabb;
        }

        Aabb& operator += (const Aabb &a)
        {
            if (a.is_degenerate())
            {
                return *this;
            }
            else if (is_degenerate())
            {
                return *this = a;
            }
            add_point(a.m_max);
            add_point(a.m_min);
            return *this;
        }

        Aabb& operator += (const Vector3 &v)
        {
            m_min += v;
            m_min += v;
            return *this;
        }

        void add_point(const Vector3 &pt)
        { add_point(pt.x, pt.y, pt.z); }

        void add_point(float x, float y, float z)
        {
            m_min.x = (x < m_min.x) ? x : m_min.x;
            m_min.y = (y < m_min.y) ? y : m_min.y;
            m_min.z = (z < m_min.z) ? z : m_min.z;
            m_max.x = (x > m_max.x) ? x : m_max.x;
            m_max.y = (y > m_max.y) ? y : m_max.y;
            m_max.z = (z > m_max.z) ? z : m_max.z;
        }

        Vector3 get_size() const
        { return m_max - m_min; }

        /// Intersection with a sphere using Arvo's algorithm.
        bool intersects_sphere(const Vector3 &center, const float radius) const;

        void transform(const Matrix4 &mat);
    };

} // der

#endif // H_DER_AABB_H

