
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
        delete_all();
    }

    //void Scene::add(const GameObject *obj)
    //{
    //    m_gameobjects.push_back(obj);
    //}

    size_t Scene::get_object_count() const
    {
        return m_gameobjects.size();
    }

    GameObject* Scene::get_object(const size_t index) const
    {
        DER_ASSERT(index >= 0 && index < m_gameobjects.size());
        return m_gameobjects[index];
    }

    const GameObject* Scene::get_object_by_id(GameObjectID id) const
    {
        auto it = m_gameobject_map.find(id);
        if (it != m_gameobject_map.end())
            return it->second;
        return nullptr;
    }

    GameObject* Scene::get_object_by_id(GameObjectID id)
    {
        auto it = m_gameobject_map.find(id);
        if (it != m_gameobject_map.end())
            return it->second;
        return nullptr;
    }

    void Scene::get_visible_objects(std::vector<GameObject*> &objects) const
    {
        auto iter = m_gameobjects.begin();
        for (; iter != m_gameobjects.end(); ++iter)
        {
            objects.push_back(*iter);
        }
    }

    GameObject* Scene::new_object()
    {
        GameObject *newobj = new GameObject(m_next_id++);
        m_gameobjects.push_back(newobj);
        m_gameobject_map[newobj->getID()] = newobj;
        return newobj;
    }

    void Scene::delete_all()
    {
        auto iter = m_gameobjects.begin();
        for (; iter != m_gameobjects.end(); ++iter)
        {
            delete *iter;
        }
        m_gameobjects.clear();
        m_gameobject_map.clear();
    }

} // der
