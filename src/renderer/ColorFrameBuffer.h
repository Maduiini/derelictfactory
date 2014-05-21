
#ifndef H_DER_COLOR_FRAME_BUFFER_H
#define H_DER_COLOR_FRAME_BUFFER_H

#include "FrameBuffer.h"
#include "Texture.h"

namespace der
{

    class ColorFrameBuffer : public FrameBuffer
    {
    public:
        ColorFrameBuffer(int width, int height);

        Texture2D* get_texture()
        { return &m_color_texture; }

    private:
        Texture2D m_color_texture;
        GLuint m_renderbuffer;

    };


}

#endif // H_DER_COLOR_FRAME_BUFFER_H
