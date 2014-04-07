
#ifndef H_DER_VERTEX_BUFFER_H
#define H_DER_VERTEX_BUFFER_H

#include "BufferObject.h"

namespace der
{

    class VertexBuffer : private BufferObject
    {
    public:
        VertexBuffer();

        using BufferObject::bind;
        using BufferObject::unbind;
        using BufferObject::resize;
        using BufferObject::write;
        using BufferObject::get_size;
        using BufferObject::is_dynamic;
    };

} // der

#endif // H_DER_VERTEX_BUFFER_H

