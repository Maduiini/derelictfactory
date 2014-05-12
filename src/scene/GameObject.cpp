
#include "GameObject.h"
#include "Camera.h"
#include "Light.h"
#include "../renderer/MeshRenderer.h"
#include "../renderer/TransformRenderer.h"
#include "../renderer/Mesh.h"

#include "../resource/ResourceCache.h"

#include "../Debug.h"

namespace der
{

    GameObject::GameObject(const GameObjectID id)
        : m_id(id)
        , m_position(Vector3::zero)
        , m_rotation(Quaternion::identity)
        , m_scale(1.0f)
        , m_parent(nullptr)
        , m_children()
        , m_mesh_renderer(nullptr)
        , m_tr_renderer(nullptr)
        , m_camera(nullptr)
        , m_light(nullptr)
        , m_bounding_box()
    {
        m_tr_renderer = new TransformRenderer();
    }

    GameObject::~GameObject()
    {
        for (GameObject *child : m_children)
        {
            delete child;
        }

        delete m_camera;
        delete m_mesh_renderer;
    }

    bool GameObject::operator == (const GameObject &obj) const
    {
        return m_id == obj.m_id;
    }


    GameObject* GameObject::get_parent() const
    {
        return m_parent;
    }

    const GameObjectID GameObject::getID() const
    {
        return m_id;
    }

    void GameObject::add_child(GameObject *obj)
    {
        // TODO: maybe only assert that obj != nullptr
        // making it invalid to call add with null object.
        // Same with remove_child.
        if (obj && obj->m_parent != this)
        {
            if (obj->m_parent)
                obj->m_parent->remove_child(obj);

            obj->m_parent = this;
            m_children.push_back(obj);
        }
    }

    void GameObject::remove_child(GameObject *obj)
    {
        if (obj && obj->m_parent == this)
        {
            auto iter = m_children.begin();
            for (; iter != m_children.end(); ++iter)
            {
                if (*iter == obj)
                {
                    m_children.erase(iter);
                    return;
                }
            }
        }
    }

    GameObject* GameObject::get_child(const size_t index) const
    {
        DER_ASSERT(index >= 0 && index < m_children.size());
        return m_children[index];
    }

    size_t GameObject::get_child_count() const
    {
        return m_children.size();
    }

    // Components

    void GameObject::set_renderer(MeshRenderer *renderer)
    {
        delete m_mesh_renderer;
        m_mesh_renderer = renderer;
    }

    MeshRenderer* GameObject::get_renderer()
    { return m_mesh_renderer; }

    TransformRenderer* GameObject::get_tr_renderer()
    { return m_tr_renderer; }

    void GameObject::set_camera(Camera *camera)
    {
        delete m_camera;
        m_camera = camera;
    }

    Camera* GameObject::get_camera()
    { return m_camera; }

    void GameObject::set_light(Light *light)
    { m_light = light; }

    Light* GameObject::get_light()
    { return m_light; }

    // Transform

    void GameObject::set_position(const Vector3 &position)
    { m_position = position; }

    Vector3 GameObject::get_position() const
    { return m_position; }

    void GameObject::move(const Vector3 &delta)
    { m_position += delta; }

    void GameObject::set_rotation(const Quaternion &rot)
    { m_rotation = rot; }

    void GameObject::set_rotation(const Vector3 &axis, float theta)
    { m_rotation.from_axis_angle(axis, theta); }

    Quaternion GameObject::get_rotation() const
    { return m_rotation; }

    void GameObject::rotate(const Vector3 &axis, float theta)
    {
        Quaternion rot;
        rot.from_axis_angle(axis, theta);
        m_rotation = rot * m_rotation;
    }

    void GameObject::set_scale(const Vector3 &scale)
    { m_scale = scale; }

    Vector3 GameObject::get_scale() const
    { return m_scale; }

    Matrix3 GameObject::get_rotation_matrix() const
    {
        Matrix3 rot;
        rot.from_quaternion(m_rotation);
        return rot;
    }

    Matrix4 GameObject::get_world_matrix() const
    {
        Matrix4 translation;
        translation.translation(m_position);
        Matrix4 rotation;
        rotation.from_quaternion(m_rotation);
        Matrix4 scale;
        scale.scale(m_scale);
        return translation * rotation * scale;
    }

    Matrix4 GameObject::get_inv_world_matrix() const
    {
        Matrix4 translation;
        translation.translation(-m_position);
        Matrix4 rotation;
        rotation.from_quaternion(m_rotation.conjugated());
        Matrix4 scale;
        scale.scale(1.0f / m_scale.x, 1.0f / m_scale.y, 1.0f / m_scale.z);
        return scale * rotation * translation;
    }

    void GameObject::set_transform(const Matrix4 &tr)
    { tr.decompose(m_position, m_rotation, m_scale); }

    void GameObject::set_transform(const Matrix3x4 &tr)
    { tr.decompose(m_position, m_rotation, m_scale); }


    void GameObject::update_bounding_box(ResourceCache *cache)
    {
        m_bounding_box.reset();
        if (m_mesh_renderer)
        {
            Mesh *mesh = cache->get<Mesh>(m_mesh_renderer->get_mesh());
            if (mesh)
                m_bounding_box += mesh->get_bounding_box();
        }
        if (m_light)
            m_bounding_box += m_light->get_bounding_box();

        if (m_bounding_box.is_degenerate())
            m_bounding_box.m_min = m_bounding_box.m_max = Vector3::zero;
    }

    Aabb GameObject::get_bounding_box() const
//    { return m_bounding_box + m_position; }
    {
        Aabb aabb(m_bounding_box);
        aabb.transform(get_world_matrix());
        return aabb;
    }

} // der
