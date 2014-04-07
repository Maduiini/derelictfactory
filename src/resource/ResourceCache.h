
#ifndef H_DER_RESOURCECACHE_H
#define H_DER_RESOURCECACHE_H

#include <string>
#include <unordered_map>

#include "../Types.h"

typedef size_t ResourceID;

namespace der
{
    enum class ResourceType
    {
        Models, Textures
    };

    struct Resource
    {
        ResourceType type;
        std::string filename;
        std::string fullpath;
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

    private:
        struct EnumHash
        {
            template <typename T>
            inline typename std::enable_if<std::is_enum<T>::value, std::size_t>::type operator ()(T const value) const
            {
                return static_cast<std::size_t>(value);
            }
        };

        std::unordered_map<ResourceType, std::string, EnumHash> m_asset_directories;

        std::unordered_map<std::string, Resource> m_resources;
        std::unordered_map<std::string, ResourceID> m_resource_names;

    };

} // der

#endif // H_DER_RESOURCECACHE_H
