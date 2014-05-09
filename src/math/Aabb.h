
#ifndef H_DER_AABB_H
#define H_DER_AABB_H

#include "Vector.h"

namespace der
{

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

        void add_point(const Vector3 &pt)
        {
            m_min.x = (pt.x < m_min.x) ? pt.x: m_min.x;
            m_min.y = (pt.y < m_min.y) ? pt.y: m_min.y;
            m_min.z = (pt.z < m_min.z) ? pt.z: m_min.z;
            m_max.x = (pt.x > m_max.x) ? pt.x: m_max.x;
            m_max.y = (pt.y > m_max.y) ? pt.y: m_max.y;
            m_max.z = (pt.z > m_max.z) ? pt.z: m_max.z;
        }

        Vector3 get_size() const
        { return m_max - m_min; }
    };

} // der

#endif // H_DER_AABB_H

