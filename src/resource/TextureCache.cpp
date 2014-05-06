
#include "TextureCache.h"
#include "../renderer/Texture.h"
#include "../Log.h"
#include "../Debug.h"

#include <fstream>

namespace der
{

    struct TgaHeader
    {
        uint8_t     id_length;
        uint8_t     color_map_type;
        uint8_t     image_type;
        uint8_t     color_map_spec[5];
        uint16_t    x_origin;
        uint16_t    y_origin;
        uint16_t    width;
        uint16_t    height;
        uint8_t     bits_per_pixel;
        uint8_t     image_desc;
    };

    static const uint32_t SIZE_OF_TGA_HEADER
        = sizeof(uint8_t) * 10 + sizeof(uint16_t) * 4;

    static_assert(sizeof(TgaHeader) >= SIZE_OF_TGA_HEADER, "Invalid size of TGA header struct");

    struct TgaFooter
    {
        uint32_t    extension_area_offset;
        uint32_t    devel_dir_offset;
        uint8_t     signature[16];  // 'TRUEVISION-XFILE'
        uint8_t     dot;            // '.'
        uint8_t     terminator;     // 0
    };

    static const uint32_t SIZE_OF_TGA_FOOTER
        = sizeof(uint32_t) * 2 + sizeof(uint8_t) * 18;

    static_assert(sizeof(TgaFooter) >= SIZE_OF_TGA_FOOTER, "Invalid size of TGA footer struct");


    class TgaReader
    {
    public:
        TgaReader();
        ~TgaReader();

        bool load(std::istream &in);
        bool save(std::ostream &out, int w, int h,
                  int bytes_per_pixel, const void *data) const;

        const void *get_data() const { return m_data; }
        int get_width() const { return m_width; }
        int get_height() const { return m_height; }
        int get_channels() const { return m_channels; }
        int get_bytes_per_ch() const { return m_bytes_per_ch; }

    private:
        void alloc_data(uint32_t size);
        void delete_data();

        void read_rle_data(std::istream &in, size_t image_size);

        static bool is_supported_fmt(const TgaHeader &header);

    private:
        char *m_data;
        int m_width;
        int m_height;
        int m_channels;
        int m_bytes_per_ch;
    };


    TgaReader::TgaReader()
        : m_data(0)
        , m_width(0)
        , m_height(0)
        , m_channels(0)
        , m_bytes_per_ch(0)
    { }

    TgaReader::~TgaReader()
    {
        delete_data();
    }

    bool TgaReader::load(std::istream &in)
    {
        delete_data();
        if (!in) return false;

        TgaHeader header;
        in.read(reinterpret_cast<char*>(&header), SIZE_OF_TGA_HEADER);
        if (!in) return false;

        if (!is_supported_fmt(header))
            return false;

        uint32_t skip_len = header.id_length;
        if (skip_len)
        {
            in.seekg(skip_len, std::ios_base::cur);
            if (!in) return false;
        }

        const bool screen_orig_right = (header.image_desc & 0x10) != 0; // fourth bit
        const bool screen_orig_top   = (header.image_desc & 0x20) != 0; // fifth bit

//        Image::Origin origin = screenOriginTop
//            ? (screenOriginRight ? Image::TopRight : Image::TopLeft)
//            : (screenOriginRight ? Image::BottomRight : Image::BottomLeft);
//        image.SetOrigin(origin);

        const uint32_t channels = (header.bits_per_pixel == 16)
                        ? 4 : header.bits_per_pixel / 8;

        m_width = header.width;
        m_height = header.height;
        m_channels = channels;
        m_bytes_per_ch = header.bits_per_pixel / channels / 8;

        const size_t image_size = header.width * header.height * channels * m_bytes_per_ch;
        alloc_data(image_size);

        if (header.image_type == 2)
        {
            in.read(m_data, image_size);
        }
        else if (header.image_type == 10)
        {
            read_rle_data(in, image_size);
        }

        if (m_channels > 2)
        {
            for (int y = 0; y < m_height; y++)
            {
                for (int x = 0; x < m_width; x++)
                {
                    const size_t pos = (y * m_width + x) * m_channels;
                    const char tmp = m_data[pos + 0];
                    m_data[pos + 0] = m_data[pos + 2];
                    m_data[pos + 2] = tmp;
                }
            }
        }

        bool result = false;
        if (!in.fail())
        {
            if (screen_orig_right)
            {
                // flip x
                for (int y = 0; y < m_height; y++)
                {
                    const size_t line = y * m_width;
                    for (int x = 0; x < m_width / 2; x++)
                    {
                        const size_t pos0 = (line + x) * m_channels;
                        const size_t pos1 = (line + m_width - 1 - x) * m_channels;

                        for (uint32_t c = 0; c < channels; c++)
                        {
                            char tmp = m_data[pos0 + c];
                            m_data[pos0 + c] = m_data[pos1 + c];
                            m_data[pos1 + c] = tmp;
                        }
                    }
                }
            }
            if (screen_orig_top)
            {
                // flip y
                for (int y = 0; y < m_height / 2; y++)
                {
                    const size_t line0 = y * m_width;
                    const size_t line1 = (m_height - 1 - y) * m_width;
                    for (int x = 0; x < m_width; x++)
                    {
                        const size_t pos0 = (line0 + x) * m_channels;
                        const size_t pos1 = (line1 + x) * m_channels;

                        for (uint32_t c = 0; c < channels; c++)
                        {
                            char tmp = m_data[pos0 + c];
                            m_data[pos0 + c] = m_data[pos1 + c];
                            m_data[pos1 + c] = tmp;
                        }
                    }
                }
            }
            switch (header.bits_per_pixel)
            {
            case 16:
                // TODO: Translate 16 bit image to supported format
                break;
            case 24:
            case 32:
                result = true;
                break;
            default:
                DER_ASSERT(0);
            }
        }

        return result;
    }

