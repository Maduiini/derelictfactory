
#include "MaterialCache.h"

#include "../renderer/Material.h"

#include <fstream>

namespace der
{

    MaterialCache::MaterialCache()
        : BaseResourceCache("assets/materials/")
    { }

    Material* MaterialCache::load(const char * const filepath)
    {
        std::ifstream in(filepath);
        if (!in)
        {
            log::error("Could not open file: %", filepath);
            return nullptr;
        }

        Material *material = new Material();

        std::string albedo_name, normal_name, roughness_name, metallic_name, env_name;
        in >> albedo_name;
        in >> normal_name;
        in >> roughness_name;
        in >> metallic_name;
        in >> env_name;
        material->set_albedo_texture(make_resource(albedo_name.c_str()));
        material->set_normal_texture(make_resource(normal_name.c_str()));
        material->set_roughness_texture(make_resource(roughness_name.c_str()));
        material->set_metallic_texture(make_resource(metallic_name.c_str()));
        material->set_env_texture(make_resource(env_name.c_str()));

        return material;
    }

} // der
