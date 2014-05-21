
#include "Renderer.h"
#include "Graphics.h"
#include "Material.h"
#include "UniformBuffer.h"
#include "VertexArrayObject.h"
#include "IndexBuffer.h"

#include "../resource/ResourceCache.h"

#include "../Log.h"

#include <algorithm> // for std::sort

namespace der
{

    struct command_cmp_struct
    {
        const Vector3 m_camera_pos;

        explicit command_cmp_struct(const Vector3 &camera_pos)
            : m_camera_pos(camera_pos)
        { }

        bool cmp_materials(const RenderCommand &a, const RenderCommand &b) const
        {
            return a.material->get_albedo_texture() < b.material->get_albedo_texture();
        }

        static float distance2(const Vector3 &a, const Vector3 &b)
        { return (a - b).length2(); }

        bool cmp_distance(const RenderCommand &a, const RenderCommand &b) const
        {
            const Vector3 a_pos = a.model_mat.get_translation();
            const Vector3 b_pos = b.model_mat.get_translation();
            return distance2(a_pos, m_camera_pos) > distance2(b_pos, m_camera_pos);
        }

        bool operator ()(const RenderCommand &a, const RenderCommand &b) const
        {
            return cmp_distance(a, b);
        }
    };

    static bool command_cmp(const RenderCommand &a, const RenderCommand &b)
    {
        return a.material->get_albedo_texture() < b.material->get_albedo_texture();
    }



    Renderer::Renderer(Graphics *graphics, ResourceCache *cache)
        : m_graphics(graphics)
        , m_cache(cache)
    {
        m_commands.reserve(1000);
        m_blend_commands.reserve(1000);
        m_global_uniforms = new GlobalUniformBlock();
        m_instance_uniforms = new InstanceUniformBlock();
        m_light_uniforms = new LightUniformBlock();
        m_param_uniforms = new ParamUniformBlock();
    }

    Renderer::~Renderer()
    {
        delete m_global_uniforms;
        delete m_instance_uniforms;
        delete m_light_uniforms;
        delete m_param_uniforms;
    }

    void Renderer::set_projection_matrix(const Matrix4 &proj_mat)
    { m_global_uniforms->set_projection_mat(proj_mat); }

    void Renderer::set_view_matrix(const Matrix4 &view_mat)
    { m_global_uniforms->set_view_mat(view_mat); }

    void Renderer::set_camera_pos(const Vector3 &camera_pos)
    { m_global_uniforms->set_camera_pos(camera_pos); }

    void Renderer::set_time(float time)
    { m_global_uniforms->set_time(time); }

    void Renderer::set_normalmap_influence(float value)
    { m_param_uniforms->set_normalmap_influence(value); }


    void Renderer::set_model_matrix(const Matrix4 &model_mat)
    { m_command.model_mat = model_mat; }


    void Renderer::set_light_count(size_t light_count)
    { m_command.light_count = light_count; }

    void Renderer::set_light_position(size_t light, const Vector3 &pos, LightType type)
    {
        m_command.lights[light].position = pos;
        m_command.lights[light].type = type;
    }

    void Renderer::set_light_direction(size_t light, const Vector3 &direction)
    { m_command.lights[light].direction = direction; }

    void Renderer::set_light_color(size_t light, const Vector3 &color, float energy)
    { m_command.lights[light].color_energy = Vector4(color, energy); }

    void Renderer::set_light_radius(size_t light, float radius)
    { m_command.lights[light].radius = radius; }

    void Renderer::set_light_spot_angle(size_t light, float spot_angle)
    { m_command.lights[light].spot_angle = spot_angle; }


    void Renderer::set_material(ResourceID material_id)
    { m_command.material = m_cache->get<Material>(material_id); }

    void Renderer::set_vao(VertexArrayObject *vao)
    { m_command.vao = vao; }

    void Renderer::set_primitive_type(PrimitiveType prim_type)
    { m_command.prim_type = prim_type; }

    void Renderer::set_indices(IndexBuffer *ib, size_t start_index, size_t index_count)
    {
        m_command.index_buffer = ib;
        m_command.start_index = start_index;
        m_command.index_count = index_count;
    }

    void Renderer::emit_command()
    {
        if (m_command.material)
        {
            size_t i = LightUniformBlock::MAX_LIGHTS - 1;
            for (; i >= m_command.light_count; i--)
                m_command.lights[i].radius = 0.0f;

            if (m_command.material->is_blending_enabled())
                m_blend_commands.push_back(m_command);
            else
                m_commands.push_back(m_command);
        }
    }

    void Renderer::bind_global_uniforms()
    {
        m_global_uniforms->bind_uniforms();
        m_param_uniforms->bind_uniforms();
    }

    void Renderer::render_command()
    {
        if (m_command.material)
            render_command(m_command);
    }

    void Renderer::render()
    {
        std::sort(m_commands.begin(), m_commands.end(), command_cmp);
//        std::sort(m_commands.begin(), m_commands.end(), cmd_cmp);
        const command_cmp_struct blend_cmd_cmp(m_global_uniforms->get_camera_pos());
        std::sort(m_blend_commands.begin(), m_blend_commands.end(), blend_cmd_cmp);

        bind_global_uniforms();
        for (RenderCommand &command : m_commands)
        {
            render_command(command);
        }
        for (RenderCommand &command : m_blend_commands)
        {
            render_command(command);
        }

        m_commands.clear();
        m_blend_commands.clear();
    }

    void Renderer::render_command(RenderCommand &command)
    {
        m_instance_uniforms->set_model_mat(command.model_mat);
        m_instance_uniforms->bind_uniforms();

        m_light_uniforms->set_light_count(command.light_count);
//        for (size_t i = 0; i < command.light_count; i++)
        for (size_t i = 0; i < LightUniformBlock::MAX_LIGHTS; i++)
        {
            m_light_uniforms->set_position(i, command.lights[i].position, command.lights[i].type);
            m_light_uniforms->set_direction(i, command.lights[i].direction);
            m_light_uniforms->set_color(i, command.lights[i].color_energy.xyz(), command.lights[i].color_energy.w);
            m_light_uniforms->set_radius(i, command.lights[i].radius);
            const float cos_spot = (command.lights[i].type == LightType::Spot) ?
                std::cos(command.lights[i].spot_angle) : 0.0f;
            m_light_uniforms->set_cos_spot_angle(i, cos_spot);
        }
        m_light_uniforms->bind_uniforms();

        command.vao->bind();

        command.material->use(m_graphics, m_cache);
        m_graphics->update_state();
        if (command.index_buffer)
        {
            command.index_buffer->bind();
            m_graphics->draw_primitives(command.prim_type, command.index_buffer,
                                        command.start_index, command.index_count);
        }
        else
        {
            m_graphics->draw_primitives(command.prim_type,
                                        command.start_index, command.index_count);
        }
    }

} // der
