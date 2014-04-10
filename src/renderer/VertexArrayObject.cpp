
#include "VertexArrayObject.h"

#include <GL/glew.h>

namespace der
{

    VertexArrayObject::VertexArrayObject()
        : m_vao()
    {
        ::glGenVertexArrays(1, &m_vao);
    }

    VertexArrayObject::~VertexArrayObject()
    {
        ::glDeleteVertexArrays(1, &m_vao);
    }

    void VertexArrayObject::bind()
    {
        ::glBindVertexArray(m_vao);
    }

    void VertexArrayObject::set_attribute(VertexAttrib attrib, size_t offset, size_t stride,
                                          size_t size, ElementType type /*= ElementType::Float*/, bool normalized /*= false*/)
    {
        const GLenum types[] = {
            GL_FLOAT, GL_HALF_FLOAT,
            GL_BYTE, GL_UNSIGNED_BYTE,
            GL_SHORT, GL_UNSIGNED_SHORT,
            GL_INT, GL_UNSIGNED_INT
        };
        const GLuint index = static_cast<GLuint>(attrib);
        ::glEnableVertexAttribArray(index);
        ::glVertexAttribPointer(index, size, types[static_cast<int>(type)], normalized,
                                stride, reinterpret_cast<const void*>(offset));
    }

    void VertexArrayObject::unset_attribute(VertexAttrib attrib)
    {
        const GLuint index = static_cast<GLuint>(attrib);
        ::glDisableVertexAttribArray(index);
    }

} // der
