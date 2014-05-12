
#include "QuadTree.h"
#include "GameObject.h"
#include "../math/Math.h"

namespace der
{

    QuadTreeNode::QuadTreeNode()
        : m_center(0.0f, 0.0f)
        , m_radius(0.0f)
    {
        m_children[0] = m_children[1] = m_children[2] = m_children[3] = nullptr;
    }

    QuadTreeNode::~QuadTreeNode()
    { }

    void QuadTreeNode::add_object(GameObject *object)
    {
        m_objects.push_back(object);
    }

    void QuadTreeNode::remove_object(GameObject *object)
    {
        auto it = m_objects.begin();
        for (; it != m_objects.end(); ++it)
        {
            if (*it == object)
            {
                m_objects.erase(it);
                return;
            }
        }
    }

    void QuadTreeNode::set_child(size_t x, size_t y, QuadTreeNode *child)
    { m_children[y * 2 + x] = child; }

    void QuadTreeNode::set_geometry(float x, float y, float radius)
    {
        m_center.x = x;
        m_center.y = y;
        m_radius = radius;
    }

    float QuadTreeNode::get_radius() const
    { return m_radius; }

    bool QuadTreeNode::is_leaf() const
    {
        return !m_children[0];
    }

    void QuadTreeNode::get_objects_by_radius(const Vector3 &position, float radius,
                                             std::vector<GameObject*> &objects)
    {
        for (GameObject *object : m_objects)
        {
            if (object->get_bounding_box().intersects_sphere(position, radius))
                objects.push_back(object);
        }
        if (is_leaf()) return;
        for (size_t i = 0; i < 4; i++)
        {
            QuadTreeNode *child = m_children[i];
//            const Vector3 radius_vec(child->m_radius);
            Vector3 radius_vec(child->m_radius);
            radius_vec.y = 0.0f;
//            const Vector3 center_vec(child->m_center.x, position.y, child->m_center.y);
            const Vector3 center_vec(child->m_center.x, 0.0f, child->m_center.y);

            const Aabb node_aabb(center_vec - radius_vec, center_vec + radius_vec);

//            if (node_aabb.intersects_sphere(position, radius))
                child->get_objects_by_radius(position, radius, objects);
        }
    }



    QuadTree::QuadTree()
        : m_nodes(nullptr)
    { }

    QuadTree::~QuadTree()
    { reset(); }

    void QuadTree::create(float size)
    {
        reset();
        m_nodes = new QuadTreeNode[QUAD_TREE_NODES];

        const float full_size = size;
        const float half_size = (size *= 0.5f);

        m_inv_radius = 255.0f / full_size;
        m_radius = half_size;

        QuadTreeNode *cur = m_nodes, *last = nullptr;
        Vector2 offset(-half_size, -half_size);
        for (size_t i = 0; i < 8; i++)
        {
            m_levels[i] = cur;
            const size_t grid_size = 1UL << i;
            for (size_t y = grid_size; y--; )
            {
                QuadTreeNode *row = cur + y * grid_size;
                const float pos_y = float(y) / float(grid_size) * full_size + half_size;
                for (size_t x = grid_size; x--; )
                {
                    const float pos_x = float(x) / float(grid_size) * full_size + half_size;
                    row[x].set_geometry(pos_x + offset.x, pos_y + offset.y, size);

                    if (last)
                    {
                        const size_t parent_x = x >> 1UL;
                        const size_t parent_y = y >> 1UL;
                        const size_t parent_gs = 1UL << (i - 1UL);
                        const size_t parent_idx = parent_y * parent_gs + parent_x;
                        last[parent_idx].set_child(x & 1, y & 1, &row[x]);
                    }
                }
            }
            last = cur;
            cur += grid_size * grid_size;
            size *= 0.5f;
        }
    }

    void QuadTree::reset()
    {
        delete[] m_nodes;
        m_nodes = nullptr;
    }

    static size_t highest_bit(size_t x)
    {
//        x |= (x >> 1);
//        x |= (x >> 2);
//        x |= (x >> 4);
//        x |= (x >> 8);
//        x |= (x >> 16);
//        return x - (x >> 1);
        size_t bit = 0;
        while (x)
        {
            bit++;
            x >>= 1;
        }
        return bit;
    }

    void QuadTree::add_object(GameObject *object)
    {
        Aabb aabb = object->get_bounding_box();
        if (aabb.m_min.x < -m_radius || aabb.m_max.x > m_radius ||
            aabb.m_min.z < -m_radius || aabb.m_max.z > m_radius)
        {
            get_root()->add_object(object);
            return;
        }

        const Vector3 offset(m_radius, m_radius, m_radius);
        Aabb shifted(aabb);
        shifted.m_min += offset;
        shifted.m_max += offset;
        shifted.m_min *= m_inv_radius;
        shifted.m_max *= m_inv_radius;

        const size_t min_x = static_cast<size_t>(clamp(std::floor(shifted.m_min.x), 0.0f, 255.0f));
        const size_t min_y = static_cast<size_t>(clamp(std::floor(shifted.m_min.z), 0.0f, 255.0f));
        const size_t max_x = static_cast<size_t>(clamp(std::ceil(shifted.m_max.x), 0.0f, 255.0f));
        const size_t max_y = static_cast<size_t>(clamp(std::ceil(shifted.m_max.z), 0.0f, 255.0f));

        const size_t x_xor = (min_x ^ max_x);
        const size_t lx = !x_xor ? 7UL : 8UL - highest_bit(x_xor);
        const size_t y_xor = (min_y ^ max_y);
        const size_t ly = !y_xor ? 7UL : 8UL - highest_bit(y_xor);

        const size_t level = (lx < ly) ? lx : ly;
        const size_t x = lx >> (8UL - level);
        const size_t y = ly >> (8UL - level);

        QuadTreeNode &node = m_levels[level][y * (1UL << level) + x];
        m_object_node_map[object->getID()] = &node;
        node.add_object(object);
    }

    void QuadTree::remove_object(GameObject *object)
    {
        QuadTreeNode *node = m_object_node_map[object->getID()];
        node->remove_object(object);
        m_object_node_map.erase(object->getID());
    }

    void QuadTree::update_object(GameObject *object)
    {
        QuadTreeNode *node = m_object_node_map[object->getID()];
        node->remove_object(object);

        add_object(object);
    }

    QuadTreeNode* QuadTree::get_root()
    { return m_nodes; }


    void QuadTree::get_objects_by_radius(const Vector3 &position, float radius,
                                         std::vector<GameObject*> &objects)
    {
        get_root()->get_objects_by_radius(position, radius, objects);
    }

} // der
