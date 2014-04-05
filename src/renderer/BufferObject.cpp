
#include "BufferObject.h"

#include <GL/glew.h>

namespace der
{

    BufferObject::BufferObject(GLenum target)
        : m_buffer()
        , m_target(target)
        , m_size_bytes(0)
        , m_dynamic(false)
    {
        ::glGenBuffers(1, &m_buffer);
    }

    BufferObject::~BufferObject()
    {
        ::glDeleteBuffers(1, &m_buffer);
    }

    void BufferObject::bind()
    {
        ::glBindBuffer(m_target, m_buffer);
    }

    void BufferObject::resize(size_t size_bytes, bool dynamic)
    {
        m_size_bytes = size_bytes;
        m_dynamic = dynamic;
        ::glBufferData(m_target, size_bytes, 0,
                       dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }

    void BufferObject::write(size_t offset, size_t size, const void *data)
    {
        ::glBufferSubData(m_target, offset, size, data);
    }


} // der
