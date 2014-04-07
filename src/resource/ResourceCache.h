
#ifndef H_DER_RESOURCECACHE_H
#define H_DER_RESOURCECACHE_H

#include <string>
#include <unordered_map>

#include "../Types.h"

typedef size_t ResourceID;

namespace der
{

    class ResourceCache
    {
    public:
        ResourceCache();
        virtual ~ResourceCache();

        void set_model_directory(const char * const directory)
        { m_model_directory = directory; }

        void scan_directories();

        void load_resources();

    private:
        const char* m_model_directory = nullptr;

        std::unordered_map<std::string, std::string> m_file_names;
        std::unordered_map<std::string, ResourceID> m_resource_names;

    };

} // der

#endif // H_DER_RESOURCECACHE_H
