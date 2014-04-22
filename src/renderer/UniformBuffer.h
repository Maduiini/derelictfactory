
#ifndef H_DER_UNIFORM_BUFFER_H
#define H_DER_UNIFORM_BUFFER_H

#include "BufferObject.h"

#include "../math/Math.h"

namespace der
{

    class UniformBuffer : private BufferObject
    {
    public:
        UniformBuffer();

        using BufferObject::bind;
        using BufferObject::bind_base;
        using BufferObject::unbind;
        using BufferObject::resize;
        using BufferObject::write;
        using BufferObject::get_size;
        using BufferObject::is_dynamic;

    };



    class GlobalUniformBlock : public UniformBuffer
    {
    public:
        void set_projection_mat(const Matrix4 &proj);
        void set_view_mat(const Matrix4 &view);
        void set_model_mat(const Matrix4 &model);

        void write_block();

    private:
        Matrix4 m_projection;
        Matrix4 m_view;
        Matrix4 m_model;
    };

} // der

#endif // H_DER_UNIFORM_BUFFER_H
