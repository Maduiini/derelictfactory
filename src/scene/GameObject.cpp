
#include "GameObject.h"

namespace der
{

    GameObject::GameObject()
        : m_position(Vector3::zero)
        , m_rotation(Quaternion::identity)
        , m_scale(1.0f)
        , m_parent(nullptr)
    { }

    GameObject::~GameObject()
    { }

} // der
