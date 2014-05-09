
namespace der
{

    static const GLenum g_gl_formats[] = {
        [Texture::IN_R8]        = GL_RED,
        [Texture::IN_RG8]       = GL_RG,
        [Texture::IN_RGB8]      = GL_RGB,
        [Texture::IN_RGBA8]     = GL_RGBA
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

} // der
