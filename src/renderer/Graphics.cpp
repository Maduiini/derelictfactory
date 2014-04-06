
#include "Graphics.h"
#include "Texture.h"

#include <GL/glew.h>

namespace der
{

    Graphics::Graphics()
    {
        m_current.m_cull_mode = CULL_BackFace;
        for (TextureUnit &tu : m_current.m_texture_units)
            tu.m_texture = nullptr;
        m_prev = m_current;
    }

    bool Graphics::init()
    {
        if (!::glewInit()) return false;

        return true;
    }

    void Graphics::set_viewport(int x, int y, int width, int height)
    {
        ::glViewport(x, y, width, height);
    }

    void Graphics::clear()
    {
        ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Graphics::reset_state()
    {
        m_current.m_cull_mode = CULL_BackFace;
    }

    void Graphics::update_state()
    {
        if (m_current.m_cull_mode != m_prev.m_cull_mode)
        {
            static const GLenum cull_modes[] = { GL_BACK, GL_FRONT };

            if (m_prev.m_cull_mode == CULL_None)
                ::glEnable(GL_CULL_FACE);

            if (m_current.m_cull_mode == CULL_None)
                ::glDisable(GL_CULL_FACE);
            else
                ::glCullFace(cull_modes[m_current.m_cull_mode]);
            m_prev.m_cull_mode = m_current.m_cull_mode;
        }

        for (int i = 0; i < MAX_TEXTURE_UNITS; i++)
        {
            TextureUnit &tu = m_current.m_texture_units[i];
            TextureUnit &prev_tu = m_prev.m_texture_units[i];
            if (tu.m_texture && tu.m_texture != prev_tu.m_texture)
            {
                ::glActiveTexture(GL_TEXTURE0 + i);
                ::glBindTexture(tu.m_texture->get_target(), tu.m_texture->get_id());
                prev_tu = tu;
            }
        }
    }

    void Graphics::set_cull_mode(CullMode mode)
    { m_current.m_cull_mode = mode; }

    CullMode Graphics::get_cull_mode() const
    { return m_current.m_cull_mode; }

    void Graphics::set_texture(int unit, Texture *texture)
    {
        m_current.m_texture_units[unit].m_texture = texture;
    }

} // der
