
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

        size_t get_gameobject_count() const;
        GameObject* get_gameobject(const size_t index) const;

        GameObject* new_gameobject();

    private:
        std::vector<GameObject*> m_gameobjects;

        GameObjectID m_next_id;

    };

} // der

#endif // H_DER_SCENE_H
