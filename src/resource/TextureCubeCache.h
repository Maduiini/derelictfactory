
#ifndef H_DER_TEXTURE_CUBE_CACHE_H
#define H_DER_TEXTURE_CUBE_CACHE_H

#include "BaseResourceCache.h"
#include "../renderer/TextureCube.h"

namespace der
{

    class TextureCubeCache : public BaseResourceCache<TextureCube>
    {
    public:
        TextureCubeCache();
    private:
        virtual TextureCube* load(const char * const filepath);
        bool load_side(TextureCube *texture, TextureCube::Side side, const std::string &filepath);
    };

} // der

#endif // H_DER_TEXTURE_CUBE_CACHE_H

