

#ifndef H_DER_GAMEOBJECT
#define H_DER_GAMEOBJECT

#include <vector>

#include "../math/Vector.h"
#include "../math/Quaternion.h"
#include "../math/Matrix3.h"
#include "../math/Matrix4.h"
#include "../Types.h"

namespace der
{
    typedef unsigned int GameObjectID;

    class GameObject;
    class MeshRenderer;
    class Camera;
    class Scene;

    class GameObject
    {
        friend class Scene;

    public:
        ~GameObject();

        bool operator == (const GameObject &obj) const;


        GameObject* get_parent() const;
        const GameObjectID getID() const;

        void add_child(GameObject *obj);
        void remove_child(GameObject *obj);
        GameObject* get_child(const size_t index) const;
        size_t get_child_count() const;


        void set_renderer(MeshRenderer *renderer);
        MeshRenderer* get_renderer();

        void set_camera(Camera *camera);
        Camera* get_camera();


        void set_position(const Vector3 &position);
        Vector3 get_position() const;
        void move(const Vector3 &delta);

        void set_rotation(const Quaternion &rot);
        void set_rotation(const Vector3 &axis, float theta);
        Quaternion get_rotation() const;

        void set_scale(const Vector3 &scale);
        Vector3 get_scale() const;

        Matrix3 get_rotation_matrix() const;
        Matrix4 get_world_matrix() const;
        Matrix4 get_inv_world_matrix() const;

    private:
        GameObject(const GameObjectID id);

        const GameObjectID m_id;

        // Transform
        Vector3 m_position;
        Quaternion m_rotation;
        Vector3 m_scale;

        // Hierarchy
        GameObject *m_parent;
        std::vector<GameObject*> m_children;

        // Components
        Camera *m_camera;
        MeshRenderer *m_mesh_renderer;

    };

} // der

#endif // H_DER_GAMEOBJECT
