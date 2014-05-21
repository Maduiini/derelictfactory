
#include "ColorFrameBuffer.h"

#include <GL/glew.h>

namespace der
{

    ColorFrameBuffer::ColorFrameBuffer(int width, int height)
        : m_color_texture()
        , m_renderbuffer()
    {
        bind();

        // Create render buffer object with depth component
        ::glGenRenderbuffers(1, &m_renderbuffer);
        ::glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
        ::glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

        // Create color texture and set filtering
        ::glActiveTexture(GL_TEXTURE0);
        ::glBindTexture(m_color_texture.get_target(), m_color_texture.get_id());
        ::glTexImage2D(m_color_texture.get_target(), 0, GL_RGB8, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
        ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        ::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Attach texture to the frame buffer
        ::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_color_texture.get_target(), m_color_texture.get_id(), 0);

        // Attach render buffer to the frame buffer
        ::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);

        FrameBuffer::bind_default_buffer();
    }

} // der
