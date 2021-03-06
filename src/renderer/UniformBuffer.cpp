
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

    void UniformBuffer::add_int_np(int &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);
        uniform.padded_size = sizeof(value);

        m_uniforms.push_back(uniform);
        m_size += uniform.padded_size;
    }

    void UniformBuffer::add_uint_np(unsigned int &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);
        uniform.padded_size = sizeof(value);

        m_uniforms.push_back(uniform);
        m_size += uniform.padded_size;
    }

    void UniformBuffer::add_float_np(float &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);
        uniform.padded_size = sizeof(value);

        m_uniforms.push_back(uniform);
        m_size += uniform.padded_size;
    }

    void UniformBuffer::add_vec2_np(Vector2 &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);
        uniform.padded_size = sizeof(value);

        m_uniforms.push_back(uniform);
        m_size += uniform.padded_size;
    }

    void UniformBuffer::add_vec3_np(Vector3 &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);
        uniform.padded_size = sizeof(value);

        m_uniforms.push_back(uniform);
        m_size += uniform.padded_size;
    }

    void UniformBuffer::add_mat3_np(Matrix3 &value)
    {
        UniformPtr uniform;
        uniform.ptr = &value;
        uniform.size = sizeof(value);
        uniform.padded_size = sizeof(value);

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
        add_vec3_np(m_camera_pos);
        add_float_np(m_time);
        add_float_np(m_window_width);
        add_float_np(m_window_height);
        apply_format();
    }

    void GlobalUniformBlock::set_projection_mat(const Matrix4 &proj)
    { m_projection = proj; }

    void GlobalUniformBlock::set_view_mat(const Matrix4 &view)
    { m_view = view; }

    void GlobalUniformBlock::set_camera_pos(const Vector3 &camera_pos)
    { m_camera_pos = camera_pos; }

    Vector3 GlobalUniformBlock::get_camera_pos() const
    { return m_camera_pos; }

    void GlobalUniformBlock::set_time(float time)
    { m_time = time; }

    void GlobalUniformBlock::set_window_size(int width, int height)
    {
        m_window_width = static_cast<float>(width);
        m_window_height = static_cast<float>(height);
    }



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
        add_mat4(m_light_mat);
        add_uint(m_light_count);
        for (size_t i = 0; i < MAX_LIGHTS; i++)
        {
            add_vec4(m_lights[i].position);
            add_vec4(m_lights[i].direction);
            add_vec4(m_lights[i].color_energy);
            add_float(m_lights[i].radius);
        }
        apply_format();
    }

    void LightUniformBlock::set_light_count(size_t light_count)
    { m_light_count = light_count; }

    void LightUniformBlock::set_position(size_t light, const Vector3 &pos, LightType type)
    {
        const float w = (type != LightType::Directional) ? 1.0f : 0.0f;
        m_lights[light].position = Vector4(pos, w);
    }

    void LightUniformBlock::set_direction(size_t light, const Vector3 &direction)
    {
        m_lights[light].direction.x = direction.x;
        m_lights[light].direction.y = direction.y;
        m_lights[light].direction.z = direction.z;
    }

    void LightUniformBlock::set_color(size_t light, const Vector3 &color, float energy)
    { m_lights[light].color_energy = Vector4(color, energy); }

    void LightUniformBlock::set_radius(size_t light, float radius)
    { m_lights[light].radius = radius; }

    void LightUniformBlock::set_cos_spot_angle(size_t light, float cos_spot_angle)
    { m_lights[light].direction.w = cos_spot_angle; }

    void LightUniformBlock::set_light_matrix(const Matrix4 &light_mat)
    { m_light_mat = light_mat; }



    ParamUniformBlock::ParamUniformBlock()
        : UniformBuffer(BindingPoint)
        , m_nm_influence(1.0f)
        , m_sm_enabled(1.0f)
    {
        add_float_np(m_nm_influence);
        add_float_np(m_sm_enabled);
        apply_format();
    }

    void ParamUniformBlock::set_normalmap_influence(float value)
    { m_nm_influence = value; }

    void ParamUniformBlock::set_shadow_map_enabled(bool enabled)
    { m_sm_enabled = enabled ? 1.0f : 0.0f; }


} // der
