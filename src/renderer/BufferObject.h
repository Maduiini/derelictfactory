
#ifndef H_DER_BUFFER_OBJECT_H
#define H_DER_BUFFER_OBJECT_H

#include "GLTypes.h"

namespace der
{

    typedef unsigned int size_t;

    class BufferObject
    {
    public:
//        enum MapAccess
//        {
//            MAP_Read,
//            MAP_Write,
//            MAP_ReadWrite,
//        };
    public:
        BufferObject(GLenum target);
        ~BufferObject();

        void bind();

        void resize(size_t size_bytes, bool dynamic);

        void write(size_t offset, size_t size, const void *data);

//        void* map(MapAccess access);
//        void unmap();

        size_t get_size() const;
        bool is_dynamic() const;

    private:
        GLuint m_buffer;
        GLenum m_target;
        size_t m_size_bytes;
        bool m_dynamic;
    };

} // der

#endif // H_DER_BUFFER_OBJECT_H

