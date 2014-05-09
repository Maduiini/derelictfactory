
#include "TextureCube.h"
#include "../Debug.h"

#include <GL/glew.h>
#include "TextureInternalConv.h"

namespace der
{

    static const GLenum g_gl_cube_targets[] = {
        [TextureCube::Negative_X] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        [TextureCube::Positive_X] = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
//        [TextureCube::Negative_Y] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
//        [TextureCube::Positive_Y] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        [TextureCube::Negative_Y] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        [TextureCube::Positive_Y] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        [TextureCube::Negative_Z] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
        [TextureCube::Positive_Z] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z
    };


    TextureCube::TextureCube()
        : Texture(GL_TEXTURE_CUBE_MAP)
        , m_sides_present(0)
        , m_width(0)
        , m_height(0)
        , m_format(RGB8)
    { }

    bool TextureCube::is_complete() const
    { return m_sides_present == 0x3f; }

    void TextureCube::TexImage(Side side, int w, int h, Format fmt, InputFormat input_fmt, const void *data)
    {
        set_format(w, h, fmt);

        const GLenum internal_fmt = g_gl_internal_fmts[m_format];
        const GLenum gl_fmt = g_gl_formats[input_fmt];
        const GLenum gl_type = g_gl_types[input_fmt];
        const GLenum gl_target = g_gl_cube_targets[side];

        ::glBindTexture(m_target, m_tex_id);

        // Set filtering options. These should be set in bind() method when ever they are changed.
        ::glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        ::glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        ::glTexParameterf(m_target, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f);
        ::glTexParameteri(m_target, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        ::glTexParameteri(m_target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        ::glTexParameteri(m_target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Upload the texture image data
        ::glTexImage2D(gl_target, 0, internal_fmt, w, h, 0, gl_fmt, gl_type, data);

        m_sides_present |= 1UL << static_cast<size_t>(side);

        // Generate mipmaps if the cubemap is complete
        if (is_complete())
            ::glGenerateMipmap(m_target);
    }

    void TextureCube::set_format(int w, int h, Format fmt)
    {
        if (m_sides_present == 0)
        {
            m_width = w;
            m_height = h;
            m_format = fmt;
        }
        else
        {
            DER_ASSERT(w == m_width);
            DER_ASSERT(h == m_height);
            DER_ASSERT(fmt == m_format);
        }
    }

} // der
