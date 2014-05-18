
#include "TextureDisplay.h"

#include "GUIRenderer.h"

#include "../renderer/Texture.h"

namespace der
{

    TextureDisplay::TextureDisplay(Vector2 position, Vector2 size)
        : m_texture(nullptr)
        , m_position(position)
        , m_size(size)
    {
        m_render_cmds.push_back(new RenderTexture(m_position, m_size));
    }

    TextureDisplay::~TextureDisplay()
    { }

    void TextureDisplay::set_texture(Texture *tex)
    {
        m_texture = tex;
        static_cast<RenderTexture*>(m_render_cmds[0])->set_texture(tex);
    }

} // der
