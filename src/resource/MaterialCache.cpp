
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

        std::string albedo_name, normal_name;
        in >> albedo_name;
        in >> normal_name;
        material->set_albedo_texture(make_resource(albedo_name.c_str()));
        material->set_normal_texture(make_resource(normal_name.c_str()));

        return material;
    }

} // der
