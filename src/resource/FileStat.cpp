
#include "FileStat.h"

#include <sys/stat.h>

namespace der
{

    bool is_directory(const char * const filepath)
    {
        struct ::_stat s;
        if (::_stat(filepath, &s) == 0)
        {
            return S_ISDIR(s.st_mode);
        }
        return false;
    }

    time_t get_modify_time(const char * const filepath)
    {
        struct ::_stat s;
        if (::_stat(filepath, &s) == 0)
        {
            return s.st_mtime;
        }
        return time_t(0);
    }

} // der
