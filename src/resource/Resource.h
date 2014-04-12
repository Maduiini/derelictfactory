
#ifndef H_DER_RESOURCE_H
#define H_DER_RESOURCE_H

#include "../Types.h"

namespace der
{

    typedef size_t ResourceID;

    constexpr uint32_t rotate(uint32_t value, uint32_t rot)
    {
        return (value << rot) + (value >> (32 - rot));
    }

    constexpr uint32_t make_resource_r(const char * const str, uint32_t h)
    {
        return (!*str) ? h : make_resource_r(str + 1, *str + h + rotate(h, 5));
    }

    constexpr ResourceID make_resource(const char * const name)
    {
        return static_cast<ResourceID>(make_resource_r(name, 0x1234));
    }

} // der

#endif // H_DER_RESOURCE_H

