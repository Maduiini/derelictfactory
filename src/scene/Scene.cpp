
#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "QuadTree.h"

#include "../Debug.h"

namespace der
{

    Scene::Scene()
        : m_gameobjects()
        , m_next_id(0)
        , m_camera_object_id(InvalidID)
        , m_quad_tree(nullptr)
        , m_scene_source()
    {
        m_quad_tree = new QuadTree();
        m_quad_tree->create(2000.0f);
    }

    Scene::~Scene()
    {
        delete_all();
        delete m_quad_tree;
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


    void Scene::update(ResourceCache *cache)
    {
        for (GameObject *object : m_gameobjects)
        {
            object->update_bounding_box(cache);
        }

        for (GameObject *object : m_gameobjects)
        {
            m_quad_tree->update_object(object);
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

    void Scene::get_visible_objects(const Vector3 &position, std::vector<GameObject*> &objects) const
    {
        m_quad_tree->get_objects_by_radius(position, 20.0f, objects);
//        auto iter = m_gameobjects.begin();
//        for (; iter != m_gameobjects.end(); ++iter)
//        {
//            objects.push_back(*iter);
//        }
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
        m_quad_tree->add_object(newobj);
        return newobj;
    }

    void Scene::detach_object(GameObjectID id)
    {
        GameObject *obj = get_object_by_id(id);
        DER_ASSERT(obj != nullptr);

        m_quad_tree->remove_object(obj);
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
        m_quad_tree->add_object(obj);
    }

    void Scene::delete_all()
    {
        m_quad_tree->reset();
        m_quad_tree->create(2000.0f);

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

    QuadTree* Scene::get_quad_tree()
    { return m_quad_tree; }

    void Scene::set_source(const std::string &scene_source)
    { m_scene_source = scene_source; }

    std::string Scene::get_source() const
    { return m_scene_source; }

} // der
