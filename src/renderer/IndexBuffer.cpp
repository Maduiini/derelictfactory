
#include "IndexBuffer.h"

#include <GL/glew.h>
#include "PrimitiveTypeInternalConv.h"

namespace der
{

    IndexBuffer::IndexBuffer()
        : BufferObject(GL_ELEMENT_ARRAY_BUFFER)
        , m_gl_type(GL_UNSIGNED_SHORT)
        , m_element_size(2)
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


    void IndexBuffer::draw_primitives(PrimitiveType prim_type, size_t start_index, size_t index_count)
    {
        const void *start = reinterpret_cast<const void*>(start_index * m_element_size);
        ::glDrawElements(g_gl_modes[static_cast<int>(prim_type)], index_count, m_gl_type, start);
    }

    void IndexBuffer::draw_triangles(size_t start_index, size_t index_count)
    {
        const void *start = reinterpret_cast<const void*>(start_index * m_element_size);
        ::glDrawElements(GL_TRIANGLES, index_count, m_gl_type, start);
    }

    void IndexBuffer::draw_triangle_strip(size_t start_index, size_t index_count)
    {
        const void *start = reinterpret_cast<const void*>(start_index * m_element_size);
        ::glDrawElements(GL_TRIANGLE_STRIP, index_count, m_gl_type, start);
    }

    void IndexBuffer::draw_triangle_fan(size_t start_index, size_t index_count)
    {
        const void *start = reinterpret_cast<const void*>(start_index * m_element_size);
        ::glDrawElements(GL_TRIANGLE_FAN, index_count, m_gl_type, start);
    }

    void IndexBuffer::draw_lines(size_t start_index, size_t index_count)
    {
        const void *start = reinterpret_cast<const void*>(start_index * m_element_size);
        ::glDrawElements(GL_LINES, index_count, m_gl_type, start);
    }

    void IndexBuffer::draw_line_strip(size_t start_index, size_t index_count)
    {
        const void *start = reinterpret_cast<const void*>(start_index * m_element_size);
        ::glDrawElements(GL_LINE_STRIP, index_count, m_gl_type, start);
    }

    void IndexBuffer::draw_line_loop(size_t start_index, size_t index_count)
    {
        const void *start = reinterpret_cast<const void*>(start_index * m_element_size);
        ::glDrawElements(GL_LINE_LOOP, index_count, m_gl_type, start);
    }

} // der
