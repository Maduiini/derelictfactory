
#ifndef H_DER_MATERIAL_CACHE_H
#define H_DER_MATERIAL_CACHE_H

#include "BaseResourceCache.h"

namespace der
{

    class Material;

    class MaterialCache : public BaseResourceCache<Material>
    {
    public:
        MaterialCache();
    private:
        virtual Material* load(const char * const filepath);
    };

} // der

#endif // H_DER_MATERIAL_CACHE_H

