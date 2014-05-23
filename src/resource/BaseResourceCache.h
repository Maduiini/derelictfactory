
#ifndef H_DER_BASE_RESOURCE_CACHE_H
#define H_DER_BASE_RESOURCE_CACHE_H

#include "Resource.h"
#include "DirectoryFiles.h"
#include "FileStat.h"
#include "../Log.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace der
{

    template <class RT>
    class BaseResourceCache
    {
    protected:
        struct InputFile
        {
            std::string filepath;
            time_t      last_modified;
        };

        typedef std::vector<InputFile> InputFileList;

        struct Resource
        {
            InputFile       file;
            InputFileList   dependencies;
            RT *            resource;
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
        virtual RT* load(const char * const filepath, InputFileList &dependencies) = 0;
    private:
        void do_load(Resource &res);

        static bool needs_refresh(const InputFile &file);
        static bool needs_refresh(const Resource &res);

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
        const char * const filepath = res.file.filepath.c_str();
        res.dependencies.clear();
        res.resource = load(filepath, res.dependencies);
        if (res.resource)
        {
            log::info("Resource loaded: %", filepath);
            res.file.last_modified = get_modify_time(filepath);
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

    // static
    template <class RT>
    bool BaseResourceCache<RT>::needs_refresh(const InputFile &file)
    {
        time_t last_modified = get_modify_time(file.filepath.c_str());
        return (file.last_modified != last_modified);
    }

    // static
    template <class RT>
    bool BaseResourceCache<RT>::needs_refresh(const Resource &res)
    {
        if (needs_refresh(res.file)) return true;
        const size_t dep_count = res.dependencies.size();
        for (size_t i = 0; i < dep_count; i++)
        {
            const InputFile &input_file = res.dependencies[i];
            if (needs_refresh(input_file)) return true;
        }
        return false;
    }

    template <class RT>
    void BaseResourceCache<RT>::refresh_all()
    {
        scan();
        typename ResourceMap::iterator it = m_resources.begin();
        for (; it != m_resources.end(); ++it)
        {
            Resource &res = it->second;
            if (needs_refresh(res))
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
        get_directory_files(dir, files, true);

//        std::string directory = dir;
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
                res.file.filepath = dir + file;
                res.file.last_modified = get_modify_time(res.file.filepath.c_str());
                res.resource = nullptr;
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

