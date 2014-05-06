
#ifndef H_DER_BASE_RESOURCE_CACHE_H
#define H_DER_BASE_RESOURCE_CACHE_H

#include "Resource.h"
#include "DirectoryFiles.h"
#include "FileStat.h"
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
            time_t      last_modified;
            RT *        resource;
        };
    public:
        explicit BaseResourceCache(const char * const dir);
        ~BaseResourceCache();

        RT* get(ResourceID id);

        void reload_all();
        void refresh_all();

        void scan();

    protected:
        void scan_resources(const char * const dir);
        virtual RT* load(const char * const filepath) = 0;
    private:
        void do_load(Resource &res);

        bool has_supported_extension(const std::string &filepath);
        static std::string get_extension(const std::string &filepath);

    protected:
        typedef std::unordered_map<ResourceID, Resource> ResourceMap;
        ResourceMap m_resources;

        std::string m_resource_dir;
        std::vector<std::string> m_supported_extensions;
    };


    template <class RT>
    BaseResourceCache<RT>::BaseResourceCache(const char * const dir)
        : m_resource_dir(dir)
    { }

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
    void BaseResourceCache<RT>::do_load(Resource &res)
    {
        const char * const filepath = res.filepath.c_str();
        res.resource = load(filepath);
        if (res.resource)
        {
            log::info("Resource loaded: %", filepath);
            res.last_modified = get_modify_time(filepath);
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
                do_load(res);
            }
            return res.resource;
        }
        return nullptr;
    }

    template <class RT>
    void BaseResourceCache<RT>::reload_all()
    {
        scan();
        typename ResourceMap::iterator it = m_resources.begin();
        for (; it != m_resources.end(); ++it)
        {
            Resource &res = it->second;

            delete res.resource;
            do_load(res);
        }
    }

    template <class RT>
    void BaseResourceCache<RT>::refresh_all()
    {
//        scan();
        typename ResourceMap::iterator it = m_resources.begin();
        for (; it != m_resources.end(); ++it)
        {
            Resource &res = it->second;
            time_t last_modified = get_modify_time(res.filepath.c_str());
            if (res.last_modified != last_modified)
            {
                delete res.resource;
                do_load(res);
            }
        }
    }

    template <class RT>
    void BaseResourceCache<RT>::scan()
    {
        scan_resources(m_resource_dir.c_str());
    }

    template <class RT>
    void BaseResourceCache<RT>::scan_resources(const char * const dir)
    {
        std::vector<std::string> files;
        get_directory_files(dir, files);

        std::string directory = dir;
        for (const std::string &file : files)
        {
            if (has_supported_extension(file))
            {
                ResourceID id = make_resource(file.c_str());

                // Do not attempt to re-insert an existing resource. It will overwrite
                // the data that may be in use.
                if (m_resources.find(id) != m_resources.end())
                    continue;

                Resource res;
                res.filepath = dir + file;
                res.resource = nullptr;
                res.last_modified = get_modify_time(res.filepath.c_str());
                m_resources[id] = res;
            }
        }
    }

    template <class RT>
    bool BaseResourceCache<RT>::has_supported_extension(const std::string &filepath)
    {
        const std::string ext = get_extension(filepath);
        for (const std::string &e : m_supported_extensions)
        {
            if (e == ext) return true;
        }
        return false;
    }

    // static
    template <class RT>
    std::string BaseResourceCache<RT>::get_extension(const std::string &filepath)
    {
        const std::string::size_type pos = filepath.rfind('.');
        if (pos != std::string::npos)
        {
            return filepath.substr(pos + 1);
        }
        return "";
    }

} // der

#endif // H_DER_BASE_RESOURCE_CACHE_H

