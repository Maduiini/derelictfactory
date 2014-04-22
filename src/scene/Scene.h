
#ifndef H_DER_SCENE_H
#define H_DER_SCENE_H

#include <vector>
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

        void get_visible_objects(std::vector<GameObject*> &objects) const;

        GameObject* new_object();
        void delete_object(GameObjectID id);

        void delete_all();

    private:
        std::vector<GameObject*> m_gameobjects;

        GameObjectID m_next_id;

    };

} // der

#endif // H_DER_SCENE_H
