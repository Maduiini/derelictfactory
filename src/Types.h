
#ifndef H_DER_TYPES_H
#define H_DER_TYPES_H

#include <cinttypes>
#include <sstream>

namespace der
{

    typedef unsigned int size_t;
    typedef uint32_t ResourceID;


    // NOTE: The c++11 value to string conversion functionality
    // std::to_string does not work in MinGW (at least <4.8.1).
    template<typename T>
    std::string to_string(T value)
    {
        std::ostringstream string_stream;
        string_stream << value;
        return string_stream.str();
    }

} // der

#endif // H_DER_TYPES_H

