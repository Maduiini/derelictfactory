
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

        virtual void set_draw_buffers() override;

        Texture2D* get_color_texture()
        { return &m_color_texture; }

        Texture2D* get_normal_texture()
        { return &m_normal_texture; }

        Texture2D* get_extra_texture()
        { return &m_extra_texture; }

        void set_viewport();

    private:
        void initialize_texture(GLenum position, Texture *texture);

    private:
        Texture2D m_color_texture;
        Texture2D m_normal_texture;
        Texture2D m_extra_texture;
        GLuint m_renderbuffer;

        int m_width;
        int m_height;

    };


}

#endif // H_DER_COLOR_FRAME_BUFFER_H
