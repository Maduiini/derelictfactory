
#include "TextureCubeCache.h"
#include "ImageTga.h"
#include "../renderer/TextureCube.h"

#include <fstream>

namespace der
{

    TextureCubeCache::TextureCubeCache()
        : BaseResourceCache("assets/textures/")
    {
        m_supported_extensions.push_back("cube");
    }

    TextureCube* TextureCubeCache::load(const char * const filepath)
    {
        std::ifstream in(filepath);
        if (!in)
        {
            log::error("Could not open file: %", filepath);
            return nullptr;
        }

        std::string path_neg_x, path_pos_x;
        std::string path_neg_y, path_pos_y;
        std::string path_neg_z, path_pos_z;
        in >> path_neg_x;
        in >> path_pos_x;
        in >> path_neg_y;
        in >> path_pos_y;
        in >> path_neg_z;
        in >> path_pos_z;

        TextureCube *texture = new TextureCube();
        if (!load_side(texture, TextureCube::Negative_X, path_neg_x.c_str()))
            return texture;
        if (!load_side(texture, TextureCube::Positive_X, path_pos_x.c_str()))
            return texture;
        if (!load_side(texture, TextureCube::Negative_Y, path_neg_y.c_str()))
            return texture;
        if (!load_side(texture, TextureCube::Positive_Y, path_pos_y.c_str()))
            return texture;
        if (!load_side(texture, TextureCube::Negative_Z, path_neg_z.c_str()))
            return texture;
        if (!load_side(texture, TextureCube::Positive_Z, path_pos_z.c_str()))
            return texture;
        return texture;
    }

    bool TextureCubeCache::load_side(TextureCube *texture, TextureCube::Side side, const std::string &filepath)
    {
        std::ifstream in(m_resource_dir + filepath, std::ios_base::binary);
        if (!in)
        {
            log::error("Could not open file: %", filepath);
            return false;
        }

        TgaReader reader;
        if (!reader.load(in))
        {
            log::error("Could not load cube texture: %", filepath);
            return false;
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

        texture->TexImage(side, reader.get_width(), reader.get_height(), fmt, input_fmt, reader.get_data());
        return true;
    }

} // der
