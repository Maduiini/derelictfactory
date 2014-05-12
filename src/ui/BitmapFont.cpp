
#include "BitmapFont.h"

#include "../Debug.h"
#include "../Log.h"

namespace der
{

    BitmapFont::BitmapFont()
        : m_max_characters(0)
        , m_characters(nullptr)
    { }

    BitmapFont::~BitmapFont()
    {
        if (m_characters)
            delete m_characters;
    }

    void BitmapFont::reserve(const size_t characters)
    {
        if (m_characters)
            delete m_characters;

        m_max_characters = characters;
        m_characters = new BitmapFontCharacter[m_max_characters];
        for (size_t i=0; i<m_max_characters; i++)
        {
            m_characters[0].id = InvalidCharacterID;
        }
    }

    void BitmapFont::add_character(BitmapFontCharacter character)
    {
        DER_ASSERT(character.id != InvalidCharacterID);
        m_characters[character.id] = character;
    }

    BitmapFontCharacter BitmapFont::get_character(unsigned int id) const
    {
        DER_ASSERT(id < m_max_characters);
        return m_characters[id];
    }

    unsigned int BitmapFont::get_text_width(const char *text) const
    {
        unsigned int width = 0;
        while (*text)
        {
            char c = *text++;
            if (c == ' ')
            {
                width += get_line_height() * 0.25f;
            }
            else
            {
                BitmapFontCharacter glyph = get_character(c);
                width += glyph.advance_x;
            }
        }
        return width;
    }

} // der
