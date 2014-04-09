
#include "DirectoryFiles.h"
#include "../Log.h"

#include <cstring>
#include <dirent.h>

namespace der
{

    void get_directory_files(const std::string &directory, std::vector<std::string> &files)
    {
        DIR *dir = nullptr;
        struct dirent *drnt = nullptr;

        dir = ::opendir(directory.c_str());
        if (dir)
        {
            while((drnt = readdir(dir)))
            {
                if (std::strcmp(drnt->d_name, "..") != 0
                    && std::strcmp(drnt->d_name, ".") != 0
                    && !(drnt->d_type == DT_DIR))
                {
                    files.push_back(drnt->d_name);
                }
            }
            ::closedir(dir);
        }
        else
        {
            log::warning("Failed to open directory '%'.", directory.c_str());
        }
    }

} // der
