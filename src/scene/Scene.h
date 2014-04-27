
#ifndef H_DER_SCENE_H
#define H_DER_SCENE_H

#include <vector>
#include <unordered_map>
#include "../Types.h"

namespace der
{
    typedef unsigned int GameObjectID;
    class GameObject;

    constexpr GameObjectID NoObject(-1);

    class Scene
    {
    public:
        Scene();
        ~Scene();

        void reshape(int w, int h);

        size_t get_object_count() const;
        GameObject* get_object(const size_t index) const;
        const GameObject* get_object_by_id(GameObjectID id) const;
        GameObject* get_object_by_id(GameObjectID id);

        void get_visible_objects(std::vector<GameObject*> &objects) const;

        GameObject* new_object();
        void delete_object(GameObjectID id);

        void delete_all();

        void set_camera_object(GameObjectID id);
        GameObject *get_camera_object();

    private:
        std::vector<GameObject*> m_gameobjects;
        std::unordered_map<GameObjectID, GameObject*> m_gameobject_map;

        GameObjectID m_next_id;

        GameObjectID m_camera_object_id;

    };

} // der

#endif // H_DER_SCENE_H
