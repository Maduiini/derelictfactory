
#ifndef H_DER_TEXTURE_DISPLAY_H
#define H_DER_TEXTURE_DISPLAY_H

#include "Widget.h"

namespace der
{

    class Texture;

    class TextureDisplay : public Widget
    {
    public:
        TextureDisplay(Vector2 position, Vector2 size);
        virtual ~TextureDisplay();

        void set_texture(Texture *tex);

    private:
        Texture *m_texture;
        Vector2 m_position;
        Vector2 m_size;

    };


} // der

#endif // H_DER_TEXTURE_DISPLAY_H
