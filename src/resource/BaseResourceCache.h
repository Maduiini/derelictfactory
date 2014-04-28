
#ifndef H_DER_BASE_RESOURCE_CACHE_H
#define H_DER_BASE_RESOURCE_CACHE_H

#include "Resource.h"
#include "DirectoryFiles.h"
#include "../Log.h"

#include <string>
#include <unordered_map>

namespace der
{

    template <class RT>
    class BaseResourceCache
    {
    protected:
        struct Resource
        {
            std::string filepath;
            RT *        resource;
        };
    public:
        explicit BaseResourceCache(const char * const dir);
        ~BaseResourceCache();

        RT* get(ResourceID id);

    protected:
        void scan_resources(const char * const dir);
        virtual RT* load(const char * const filepath) = 0;

    protected:
        typedef std::unordered_map<ResourceID, Resource> ResourceMap;
        ResourceMap m_resources;

        std::string m_resource_dir;
    };


    template <class RT>
    BaseResourceCache<RT>::BaseResourceCache(const char * const dir)
        : m_resource_dir(dir)
    {
        // Scan only if resource directory is non-empty
        if (*dir != 0)
            scan_resources(dir);
    };

    template <class RT>
    BaseResourceCache<RT>::~BaseResourceCache()
    {
        auto it = m_resources.begin();
        for (; it != m_resources.end(); ++it)
        {
            Resource &res = it->second;
            delete res.resource;
        }
    }

    template <class RT>
    RT* BaseResourceCache<RT>::get(ResourceID id)
    {
        auto it = m_resources.find(id);
        if (it != m_resources.end())
        {
            Resource &res = it->second;
            if (!res.resource)
            {
                res.resource = load(res.filepath.c_str());
                if (res.resource)
                    log::info("Resource loaded: %", res.filepath.c_str());
            }
            return res.resource;
        }
        return nullptr;
    }

    template <class RT>
    void BaseResourceCache<RT>::scan_resources(const char * const dir)
    {
        std::vector<std::string> files;
        get_directory_files(dir, files);

        std::string directory = dir;
        for (const std::string &file : files)
        {
            Resource res;
            res.filepath = dir + file;
            res.resource = nullptr;
            ResourceID id = make_resource(file.c_str());
            m_resources[id] = res;
        }
    }

} // der

#endif // H_DER_BASE_RESOURCE_CACHE_H

