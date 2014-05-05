
#include "Scene.h"
#include "Camera.h"
#include "Light.h"

#include "../Debug.h"

namespace der
{

    Scene::Scene()
        : m_gameobjects()
        , m_next_id(0)
        , m_camera_object_id(InvalidID)
        , m_scene_source()
    { }

    Scene::~Scene()
    {
        delete_all();
    }

    void Scene::reshape(int w, int h)
    {
        GameObject *camera_object = get_camera_object();
        if (camera_object)
        {
            Camera *camera = camera_object->get_camera();
            if (camera) camera->set_aspect_ratio(float(w) / h);
        }
    }

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

    void Scene::get_light_objects(const Vector3 &position, std::vector<GameObject*> &objects) const
    {
        auto iter = m_gameobjects.begin();
        for (; iter != m_gameobjects.end(); ++iter)
        {
            GameObject *object = *iter;
            Light *light = object->get_light();
            if (light)
            {
                const float r = light->get_radius();
                if (light->get_type() == LightType::Directional ||
                    (object->get_position() - position).length2() < r * r)
                {
                    objects.push_back(object);
                }
            }
        }
    }

    GameObject* Scene::new_object()
    {
        GameObject *newobj = new GameObject(m_next_id++);
        m_gameobjects.push_back(newobj);
        m_gameobject_map[newobj->getID()] = newobj;
        return newobj;
    }

    void Scene::detach_object(GameObjectID id)
    {
        GameObject *obj = get_object_by_id(id);
        DER_ASSERT(obj != nullptr);

        m_gameobject_map.erase(id);

        auto it = m_gameobjects.begin();
        for (; it != m_gameobjects.end(); ++it)
        {
            if (*it == obj)
            {
                obj->m_id = InvalidID;
                m_gameobjects.erase(it);
                return;
            }
        }
    }

    void Scene::add_object(GameObject *obj)
    {
        obj->m_id = m_next_id++;
        m_gameobjects.push_back(obj);
        m_gameobject_map[obj->getID()] = obj;
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
        m_camera_object_id = InvalidID;
    }

    void Scene::set_camera_object(GameObjectID id)
    { m_camera_object_id = id; }

    GameObject* Scene::get_camera_object()
    { return get_object_by_id(m_camera_object_id); }

    void Scene::set_source(const std::string &scene_source)
    { m_scene_source = scene_source; }

    std::string Scene::get_source() const
    { return m_scene_source; }

} // der
