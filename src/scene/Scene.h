
#ifndef H_DER_SCENE_H
#define H_DER_SCENE_H

#include <vector>
#include <unordered_map>
#include <string>
#include "../Types.h"
#include "GameObject.h"

namespace der
{

    class ResourceCache;
    class QuadTree;

    struct Vector3;
    struct Aabb;

    typedef unsigned int GameObjectID;
    class GameObject;

    constexpr GameObjectID NoObject(-1);

    class Scene
    {
    public:
        Scene();
        ~Scene();

        void reshape(int w, int h);

        void update(ResourceCache *cache);

        size_t get_object_count() const;
        GameObject* get_object(const size_t index) const;
        const GameObject* get_object_by_id(GameObjectID id) const;
        GameObject* get_object_by_id(GameObjectID id);

        void get_all_objects(std::vector<GameObject*> &objects) const;
        void get_light_objects(const Aabb &aabb, std::vector<GameObject*> &objects) const;

        GameObject* new_object();
        void delete_object(GameObjectID id);

        void detach_object(GameObjectID id);
        void add_object(GameObject *obj);

        void delete_all();

        void set_camera_object(GameObjectID id);
        GameObject *get_camera_object();

        /// Gets the first directional light.
        GameObject *get_sun();

        QuadTree* get_quad_tree();

        void set_source(const std::string &scene_source);
        std::string get_source() const;

    private:
        std::vector<GameObject*> m_gameobjects;
        std::unordered_map<GameObjectID, GameObject*> m_gameobject_map;

        GameObjectID m_next_id;

        GameObjectID m_camera_object_id;
        GameObjectID m_sun_object_id;

        QuadTree *m_quad_tree;

        std::string m_scene_source;

    };

} // der

#endif // H_DER_SCENE_H
