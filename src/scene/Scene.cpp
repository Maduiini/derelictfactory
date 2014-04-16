
#include "Scene.h"
#include "GameObject.h"
#include "../Debug.h"

namespace der
{

    Scene::Scene()
        : m_gameobjects()
        , m_next_id(0)
    { }

    Scene::~Scene()
    {
        auto iter = m_gameobjects.begin();
        for (; iter != m_gameobjects.end(); ++iter)
        {
            delete *iter;
        }
    }

    //void Scene::add(const GameObject *obj)
    //{
    //    m_gameobjects.push_back(obj);
    //}

    size_t Scene::get_gameobject_count() const
    {
        return m_gameobjects.size();
    }

    GameObject* Scene::get_gameobject(const size_t index) const
    {
        DER_ASSERT(index >= 0 && index < m_gameobjects.size());
        return m_gameobjects[index];
    }

    GameObject* Scene::new_gameobject()
    {
        GameObject *newobj = new GameObject(m_next_id++);
        m_gameobjects.push_back(newobj);
        return newobj;
    }

} // der
