
#include "UniformBuffer.h"

#include <GL/glew.h>

namespace der
{

    UniformBuffer::UniformBuffer()
        : BufferObject(GL_UNIFORM_BUFFER)
        , m_size(0)
    { }

    void UniformBuffer::add_float(float &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);

        m_uniforms.push_back(uniform);
        m_size += uniform.size;
    }

    void UniformBuffer::add_vec2(Vector2 &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);

        m_uniforms.push_back(uniform);
        m_size += uniform.size;
    }

    void UniformBuffer::add_vec3(Vector3 &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);

        m_uniforms.push_back(uniform);
        m_size += uniform.size;
    }

    void UniformBuffer::add_vec4(Vector4 &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);

        m_uniforms.push_back(uniform);
        m_size += uniform.size;
    }

    void UniformBuffer::add_mat3(Matrix3 &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);

        m_uniforms.push_back(uniform);
        m_size += uniform.size;
    }

    void UniformBuffer::add_mat4(Matrix4 &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);

        m_uniforms.push_back(uniform);
        m_size += uniform.size;
    }

    void UniformBuffer::apply_format()
    {
        bind();
        resize(m_size, true);
    }

    void UniformBuffer::update()
    {
        bind();
        size_t offset = 0;
        for (size_t i = 0; i < m_uniforms.size(); i++)
        {
            const UniformPtr &uniform = m_uniforms[i];
            write(offset, uniform.size, uniform.ptr);
            offset += uniform.size;
        }
    }



    GlobalUniformBlock::GlobalUniformBlock()
    {
        add_mat4(m_projection);
        add_mat4(m_view);
        add_float(m_time);
        apply_format();
    }

    void GlobalUniformBlock::set_projection_mat(const Matrix4 &proj)
    { m_projection = proj; }

    void GlobalUniformBlock::set_view_mat(const Matrix4 &view)
    { m_view = view; }

    void GlobalUniformBlock::set_time(float time)
    { m_time = time; }



    InstanceUniformBlock::InstanceUniformBlock()
    {
        add_mat4(m_model);
        apply_format();
    }

    void InstanceUniformBlock::set_model_mat(const Matrix4 &model)
    { m_model = model; }




} // der
