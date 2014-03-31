
#include "Log.h"

#include <iostream>

namespace der
{
namespace log
{

    static Level g_level =
    #ifdef DER_DEBUG
        LEVEL_DEBUG;
    #else
        LEVEL_INFO;
    #endif

    void set_level(Level level)
    {
        g_level = level;
    }

    Level get_level()
    {
        return g_level;
    }

    void write(const char c)
    {
        std::cout.write(&c, 1);
    }

    void write(const signed char c)
    {
        std::cout.write(reinterpret_cast<const char*>(&c), 1);
    }

    void write(const unsigned char c)
    {
        std::cout.write(reinterpret_cast<const char*>(&c), 1);
    }

    void write(const char *msg)
    {
        while (*msg) write(*msg++);
    }

    void write(const short value)
    {
        std::cout << value;
    }

    void write(const unsigned short value)
    {
        std::cout << value;
    }

    void write(const int value)
    {
        std::cout << value;
    }

    void write(const unsigned int value)
    {
        std::cout << value;
    }

    void write(const long value)
    {
        std::cout << value;
    }

    void write(const unsigned long value)
    {
        std::cout << value;
    }

    void write(const long long value)
    {
        std::cout << value;
    }

    void write(const unsigned long long value)
    {
        std::cout << value;
    }


    void write(const float value)
    {
        std::cout << value;
    }

    void write(const double value)
    {
        std::cout << value;
    }

    //void write(const long double value)
    //{
    //    std::cout << value;
    //}


} // log
} // der
