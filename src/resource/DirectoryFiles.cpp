
#include "DirectoryFiles.h"
#include "FileStat.h"
#include "../Log.h"


#include <cstring>
#include <dirent.h>

namespace der
{

    void get_directory_files(const std::string &directory, std::vector<std::string> &files)
    {
        DIR *dir = nullptr;
        struct dirent *entry = nullptr;

        dir = ::opendir(directory.c_str());
        if (dir)
        {
            while((entry = readdir(dir)))
            {
                std::string filepath = directory + entry->d_name;

                if (std::strcmp(entry->d_name, "..") != 0
                    && std::strcmp(entry->d_name, ".") != 0
                    && !is_directory(filepath.c_str()))
                {
                    files.push_back(entry->d_name);
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
