
#include "Texture.h"

#include <GL/glew.h>

namespace der
{

    static const GLenum g_gl_formats[] = {
        [Texture::IN_R8]        = GL_R8,
        [Texture::IN_RG8]       = GL_RG8,
        [Texture::IN_RGB8]      = GL_RGB8,
        [Texture::IN_RGBA8]     = GL_RGBA8
    };

    static const GLenum g_gl_types[] = {
        [Texture::IN_R8]        = GL_UNSIGNED_BYTE,
        [Texture::IN_RG8]       = GL_UNSIGNED_BYTE,
        [Texture::IN_RGB8]      = GL_UNSIGNED_BYTE,
        [Texture::IN_RGBA8]     = GL_UNSIGNED_BYTE
    };

    static const GLenum g_gl_internal_fmts[] = {
        [Texture::R8]           = GL_R8,
        [Texture::RG8]          = GL_RG8,
        [Texture::RGB8]         = GL_RGB8,
        [Texture::RGBA8]        = GL_RGBA8,
        [Texture::RGB_DXT1]     = GL_COMPRESSED_RGB_S3TC_DXT1_EXT,
        [Texture::RGBA_DXT1]    = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
        [Texture::RGBA_DXT5]    = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
        [Texture::R_RGTC1]      = GL_COMPRESSED_RED_RGTC1,
        [Texture::RG_RGTC2]     = GL_COMPRESSED_RG_RGTC2
    };



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

        // Upload the texture image data
        ::glTexImage2D(m_target, 0, internal_fmt, w, h, 0, gl_fmt, gl_type, data);

        // Generate mipmaps
        ::glGenerateMipmap(m_target);
    }

} // der
