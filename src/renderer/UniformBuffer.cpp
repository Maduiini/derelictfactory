
#include "UniformBuffer.h"

#include <GL/glew.h>

namespace der
{

    UniformBuffer::UniformBuffer(size_t binding_point)
        : BufferObject(GL_UNIFORM_BUFFER)
        , m_size(0)
        , m_binding_point(binding_point)
    { }

    void UniformBuffer::add_int(int &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);
        uniform.padded_size = sizeof(Vector4);

        m_uniforms.push_back(uniform);
        m_size += uniform.padded_size;
    }

    void UniformBuffer::add_uint(unsigned int &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);
        uniform.padded_size = sizeof(Vector4);

        m_uniforms.push_back(uniform);
        m_size += uniform.padded_size;
    }

    void UniformBuffer::add_float(float &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);
        uniform.padded_size = sizeof(Vector4);

        m_uniforms.push_back(uniform);
        m_size += uniform.padded_size;
    }

    void UniformBuffer::add_vec2(Vector2 &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);
        uniform.padded_size = sizeof(Vector4);

        m_uniforms.push_back(uniform);
        m_size += uniform.padded_size;
    }

    void UniformBuffer::add_vec3(Vector3 &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);
        uniform.padded_size = sizeof(Vector4);

        m_uniforms.push_back(uniform);
        m_size += uniform.padded_size;
    }

    void UniformBuffer::add_vec4(Vector4 &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);
        uniform.padded_size = sizeof(Vector4);

        m_uniforms.push_back(uniform);
        m_size += uniform.padded_size;
    }

    void UniformBuffer::add_mat3(Matrix3 &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);
        uniform.padded_size = sizeof(Vector4) * 3;

        m_uniforms.push_back(uniform);
        m_size += uniform.padded_size;
    }

    void UniformBuffer::add_mat4(Matrix4 &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);
        uniform.padded_size = sizeof(Matrix4);

        m_uniforms.push_back(uniform);
        m_size += uniform.padded_size;
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
            offset += uniform.padded_size;
        }
    }

    void UniformBuffer::bind_uniforms()
    {
        update();
        bind_base(m_binding_point);
    }



    GlobalUniformBlock::GlobalUniformBlock()
        : UniformBuffer(BindingPoint)
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
        : UniformBuffer(BindingPoint)
    {
        add_mat4(m_model);
        apply_format();
    }

    void InstanceUniformBlock::set_model_mat(const Matrix4 &model)
    { m_model = model; }


    LightUniformBlock::LightUniformBlock()
        : UniformBuffer(BindingPoint)
        , m_light_count(0)
    {
        add_uint(m_light_count);
        for (size_t i = 0; i < MAX_LIGHTS; i++)
        {
            add_vec4(m_lights[i].position);
            add_vec4(m_lights[i].color_energy);
            add_float(m_lights[i].radius);
        }
        apply_format();
    }

    void LightUniformBlock::set_light_count(size_t light_count)
    { m_light_count = light_count; }

    void LightUniformBlock::set_position(size_t light, const Vector3 &pos, LightType type)
    {
        const float w = (type == LightType::Point) ? 1.0f : 0.0f;
        m_lights[light].position = Vector4(pos, w);
    }

    void LightUniformBlock::set_color(size_t light, const Vector3 &color, float energy)
    { m_lights[light].color_energy = Vector4(color, energy); }

    void LightUniformBlock::set_radius(size_t light, float radius)
    { m_lights[light].radius = radius; }


} // der
