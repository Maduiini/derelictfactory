
#include "IndexBuffer.h"

#include <GL/glew.h>

namespace der
{

    IndexBuffer::IndexBuffer()
        : BufferObject(GL_ELEMENT_ARRAY_BUFFER)
        , m_gl_type()
    { }

    void IndexBuffer::set_index_type(Type type)
    {
        static const GLenum types[] = { GL_UNSIGNED_SHORT, GL_UNSIGNED_INT };
        static const size_t sizes[] = { 2, 4 };
        m_gl_type = types[type];
        m_element_size = sizes[type];
    }

    IndexBuffer::Type IndexBuffer::get_index_type() const
    {
        if (m_gl_type == GL_UNSIGNED_SHORT)
            return Index16;
        return Index32;
    }

    void IndexBuffer::draw_triangles(size_t start_index, size_t index_count)
    {
        const void *start = reinterpret_cast<const void*>(start_index * m_element_size);
        ::glDrawElements(GL_TRIANGLES, index_count, m_gl_type, start);
    }

} // der