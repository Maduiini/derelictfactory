
#include "MaterialCache.h"

#include "../renderer/Material.h"

#include <fstream>

namespace der
{

    MaterialCache::MaterialCache()
        : BaseResourceCache("assets/materials/")
    {
        m_supported_extensions.push_back("material");
    }

    Material* MaterialCache::load(const char * const filepath, InputFileList &dependencies)
    {
        std::ifstream in(filepath);
        if (!in)
        {
            log::error("Could not open file: %", filepath);
            return nullptr;
        }

        Material *material = new Material();

        std::string vert_name, frag_name;
        in >> vert_name;
        in >> frag_name;
        material->set_shaders(make_resource(vert_name.c_str()), make_resource(frag_name.c_str()));

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

        std::string cull_mode_str;
        in >> cull_mode_str;

        if (cull_mode_str == "cull_none")
            material->set_cull_mode(CullMode::None);

        return material;
    }

} // der
