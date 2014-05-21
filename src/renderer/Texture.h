
#ifndef H_DER_TEXTURE_H
#define H_DER_TEXTURE_H

#include "GLTypes.h"

namespace der
{

    class Texture
    {
    public:
        enum InputFormat
        {
            IN_R8, IN_RG8, IN_RGB8, IN_RGBA8,
            IN_R16, IN_RG16, IN_RGB16, IN_RGBA16
        };
        enum Format
        {
            R8, RG8, RGB8, RGBA8,
            R16, RG16, RGB16, RGBA16,
            RGB_DXT1, RGBA_DXT1, RGBA_DXT5,
            R_RGTC1, RG_RGTC2
        };
    public:
        explicit Texture(GLenum target);
        ~Texture();

        GLuint get_id() const;
        GLenum get_target() const;

    protected:
        GLuint m_tex_id;
        GLenum m_target;
    };

    class Texture2D : public Texture
    {
    public:
        Texture2D();

        void TexImage(int w, int h, Format fmt, InputFormat input_fmt, const void *data);
    };

    class DepthTexture : public Texture
    {
    public:
        DepthTexture();

        void TexImage(int w, int h, const void *data);
    };

} // der

#endif // H_DER_TEXTURE_H

