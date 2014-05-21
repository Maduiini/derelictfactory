
#include "FrameBuffer.h"

#include <GL/glew.h>

namespace der
{

    FrameBuffer::FrameBuffer()
        : m_framebuffer()
    {
        ::glGenFramebuffers(1, &m_framebuffer);
    }

    FrameBuffer::~FrameBuffer()
    {
        ::glDeleteFramebuffers(1, &m_framebuffer);
    }

    bool FrameBuffer::is_complete()
    {
        return ::glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }

    void FrameBuffer::bind()
    {
        ::glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
        GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
        ::glDrawBuffers(1, draw_buffers);
    }

    // static
    void FrameBuffer::bind_default_buffer()
    {
        ::glBindFramebuffer(GL_FRAMEBUFFER, 0);
        GLenum draw_buffers[] = { GL_BACK_LEFT };
        ::glDrawBuffers(1, draw_buffers);
    }

}
