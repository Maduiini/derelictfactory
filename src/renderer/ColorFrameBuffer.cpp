
#include "ColorFrameBuffer.h"

#include <GL/glew.h>

namespace der
{

    ColorFrameBuffer::ColorFrameBuffer(int width, int height)
        : m_color_texture()
        , m_normal_texture()
        , m_extra_texture()
        , m_renderbuffer()
        , m_width(width)
        , m_height(height)
    {
        bind();

        // Create render buffer object with depth component
        ::glGenRenderbuffers(1, &m_renderbuffer);
        ::glBindRenderbuffer(GL_RENDERBUFFER, m_renderbuffer);
        ::glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);

        // Create textures
        initialize_texture(GL_COLOR_ATTACHMENT0, &m_color_texture);
        initialize_texture(GL_COLOR_ATTACHMENT1, &m_normal_texture);
        initialize_texture(GL_COLOR_ATTACHMENT2, &m_extra_texture);

        // Attach render buffer to the frame buffer
        ::glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderbuffer);

        FrameBuffer::bind_default_buffer();
    }

    void ColorFrameBuffer::initialize_texture(GLenum attachment, Texture *texture)
    {
        ::glBindTexture(texture->get_target(), texture->get_id());
        ::glTexImage2D(texture->get_target(), 0, GL_RGBA16F, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        ::glTexParameteri(texture->get_target(), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        ::glTexParameteri(texture->get_target(), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        ::glTexParameteri(texture->get_target(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        ::glTexParameteri(texture->get_target(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        bind();
        ::glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texture->get_target(), texture->get_id(), 0);
    }

    void ColorFrameBuffer::set_draw_buffers()
    {
        GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
        ::glDrawBuffers(3, draw_buffers);
    }

} // der
