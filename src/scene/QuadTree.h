
#ifndef H_DER_QUAD_TREE_H
#define H_DER_QUAD_TREE_H

#include "../Types.h"
#include "../math/Vector.h"
#include "GameObject.h"

#include <vector>
#include <unordered_map>

namespace der
{

    class QuadTreeNode
    {
    public:
        QuadTreeNode();
        ~QuadTreeNode();

        void add_object(GameObject *object);
        void remove_object(GameObject *object);

        void set_child(size_t x, size_t y, QuadTreeNode *child);
        void set_geometry(float x, float y, float radius);
        float get_radius() const;

    private:
        std::vector<GameObject*> m_objects;

        QuadTreeNode *m_children[4];
        Vector2 m_center;
        float m_radius;
    };

    constexpr size_t QUAD_TREE_NODES(1UL+2UL*2UL+4UL*4UL+8UL*8UL+16UL*16UL+32UL*32UL+64UL*64UL+128UL*128UL);

    class QuadTree
    {
    public:
        QuadTree();
        ~QuadTree();

        void create(float size);
        void reset();

        const QuadTreeNode* get_root() const;

        void add_object(GameObject *object);
        void remove_object(GameObject *object);

    private:
        QuadTreeNode* get_root();

    private:
        QuadTreeNode *m_nodes;
        QuadTreeNode *m_levels[8];
        std::unordered_map<GameObjectID, QuadTreeNode*> m_object_node_map;

        float m_radius;
        float m_inv_radius;
    };

} // der

#endif // H_DER_QUAD_TREE_H

