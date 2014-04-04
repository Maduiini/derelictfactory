
#include "ResourceCache.h"
#include "../Log.h"
#include <dirent.h>
#include <vector>

namespace der
{

    /// Returns true if \c filename is a directory.
    /// \note Does not work if file does not contain dot character.
    /// \param filename Null-terminated string.
    static bool is_directory(const char * const filename)
    {
        if (filename == nullptr)
            return false;

        int i = 0;
        while (filename[i] != '\0')
        {
            if (filename[i] == '.')
                return false;
            i++;
        }
        return true;
    }

    /// Returns a list (std::vector) of files in a directory.
    /// \param directory Search directory.
    static std::vector<std::string> get_directory_files(std::string directory)
    {
        DIR *dir = NULL;
        struct dirent *drnt = NULL;
        std::vector<std::string> files;

        dir = opendir(directory.c_str());
        if (dir)
        {
            while((drnt = readdir(dir)))
            {
                if (strcmp(drnt->d_name, "..") != 0 && strcmp(drnt->d_name, ".") != 0 &&
                    !is_directory(drnt->d_name))
                    files.push_back(drnt->d_name);
            }
            closedir(dir);
        }
        else
        {
            log::warning("Failed to open directory '%'.", directory.c_str());
        }

        return files;
    }

    ResourceCache::ResourceCache()
    {

    }

    ResourceCache::~ResourceCache()
    {

    }

    void ResourceCache::scan_directories()
    {
        for (const auto& dir : get_directory_files("."))
        {
            log::info(dir.c_str());
        }
    }

} // der
