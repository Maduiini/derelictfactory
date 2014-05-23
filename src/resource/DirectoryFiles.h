
#ifndef H_DER_DIRECTORY_FILES_H
#define H_DER_DIRECTORY_FILES_H

#include <string>
#include <vector>

namespace der
{

    /// Returns a list (std::vector) of files in a directory.
    /// \param directory Search directory.
    void get_directory_files(const std::string &directory, std::vector<std::string> &files,
                             bool recursive = false);

} // der

#endif // H_DER_DIRECTORY_FILES_H

