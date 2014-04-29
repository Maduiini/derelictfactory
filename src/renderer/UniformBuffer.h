
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
        UniformBuffer();

        using BufferObject::bind;
        using BufferObject::bind_base;
        using BufferObject::unbind;
        using BufferObject::resize;
        using BufferObject::write;
        using BufferObject::get_size;
        using BufferObject::is_dynamic;

        void update();

    protected:
        void add_int(int &value);
        void add_float(float &value);
        void add_vec2(Vector2 &value);
        void add_vec3(Vector3 &value);
        void add_vec4(Vector4 &value);
        void add_mat3(Matrix3 &value);
        void add_mat4(Matrix4 &value);
        void apply_format();

    protected:
        size_t m_size;

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
        GlobalUniformBlock();

        void set_projection_mat(const Matrix4 &proj);
        void set_view_mat(const Matrix4 &view);
        void set_time(float time);

    private:
        Matrix4 m_projection;
        Matrix4 m_view;
        float m_time;
    };

    class InstanceUniformBlock : public UniformBuffer
    {
    public:
        InstanceUniformBlock();

        void set_model_mat(const Matrix4 &model);
        void set_light_count(int light_count);

    private:
        Matrix4 m_model;
        int m_light_count;
    };

    class LightUniformBlock : public UniformBuffer
    {
    public:
        static const int MAX_LIGHTS = 16;
    public:
        LightUniformBlock();

        void set_position(int light, const Vector3 &pos, LightType type);
        void set_color(int light, const Vector3 &color, float energy);
        void set_radius(int light, float radius);

    private:
        struct LightData
        {
            Vector4 position;      // Position(w=1) or direction(w=0)
            Vector4 color_energy;  // rgb = color, w = energy
            float radius;
            float padd[3];
        } m_lights[MAX_LIGHTS];
    };

} // der

#endif // H_DER_UNIFORM_BUFFER_H