    bool TgaReader::save(std::ostream &out, int w, int h, int bytes_per_pixel, const void *data) const
    {
        if (!out) return false;

        TgaHeader header = { 0 };
        header.id_length         = 0;
        header.color_map_type    = 0;
        header.image_type        = 2;
        header.color_map_spec[0] = 0;
        header.color_map_spec[1] = 0;
        header.color_map_spec[2] = 0;
        header.color_map_spec[3] = 0;
        header.color_map_spec[4] = 0;
        header.x_origin          = 0;
        header.y_origin          = 0;
        header.width             = w;
        header.height            = h;

         // 0: No alpha, 8: 8bit alpha
        uint8_t img_desc_alpha = (m_channels == 4) ? 0x8 : 0x0;
        uint8_t origin = 0x00;

//        switch (image.GetOrigin())
//        {
//        case Image::BottomLeft:     origin = 0x00; break;
//        case Image::TopLeft:        origin = 0x20; break;
//        case Image::BottomRight:    origin = 0x10; break;
//        case Image::TopRight:       origin = 0x30; break;
//        }
        header.bits_per_pixel = m_bytes_per_ch * m_channels / 8;
        header.image_desc = img_desc_alpha | origin;

        out.write(reinterpret_cast<const char*>(&header), SIZE_OF_TGA_HEADER);

        const size_t image_size = bytes_per_pixel * w * h;
        out.write(reinterpret_cast<const char*>(data), image_size);

        return true;
    }

    void TgaReader::alloc_data(uint32_t size)
    {
        m_data = new char[size];
    }

    void TgaReader::delete_data()
    {
        delete[] m_data; m_data = 0;
        m_width = m_height = 0;
        m_channels = 0;
        m_bytes_per_ch = 0;
    }

    void TgaReader::read_rle_data(std::istream &in, size_t image_size)
    {
        const uint32_t pixel_size = m_bytes_per_ch * m_channels;
        uint8_t rle;
        char *data = m_data;
        const char *data_end = m_data + image_size;
        while (data != data_end && in)
        {
            in.read(reinterpret_cast<char*>(&rle), 1);
            if (in.fail()) break;

            if (rle < 128)
            {
                // read rle + 1 raw pixels
                size_t bytes = pixel_size * (rle + 1);
                in.read(data, bytes);
                if (in.fail()) break;

                data += bytes;
            }
            else
            {
                uint8_t color[pixel_size];
                in.read(reinterpret_cast<char*>(color), pixel_size);
                if (in.fail()) break;

                // replicate color to the next rle - 127 pixels
                const size_t pixel_count = rle - 127;
                for (size_t px = 0; px < pixel_count; px++)
                {
                    for (size_t i = 0; i < pixel_size; i++, data++)
                        *data = color[i];
                }
            }
        }
    }

    // static
    bool TgaReader::is_supported_fmt(const TgaHeader &header)
    {
        // Not support for color-map data (indexed colors)
        if (header.color_map_type != 0)
            return false;

        if (!(header.image_type == 2 || // Uncompressed true-color
              header.image_type == 10)) // RLE-compressed true-color
            return false;

        switch (header.bits_per_pixel)
        {
        // case 48: case 64: // For 16 bits per channel custom formats
        case 8: case 16: case 24: case 32:
            break;
        default:
            return false;
        }
        return true;
    }



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
            // Some how the compressed formats look very blocky
            // on Intel integrated GPU.
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
