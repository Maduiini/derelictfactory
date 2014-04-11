

#ifndef H_DER_GAMEOBJECT
#define H_DER_GAMEOBJECT

#include <vector>

#include "../math/Vector.h"
#include "../math/Quaternion.h"

namespace der
{

    class GameObject;
    class MeshRenderer;

    class GameObject
    {
    public:
        GameObject();
        ~GameObject();

    private:
        // GameObject transform
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
