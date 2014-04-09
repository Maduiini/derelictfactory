
#include "ResourceCache.h"
#include "DirectoryFiles.h"
#include "../Log.h"

#include <dirent.h>
#include <string.h>
#include <vector>
#include <sstream>

namespace der
{

    ResourceCache::ResourceCache()
    {

    }

    ResourceCache::~ResourceCache()
    {

    }

//    void ResourceCache::set_directory(ResourceType type, const std::string directory)
//    {
//        std::pair<ResourceType, std::string> pair = {type, directory + DELIMETER};
//        m_asset_directories.insert(pair);
//    }

//    void ResourceCache::scan_directories()
//    {
//        m_resources.clear();
//        for (const auto &dir : m_asset_directories)
//        {
//            std::vector<std::string> files;
//            get_directory_files(dir.second, files);
//            for (const std::string &filename : files)
//            {
//                log::debug(filename.c_str());
//                Resource res {dir.first, filename, dir.second + filename};
//                std::pair<std::string, Resource> pair = {filename, res};
//                m_resources.insert(pair);
//            }
//        }
//    }

//    void ResourceCache::load_resources()
//    {
//        // Load models
//    }

} // der
