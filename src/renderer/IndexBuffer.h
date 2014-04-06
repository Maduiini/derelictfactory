
#ifndef H_DER_INDEX_BUFFER_H
#define H_DER_INDEX_BUFFER_H

#include "BufferObject.h"

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

        void set_index_type(Type type);
        Type get_index_type() const;

        void draw_triangles(size_t start_index, size_t index_count);

    private:
        GLenum m_gl_type;
        size_t m_element_size;
    };

} // der

#endif // H_DER_INDEX_BUFFER_H
