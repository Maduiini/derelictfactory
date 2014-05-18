
#include "FrameBuffer.h"

#include <GL/glew.h>

namespace der
{

    FrameBuffer::FrameBuffer()
        : m_buffer()
    {
        ::glGenFramebuffers(1, &m_buffer);
    }

    FrameBuffer::~FrameBuffer()
    {
        ::glDeleteFramebuffers(1, &m_buffer);
    }

    bool FrameBuffer::is_complete()
    {
        return ::glCheckFramebufferStatus(m_buffer) == GL_FRAMEBUFFER_COMPLETE;
    }

    void FrameBuffer::bind()
    {
        ::glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
    }

    // static
    void FrameBuffer::bind_default_buffer()
    {
        ::glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

}
