
#ifndef H_DER_RENDERER_H
#define H_DER_RENDERER_H

#include "../Types.h"
#include "GraphicsTypes.h"
#include "UniformBuffer.h"

#include "../scene/Light.h"

#include "../math/Math.h"

#include <vector>

namespace der
{

    class Graphics;
    class ResourceCache;

    class Material;
    class VertexArrayObject;
    class IndexBuffer;

    class GlobalUniformBlock;
    class InstanceUniformBlock;
    class LightUniformBlock;
    class ParamsUniformBlock;

    struct RenderCommand
    {
    public:
        Matrix4 model_mat;
        size_t light_count;

        struct LightData
        {
            LightData() : radius(0.0f) { }
            LightType type;
            Vector3 position;
            Vector3 direction;
            Vector4 color_energy;
            float radius;
            float spot_angle;
        } lights[LightUniformBlock::MAX_LIGHTS];

        Material *          material;
        VertexArrayObject * vao;
        PrimitiveType       prim_type;
        IndexBuffer *       index_buffer;
        size_t              start_index;
        size_t              index_count;
    };

    class Renderer
    {
    public:
        explicit Renderer(Graphics *graphics, ResourceCache *cache);
        ~Renderer();

        void set_projection_matrix(const Matrix4 &proj_mat);
        void set_view_matrix(const Matrix4 &view_mat);
        void set_camera_pos(const Vector3 &camera_pos);
        void set_time(float time);

        void set_normalmap_influence(float value);

        void set_model_matrix(const Matrix4 &model_mat);

        void set_light_count(size_t light_count);
        void set_light_position(size_t light, const Vector3 &pos, LightType type);
        void set_light_direction(size_t light, const Vector3 &direction);
        void set_light_color(size_t light, const Vector3 &color, float energy);
        void set_light_radius(size_t light, float radius);
        void set_light_spot_angle(size_t light, float spot_angle);

        void set_material(ResourceID material_id);
        void set_vao(VertexArrayObject *vao);
        void set_primitive_type(PrimitiveType prim_type);
        void set_indices(IndexBuffer *ib, size_t start_index, size_t index_count);
        void emit_command();

        void bind_global_uniforms();
        void render_command();

        void render();

    private:
        void render_command(RenderCommand &command);

    private:
        Graphics *m_graphics;
        ResourceCache *m_cache;

        std::vector<RenderCommand> m_commands;
        std::vector<RenderCommand> m_blend_commands;
        RenderCommand m_command;

        GlobalUniformBlock *    m_global_uniforms;
        InstanceUniformBlock *  m_instance_uniforms;
        LightUniformBlock *     m_light_uniforms;
        ParamUniformBlock *     m_param_uniforms;

    };

} // der

#endif // H_DER_RENDERER_H

