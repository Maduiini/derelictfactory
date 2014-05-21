
#include "DepthOnlyFrameBuffer.h"

#include <GL/glew.h>

namespace der
{

    DepthOnlyFrameBuffer::DepthOnlyFrameBuffer(int size)
        : m_depth_texture()
        , m_size(size)
    {
        bind();

        // Initialize depth texture
        m_depth_texture.TexImage(m_size, m_size, NULL);

        // Attach depth texture to the frame buffer
        ::glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_depth_texture.get_target(), m_depth_texture.get_id(), 0);

        GLenum draw_buffers[] = { GL_NONE };
        ::glDrawBuffers(1, draw_buffers);

        FrameBuffer::bind_default_buffer();
    }

}
