
#ifndef H_DER_VERTEX_ARRAY_OBJECT_H
#define H_DER_VERTEX_ARRAY_OBJECT_H

#include "GLTypes.h"
#include "../Types.h"

namespace der
{

    /// The predefined vertex attributes
    enum class VertexAttrib
    {
        Position,
        Normal,
        Tangent,
        TexCoord,
        Color
    };

    /// The vertex attribute element types.
    enum class ElementType
    {
        Float, Half,
        Int8, UInt8,
        Int16, UInt16,
        Int32, UInt32
    };

    class VertexArrayObject
    {
    public:
        VertexArrayObject();
        ~VertexArrayObject();

        void bind();

        void set_attribute(VertexAttrib attrib, size_t offset, size_t stride,
                           size_t size, ElementType type = ElementType::Float, bool normalized = false);
        void unset_attribute(VertexAttrib attrib);

    private:
        GLuint m_vao;
    };

} // der

#endif // H_DER_VERTEX_ARRAY_OBJECT_H

