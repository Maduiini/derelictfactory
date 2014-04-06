
#include "Debug.h"
#include "Log.h"

namespace der
{

    void assert(const char * const msg, const char * const file, int line)
    {
        const char * file_only = file;
        const char * p = file;
        while (*p != '\0')
        {
            if (*p == '\\')
                file_only = ++p;
            else
                ++p;
        }
        log::debug("Assert failure: % in [%:%]", msg, file_only, line);
    }

} // der
