
#include "DirectoryFiles.h"
#include "FileStat.h"
#include "../Log.h"


#include <cstring>
#include <dirent.h>

namespace der
{

    /// Returns a list (std::vector) of files in a directory.
    /// \param directory Search directory.
    static void get_directory_files_impl(const std::string &directory,
                                         const std::string &sub_dir,
                                         std::vector<std::string> &files,
                                         bool recursive /*= false*/)
    {
        DIR *dir = nullptr;
        struct dirent *entry = nullptr;

        dir = ::opendir(directory.c_str());
        if (dir)
        {
            while((entry = readdir(dir)))
            {
                const std::string filepath = directory + entry->d_name;
                const std::string file = sub_dir + entry->d_name;

                if (std::strcmp(entry->d_name, "..") != 0
                    && std::strcmp(entry->d_name, ".") != 0)
                {
                    if (is_directory(filepath.c_str()))
                    {
                        if (recursive)
                            get_directory_files_impl(filepath, file + "/", files, true);
                    }
                    else
                    {
                        files.push_back(file);
                    }
                }
            }
            ::closedir(dir);
        }
        else
        {
            log::warning("Failed to open directory '%'.", directory.c_str());
        }
    }

    void get_directory_files(const std::string &directory, std::vector<std::string> &files,
                             bool recursive /*= false*/)
    {
        get_directory_files_impl(directory, "", files, recursive);
//        DIR *dir = nullptr;
//        struct dirent *entry = nullptr;
//
//        dir = ::opendir(directory.c_str());
//        if (dir)
//        {
//            while((entry = readdir(dir)))
//            {
//                std::string filepath = directory + entry->d_name;
//
//                if (std::strcmp(entry->d_name, "..") != 0
//                    && std::strcmp(entry->d_name, ".") != 0)
//                {
//                    if (is_directory(filepath.c_str()))
//                    {
//                        if (recursive) get_directory_files(filepath, files, true);
//                    }
//                    else
//                    {
//                        files.push_back(entry->d_name);
//                    }
//                }
//            }
//            ::closedir(dir);
//        }
//        else
//        {
//            log::warning("Failed to open directory '%'.", directory.c_str());
//        }
    }

} // der
