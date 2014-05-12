
#ifndef H_DER_INDEX_BUFFER_H
#define H_DER_INDEX_BUFFER_H

#include "BufferObject.h"
#include "GraphicsTypes.h"

namespace der
{

    class IndexBuffer : private BufferObject
    {
    public:
        enum Type
        {
            Index16, Index32
        };
    public:
        IndexBuffer();

        using BufferObject::bind;
        using BufferObject::unbind;
        using BufferObject::resize;
        using BufferObject::write;
        using BufferObject::get_size;
        using BufferObject::is_dynamic;

        void set_index_type(Type type);
        Type get_index_type() const;

        void draw_primitives(PrimitiveType prim_type, size_t start_index, size_t index_count);

        void draw_triangles(size_t start_index, size_t index_count);
        void draw_triangle_strip(size_t start_index, size_t index_count);
        void draw_triangle_fan(size_t start_index, size_t index_count);

        void draw_lines(size_t start_index, size_t index_count);
        void draw_line_strip(size_t start_index, size_t index_count);
        void draw_line_loop(size_t start_index, size_t index_count);

    private:
        GLenum m_gl_type;
        size_t m_element_size;
    };

} // der

#endif // H_DER_INDEX_BUFFER_H

