
#include "GameObject.h"
#include "../Debug.h"

namespace der
{
    GameObjectID GameObject::nextID = 0;

    GameObject::GameObject()
        : m_id(nextID++)
        , m_position(Vector3::zero)
        , m_rotation(Quaternion::identity)
        , m_scale(1.0f)
        , m_parent(nullptr)
        , m_children()
        , m_mesh_renderer(nullptr)
    { }

    GameObject::~GameObject()
    { }

    GameObject* GameObject::get_parent() const
    {
        return m_parent;
    }

    const GameObjectID GameObject::getID() const
    {
        return m_id;
    }

    void GameObject::add_child(GameObject *obj)
    {
        if (obj)
        {
            obj->m_parent = this;
            remove_child(obj);
            m_children.push_back(obj);
        }
    }

    void GameObject::remove_child(GameObject *obj)
    {
        if (obj)
        {
            auto iter = m_children.begin();
            for (; iter != m_children.end(); ++iter)
            {
                if (*(*iter) == *obj)
                {
                    m_children.erase(iter);
                    return;
                }
            }
        }
    }

    GameObject* GameObject::get_child(const size_t index) const
    {
        DER_ASSERT(index >= 0 && index < m_children.size());
        return m_children[index];
    }

    const size_t GameObject::get_child_count() const
    {
        return m_children.size();
    }

    bool GameObject::operator == (const GameObject &obj) const
    {
        return m_id == obj.m_id;
    }

} // der
