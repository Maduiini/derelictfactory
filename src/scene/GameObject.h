

#ifndef H_DER_GAMEOBJECT
#define H_DER_GAMEOBJECT

#include <vector>

#include "../math/Vector.h"
#include "../math/Quaternion.h"
#include "../Types.h"

namespace der
{
    typedef unsigned int GameObjectID;

    class GameObject;
    class MeshRenderer;

    class GameObject
    {
    public:
        GameObject();
        ~GameObject();

        GameObject* get_parent() const;
        const GameObjectID getID() const;

        void add_child(GameObject *obj);
        void remove_child(GameObject *obj);
        GameObject* get_child(const size_t index) const;
        const size_t get_child_count() const;

        bool operator == (const GameObject &obj) const;

    private:
        static GameObjectID nextID;

        GameObjectID m_id;

        // Transform
        Vector3 m_position;
        Quaternion m_rotation;
        Vector3 m_scale;

        // Hierarchy
        GameObject *m_parent;
        std::vector<GameObject*> m_children;

        // Components
        MeshRenderer *m_mesh_renderer;

    };

} // der

#endif // H_DER_GAMEOBJECT
