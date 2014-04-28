
#include "SceneLoader.h"
#include "../scene/Scene.h"
#include "../scene/GameObject.h"
#include "../renderer/MeshRenderer.h"

#include "ResourceCache.h"

#include "../math/Math.h"
#include "../Log.h"

#include <fstream>

namespace der
{
namespace scene_loader
{
    enum class ObjectType
    {
        MESH = 0, LIGHT = 1
    };

    struct SceneObject
    {
        unsigned int m_type;
        float m_transform[12];
    };

    constexpr size_t NameLength = 32;

    struct MeshObjectInfo
    {
        char m_mesh_name[NameLength];
        char m_material_name[NameLength];
    };
} // scene_loader


    SceneLoader::SceneLoader(ResourceCache &resource_cache)
        : m_resource_cache(resource_cache)
        , m_resource_dir("assets/scenes/")
    { }

    bool SceneLoader::load(const char *scene_name, Scene *scene)
    {
        const std::string filepath = m_resource_dir + scene_name;
        std::ifstream input;
        input.open(filepath.c_str(), std::ios::binary);

        if (!input.is_open())
        {
            log::error("Scene: Could not open file %", filepath.c_str());
            return false;
        }

        scene->delete_all();

        using namespace scene_loader;
        while (input)
        {
            SceneObject scene_object;
            input.read(reinterpret_cast<char*>(&scene_object), sizeof(SceneObject));
            if (ObjectType(scene_object.m_type) == ObjectType::MESH)
            {
                MeshObjectInfo info;
                input.read(reinterpret_cast<char*>(&info), sizeof(MeshObjectInfo));

                std::string mesh_name = info.m_mesh_name;
                mesh_name += ".obj";
                const ResourceID mesh_id = make_resource(mesh_name.c_str());
                Mesh *mesh = m_resource_cache.get<Mesh>(mesh_id);
                if (!mesh)
                {
                    log::error("Could not load mesh %", mesh_name.c_str());
                    continue;
                }

                GameObject *object = scene->new_object();
                const Matrix3x4 tr(scene_object.m_transform);
                object->set_transform(tr);

                MeshRenderer *renderer = new MeshRenderer();
                renderer->set_mesh(mesh);
                object->set_renderer(renderer);
            }
        }

        return true;
    }


} // der
