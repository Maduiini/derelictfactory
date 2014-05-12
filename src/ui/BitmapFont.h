
#ifndef H_DER_BITMAP_FONT_H
#define H_DER_BITMAP_FONT_H

#include "../Types.h"

namespace der
{

    class BitmapFontLoader;

    constexpr unsigned int InvalidCharacterID = -1;

    struct BitmapFontCharacter
    {
        unsigned int id;
        unsigned int x;
        unsigned int y;
        unsigned int width;
        unsigned int height;
        int offset_x;
        int offset_y;
        unsigned int advance_x;
    };

    class BitmapFont
    {
        friend class BitmapFontLoader;

    public:
        BitmapFont();
        ~BitmapFont();

        void reserve(const size_t characters);
        void add_character(BitmapFontCharacter character);

        BitmapFontCharacter get_character(unsigned int id) const;

        unsigned int get_text_width(const char * const text) const;

        unsigned int get_line_height() const
        { return m_line_height; }

        unsigned int get_base() const
        { return m_base; }

    private:
        size_t m_max_characters;
        BitmapFontCharacter *m_characters;

        unsigned int m_line_height;
        unsigned int m_base;

    };

} // der

#endif // H_DER_BITMAP_FONT_H
