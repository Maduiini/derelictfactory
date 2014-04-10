
#include "DirectoryFiles.h"
#include "../Log.h"

#include <cstring>
#include <dirent.h>
#include <sys/stat.h>

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
                struct ::_stat s;
                if (::_stat(filepath.c_str(), &s) != 0)
                    continue;

                if (std::strcmp(entry->d_name, "..") != 0
                    && std::strcmp(entry->d_name, ".") != 0
                    && !S_ISDIR(s.st_mode))
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
