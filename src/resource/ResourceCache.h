
#ifndef H_DER_RESOURCECACHE_H
#define H_DER_RESOURCECACHE_H

#include <string>
#include <unordered_map>

#include "MeshCache.h"
#include "TextureCache.h"

#include "../renderer/Mesh.h"
#include "../renderer/Texture.h"


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

    private:
//        struct EnumHash
//        {
//            template <typename T>
//            inline typename std::enable_if<std::is_enum<T>::value, std::size_t>::type operator ()(T const value) const
//            {
//                return static_cast<std::size_t>(value);
//            }
//        };
//
//        std::unordered_map<ResourceType, std::string, EnumHash> m_asset_directories;

//        std::unordered_map<std::string, Resource> m_resources;
        //std::unordered_map<std::string, ResourceID> m_resource_names;

        template <class T>
        friend struct TypeToGet;

        MeshCache    m_mesh_cache;
        TextureCache m_texture_cache;
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


    template <typename T>
    T* ResourceCache::get(ResourceID id)
    {
        return TypeToGet<T>::get(this, id);
//        switch (TypeToRT<T>::type)
//        {
//        case ResourceType::Model:
//            return m_mesh_cache.get(id);
//        case ResourceType::Texture:
//            return m_texture_cache.get(id);
//        }
        return nullptr;
    }

} // der

#endif // H_DER_RESOURCECACHE_H
