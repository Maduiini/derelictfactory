
#ifndef H_DER_RESOURCECACHE_H
#define H_DER_RESOURCECACHE_H

#include <string>
#include <unordered_map>

#include "MeshCache.h"
#include "TextureCache.h"
#include "ShaderCache.h"
#include "MaterialCache.h"

#include "../renderer/Mesh.h"
#include "../renderer/Texture.h"
#include "../renderer/Shader.h"
#include "../renderer/Material.h"


namespace der
{

    class Mesh;

    enum class ResourceType
    {
        Model, Texture
    };

    template <class RT>
    struct TypeToRT;

    template <>
    struct TypeToRT<Mesh>
    {
        static constexpr ResourceType type = ResourceType::Model;
    };

    template <>
    struct TypeToRT<Texture2D>
    {
        static constexpr ResourceType type = ResourceType::Texture;
    };


    class ResourceCache
    {
    public:
        static const char DELIMETER = '\\';

        ResourceCache();
        virtual ~ResourceCache();

        void set_directory(ResourceType type, const std::string directory);

        void scan_directories();

        void load_resources();

        template <typename T>
        T* get(ResourceID id);

        Program* get_program(ResourceID v, ResourceID f);

    private:
        template <class T>
        friend struct TypeToGet;

        MeshCache       m_mesh_cache;
        TextureCache    m_texture_cache;
        ShaderCache     m_shader_cache;
        ProgramCache    m_program_cache;
        MaterialCache   m_material_cache;
    };


    template <class T>
    struct TypeToGet;

    template <>
    struct TypeToGet<Mesh>
    {
        static Mesh* get(ResourceCache *cache, ResourceID id)
        { return cache->m_mesh_cache.get(id); }
    };

    template <>
    struct TypeToGet<Texture2D>
    {
        static Texture2D* get(ResourceCache *cache, ResourceID id)
        { return cache->m_texture_cache.get(id); }
    };

    template <>
    struct TypeToGet<Shader>
    {
        static Shader* get(ResourceCache *cache, ResourceID id)
        { return cache->m_shader_cache.get(id); }
    };

    template <>
    struct TypeToGet<Material>
    {
        static Material* get(ResourceCache *cache, ResourceID id)
        { return cache->m_material_cache.get(id); }
    };


    template <typename T>
    T* ResourceCache::get(ResourceID id)
    {
        return TypeToGet<T>::get(this, id);
    }

} // der

#endif // H_DER_RESOURCECACHE_H
