
#ifndef H_DER_TEXTURE_CACHE_H
#define H_DER_TEXTURE_CACHE_H

#include "BaseResourceCache.h"

namespace der
{

    class Texture2D;

    class TextureCache : public BaseResourceCache<Texture2D>
    {
    public:
        TextureCache();
    private:
        virtual Texture2D* load(const char * const filepath);
    };

} // der

#endif // H_DER_TEXTURE_CACHE_H

