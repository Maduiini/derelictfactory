
#include "UniformBuffer.h"

#include <GL/glew.h>

namespace der
{

    UniformBuffer::UniformBuffer()
        : BufferObject(GL_UNIFORM_BUFFER)
        , m_size(0)
    { }

    void UniformBuffer::add_int(int &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);

        m_uniforms.push_back(uniform);
        m_size += uniform.size;
    }

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
        : m_light_count(0)
    {
        add_mat4(m_model);
        add_int(m_light_count);
        apply_format();
    }

    void InstanceUniformBlock::set_model_mat(const Matrix4 &model)
    { m_model = model; }

    void InstanceUniformBlock::set_light_count(int light_count)
    { m_light_count = light_count; }


    LightUniformBlock::LightUniformBlock()
    {
        for (int i = 0; i < MAX_LIGHTS; i++)
        {
            add_vec4(m_lights[i].position);
            add_vec4(m_lights[i].color_energy);
            add_float(m_lights[i].radius);
        }
        apply_format();
    }

    void LightUniformBlock::set_position(int light, const Vector3 &pos, LightType type)
    {
        const float w = (type == LightType::Point) ? 1.0f : 0.0f;
        m_lights[light].position = Vector4(pos, w);
    }

    void LightUniformBlock::set_color(int light, const Vector3 &color, float energy)
    { m_lights[light].color_energy = Vector4(color, energy); }

    void LightUniformBlock::set_radius(int light, float radius)
    { m_lights[light].radius = radius; }


} // der
