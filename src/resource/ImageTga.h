
#ifndef H_DER_IMAGE_TGA_H
#define H_DER_IMAGE_TGA_H

#include "../Types.h"
#include <iosfwd>

namespace der
{

    struct TgaHeader;

    class TgaReader
    {
    public:
        TgaReader();
        ~TgaReader();

        bool load(std::istream &in);
        bool save(std::ostream &out, int w, int h, int bytes_per_pixel,
                  int channels, const void *data) const;

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

} // der

#endif // H_DER_IMAGE_TGA_H

