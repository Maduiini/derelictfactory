
#include "Texture.h"

#include <GL/glew.h>
#include "TextureInternalConv.h"

namespace der
{



    Texture::Texture(GLenum target)
        : m_tex_id()
        , m_target(target)
    {
        ::glGenTextures(1, &m_tex_id);
    }

    Texture::~Texture()
    {
        ::glDeleteTextures(1, &m_tex_id);
    }

    GLuint Texture::get_id() const
    { return m_tex_id; }

    GLenum Texture::get_target() const
    { return m_target; }



    Texture2D::Texture2D()
        : Texture(GL_TEXTURE_2D)
    { }

    void Texture2D::TexImage(int w, int h, Format fmt, InputFormat input_fmt, const void *data)
    {
        const GLenum internal_fmt = g_gl_internal_fmts[fmt];
        const GLenum gl_fmt = g_gl_formats[input_fmt];
        const GLenum gl_type = g_gl_types[input_fmt];

        ::glBindTexture(m_target, m_tex_id);

        // Set filtering options. These should be set in bind() method when ever they are changed.
        ::glTexParameteri(m_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        ::glTexParameteri(m_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        ::glTexParameteri(m_target, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);

        // Upload the texture image data
        ::glTexImage2D(m_target, 0, internal_fmt, w, h, 0, gl_fmt, gl_type, data);

        // Generate mipmaps
        ::glGenerateMipmap(m_target);
    }

} // der
