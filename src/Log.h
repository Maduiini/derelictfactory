
#ifndef H_DER_LOG_H
#define H_DER_LOG_H

#include <functional>
#include <string>

namespace der
{

    struct Vector2;
    struct Vector3;
    struct Vector4;

    struct Matrix3;
    struct Matrix4;
    struct Matrix3x4;

    struct Quaternion;

namespace log
{

    enum Level
    {
        LEVEL_TRACE,
        LEVEL_DEBUG,
        LEVEL_INFO,
        LEVEL_WARNING,
        LEVEL_ERROR,
        LEVEL_FATAL
    };

    void set_level(Level level);
    Level get_level();

    void write(const bool b);

    void write(const char c);
    void write(const signed char c);
    void write(const unsigned char c);
    void write(const char *msg);
    void write(const std::string &msg);

    void write(const short value);
    void write(const unsigned short value);
    void write(const int value);
    void write(const unsigned int value);
    void write(const long value);
    void write(const unsigned long value);
    void write(const long long value);
    void write(const unsigned long long value);

    void write(const float value);
    void write(const double value);

    void write(const Vector2 &);
    void write(const Vector3 &);
    void write(const Vector4 &);
    void write(const Matrix3 &);
    void write(const Matrix4 &);
    void write(const Matrix3x4 &);
    void write(const Quaternion &);


    template <class... Args>
    void trace(const char *msg, Args&& ...args);

    template <class... Args>
    void debug(const char *msg, Args&& ...args);

    template <class... Args>
    void info(const char *msg, Args&& ...args);

    template <class... Args>
    void warning(const char *msg, Args&& ...args);

    template <class... Args>
    void error(const char *msg, Args&& ...args);

    template <class... Args>
    void fatal(const char *msg, Args&& ...args);

    namespace internal
    {
        inline void write_impl(const char *msg)
        {
            while (*msg)
            {
                if (*msg == '%')
                {
                    msg++;
                    if (*msg != '%')
                    {
                        // Error: not enough parameters to print.
                        break;
                    }
                }
                write(*msg);
                msg++;
            }
            write('\n');
        }

        template <class T, class... Args>
        void write_impl(const char *msg, T&& v, Args&& ...args)
        {
            while (*msg)
            {
                if (*msg == '%')
                {
                    msg++;
                    if (*msg != '%')
                    {
                        write(std::forward<T&&>(v));
                        write_impl(msg, std::forward<Args&&>(args)...);
                        return;
                    }
                }
                write(*msg);
                msg++;
            }
            write('\n');
            // Error: extranous parameters
        }

        template <Level LVL, class... Args>
        void write_level_stub(const char * const level_str,
                              const char *msg, Args&& ...args)
        {
            if (get_level() > LVL) return;
            write(level_str);
            write_impl(msg, std::forward<Args&&>(args)...);
        }
    } // internal


    template <class... Args>
    void trace(const char *msg, Args&& ...args)
    {
        internal::write_level_stub<LEVEL_TRACE>("trace: ", msg, std::forward<Args&&>(args)...);
    }

    template <class... Args>
    void debug(const char *msg, Args&& ...args)
    {
        internal::write_level_stub<LEVEL_DEBUG>("debug: ", msg, std::forward<Args&&>(args)...);
    }

    template <class... Args>
    void info(const char *msg, Args&& ...args)
    {
        internal::write_level_stub<LEVEL_INFO>("info: ", msg, std::forward<Args&&>(args)...);
    }

    template <class... Args>
    void warning(const char *msg, Args&& ...args)
    {
        internal::write_level_stub<LEVEL_WARNING>("warning: ", msg, std::forward<Args&&>(args)...);
    }

    template <class... Args>
    void error(const char *msg, Args&& ...args)
    {
        internal::write_level_stub<LEVEL_ERROR>("error: ", msg, std::forward<Args&&>(args)...);
    }

    template <class... Args>
    void fatal(const char *msg, Args&& ...args)
    {
        internal::write_level_stub<LEVEL_FATAL>("fatal: ", msg, std::forward<Args&&>(args)...);
    }

} // log
} // der

#endif // H_DER_LOG_H

