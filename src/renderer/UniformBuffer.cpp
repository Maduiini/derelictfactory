
#include "UniformBuffer.h"

#include <GL/glew.h>

namespace der
{

    UniformBuffer::UniformBuffer()
        : BufferObject(GL_UNIFORM_BUFFER)
    { }



    void GlobalUniformBlock::set_projection_mat(const Matrix4 &proj)
    { m_projection = proj; }

    void GlobalUniformBlock::set_view_mat(const Matrix4 &view)
    { m_view = view; }

    void GlobalUniformBlock::set_model_mat(const Matrix4 &model)
    { m_model = model; }

    void GlobalUniformBlock::write_block()
    {
        const size_t mat4_size = sizeof(Matrix4);
        write(mat4_size * 0, mat4_size, reinterpret_cast<const void*>(&m_projection.m));
        write(mat4_size * 1, mat4_size, reinterpret_cast<const void*>(&m_view.m));
        write(mat4_size * 2, mat4_size, reinterpret_cast<const void*>(&m_model.m));
    }

} // der
