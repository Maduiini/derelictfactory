
#ifndef H_DER_SCENE_H
#define H_DER_SCENE_H

#include <vector>
#include <unordered_map>
#include "../Types.h"

namespace der
{
    typedef unsigned int GameObjectID;
    class GameObject;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        size_t get_object_count() const;
        GameObject* get_object(const size_t index) const;
        const GameObject* get_object_by_id(GameObjectID id) const;
        GameObject* get_object_by_id(GameObjectID id);

        void get_visible_objects(std::vector<GameObject*> &objects) const;

        GameObject* new_object();
        void delete_object(GameObjectID id);

        void delete_all();

    private:
        std::vector<GameObject*> m_gameobjects;
        std::unordered_map<GameObjectID, GameObject*> m_gameobject_map;

        GameObjectID m_next_id;

    };

} // der

#endif // H_DER_SCENE_H
