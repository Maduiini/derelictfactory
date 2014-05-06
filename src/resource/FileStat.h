
#ifndef H_DER_FILE_STAT_H
#define H_DER_FILE_STAT_H

#include <ctime>

namespace der
{

    bool is_directory(const char * const filepath);
    time_t get_modify_time(const char * const filepath);

} // der

#endif // H_DER_FILE_STAT_H

