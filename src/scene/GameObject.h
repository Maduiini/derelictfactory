

#ifndef H_DER_GAMEOBJECT
#define H_DER_GAMEOBJECT

#include "../math/Vector.h"
#include "../math/Quaternion.h"

namespace der
{
    struct Vector3;
    struct Quaternion;
    class GameObject;

    class GameObject
    {
    public:
        GameObject();
        ~GameObject();

    private:
        Vector3 m_position;
        Quaternion m_rotation;
        Vector3 m_scale;

        GameObject *m_parent;

    };

} // der

#endif // H_DER_GAMEOBJECT
