
#ifndef H_DER_UNIFORM_BUFFER_H
#define H_DER_UNIFORM_BUFFER_H

#include "BufferObject.h"
#include "../scene/Light.h"

#include "../math/Math.h"

#include <vector>

namespace der
{

    class UniformBuffer : private BufferObject
    {
    public:
        UniformBuffer(size_t binding_point);

        using BufferObject::bind;
        using BufferObject::bind_base;
        using BufferObject::unbind;
        using BufferObject::resize;
        using BufferObject::write;
        using BufferObject::get_size;
        using BufferObject::is_dynamic;

        void update();
        void bind_uniforms();

    protected:
        void add_int(int &value);
        void add_uint(unsigned int &value);
        void add_float(float &value);
        void add_vec2(Vector2 &value);
        void add_vec3(Vector3 &value);
        void add_vec4(Vector4 &value);
        void add_mat3(Matrix3 &value);
        void add_mat4(Matrix4 &value);
        void apply_format();

    protected:
        size_t m_size;
        size_t m_binding_point;

        struct UniformPtr
        {
            const void *ptr;
            size_t      size;
            size_t      padded_size;
        };
        std::vector<UniformPtr> m_uniforms;

    };



    class GlobalUniformBlock : public UniformBuffer
    {
    public:
        static const unsigned int BindingPoint = 0;
    public:
        GlobalUniformBlock();

        void set_projection_mat(const Matrix4 &proj);
        void set_view_mat(const Matrix4 &view);
        void set_camera_pos(const Vector3 &camera_pos);
        void set_time(float time);

    private:
        Matrix4 m_projection;
        Matrix4 m_view;
        Vector3 m_camera_pos;
        float m_time;
    };

    class InstanceUniformBlock : public UniformBuffer
    {
    public:
        static const unsigned int BindingPoint = 1;
    public:
        InstanceUniformBlock();

        void set_model_mat(const Matrix4 &model);

    private:
        Matrix4 m_model;
    };

    class LightUniformBlock : public UniformBuffer
    {
    public:
        static const unsigned int BindingPoint = 2;
    public:
        static const size_t MAX_LIGHTS = 16;
    public:
        LightUniformBlock();

        void set_light_count(size_t light_count);

        void set_position(size_t light, const Vector3 &pos, LightType type);
        void set_color(size_t light, const Vector3 &color, float energy);
        void set_radius(size_t light, float radius);

    private:
        unsigned int m_light_count;
        struct LightData
        {
            Vector4 position;      // Position(w=1) or direction(w=0)
            Vector4 color_energy;  // rgb = color, w = energy
            float radius;
//            Vector4 radius;
        } m_lights[MAX_LIGHTS];
    };

} // der

#endif // H_DER_UNIFORM_BUFFER_H
