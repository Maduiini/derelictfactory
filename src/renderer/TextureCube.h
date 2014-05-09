
#ifndef H_DER_TEXTURE_CUBE_H
#define H_DER_TEXTURE_CUBE_H

#include "../Types.h"
#include "Texture.h"

namespace der
{

    class TextureCube : public Texture
    {
    public:
        enum Side
        {
            Negative_X, Positive_X,
            Negative_Y, Positive_Y,
            Negative_Z, Positive_Z
        };
    public:
        TextureCube();

        bool is_complete() const;

        void TexImage(Side side, int w, int h, Format fmt, InputFormat input_fmt, const void *data);

    private:
        void set_format(int w, int h, Format fmt);

    private:
        size_t m_sides_present;
        int m_width, m_height;
        Format m_format;
    };

} // der

#endif // H_DER_TEXTURE_CUBE_H

