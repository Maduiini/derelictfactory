
#include "TextureCache.h"
#include "ImageTga.h"
#include "../renderer/Texture.h"
#include "../Log.h"

#include <fstream>

namespace der
{

    TextureCache::TextureCache()
        : BaseResourceCache("assets/textures/")
    {
        m_supported_extensions.push_back("tga");
    }

    Texture2D* TextureCache::load(const char * const filepath)
    {
        std::ifstream in(filepath, std::ios_base::binary);
        if (!in)
        {
            log::error("Could not open file: %", filepath);
            return nullptr;
        }

        TgaReader reader;
        if (!reader.load(in))
        {
            log::error("Could not load texture: %", filepath);
            return nullptr;
        }

        Texture::InputFormat input_fmt = Texture::IN_RGB8;
        Texture::Format fmt = Texture::RGB_DXT1;
        switch (reader.get_channels())
        {
        case 1: input_fmt = Texture::IN_R8;
                fmt = Texture::R_RGTC1;
            break;
        case 2: input_fmt = Texture::IN_RG8;
                fmt = Texture::RG_RGTC2;
            break;
        case 3: input_fmt = Texture::IN_RGB8;
//                fmt = Texture::RGB_DXT1;
                fmt = Texture::RGB8;
            break;
        case 4: input_fmt = Texture::IN_RGBA8;
//                fmt = Texture::RGBA_DXT5;
                fmt = Texture::RGBA8;
            break;
        }

        Texture2D *texture = new Texture2D();
        texture->TexImage(reader.get_width(), reader.get_height(), fmt, input_fmt, reader.get_data());
        return texture;
    }

} // der
