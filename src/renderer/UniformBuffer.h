
#ifndef H_DER_UNIFORM_BUFFER_H
#define H_DER_UNIFORM_BUFFER_H

#include "BufferObject.h"

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
        void add_sampler(int &value);
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
        };
        std::vector<UniformPtr> m_uniforms;

    };



    class GlobalUniformBlock : public UniformBuffer
    {
    public:
        GlobalUniformBlock();

        void set_projection_mat(const Matrix4 &proj);
        void set_view_mat(const Matrix4 &view);

    private:
        Matrix4 m_projection;
        Matrix4 m_view;
    };

    class InstanceUniformBlock : public UniformBuffer
    {
    public:
        InstanceUniformBlock();

        void set_model_mat(const Matrix4 &model);

    private:
        Matrix4 m_model;
    };

} // der

#endif // H_DER_UNIFORM_BUFFER_H
