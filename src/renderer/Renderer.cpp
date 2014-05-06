
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

    static bool command_cmp(const RenderCommand &a, const RenderCommand &b)
    {
        return a.material->get_albedo_texture() < b.material->get_albedo_texture();
    }

//    RenderCommand::RenderCommand()
//    {
//        new_uniform_blocks();
//    }
//
//    void RenderCommand::del_uniform_blocks()
//    {
//        delete instance_uniforms;
//        delete light_uniforms;
//        instance_uniforms = nullptr;
//        light_uniforms = nullptr;
//    }
//
//    void RenderCommand::new_uniform_blocks()
//    {
//        instance_uniforms = new InstanceUniformBlock();
//        light_uniforms = new LightUniformBlock();
//    }



    Renderer::Renderer(Graphics *graphics, ResourceCache *cache)
        : m_graphics(graphics)
        , m_cache(cache)
    {
        m_commands.reserve(1000);
        m_global_uniforms = new GlobalUniformBlock();
        m_instance_uniforms = new InstanceUniformBlock();
        m_light_uniforms = new LightUniformBlock();
    }

    Renderer::~Renderer()
    {
        delete m_global_uniforms;
        delete m_instance_uniforms;
        delete m_light_uniforms;
    }

    void Renderer::set_projection_matrix(const Matrix4 &proj_mat)
    { m_global_uniforms->set_projection_mat(proj_mat); }

    void Renderer::set_view_matrix(const Matrix4 &view_mat)
    { m_global_uniforms->set_view_mat(view_mat); }

    void Renderer::set_camera_pos(const Vector3 &camera_pos)
    { m_global_uniforms->set_camera_pos(camera_pos); }

    void Renderer::set_time(float time)
    { m_global_uniforms->set_time(time); }


    void Renderer::set_model_matrix(const Matrix4 &model_mat)
    { m_command.model_mat = model_mat; }


    void Renderer::set_light_count(size_t light_count)
    { m_command.light_count = light_count; }

    void Renderer::set_light_position(size_t light, const Vector3 &pos, LightType type)
    {
        m_command.lights[light].position = pos;
        m_command.lights[light].type = type;
    }

    void Renderer::set_light_color(size_t light, const Vector3 &color, float energy)
    { m_command.lights[light].color_energy = Vector4(color, energy); }

    void Renderer::set_light_radius(size_t light, float radius)
    { m_command.lights[light].radius = radius; }


//    void Renderer::set_model_matrix(const Matrix4 &model_mat)
//    { m_command.instance_uniforms->set_model_mat(model_mat); }
//
//
//    void Renderer::set_light_count(size_t light_count)
//    { m_command.light_uniforms->set_light_count(light_count); }
//
//    void Renderer::set_light_position(size_t light, const Vector3 &pos, LightType type)
//    { m_command.light_uniforms->set_position(light, pos, type); }
//
//    void Renderer::set_light_color(size_t light, const Vector3 &color, float energy)
//    { m_command.light_uniforms->set_color(light, color, energy); }
//
//    void Renderer::set_light_radius(size_t light, float radius)
//    { m_command.light_uniforms->set_radius(light, radius); }


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
            m_commands.push_back(m_command);
//            m_command.new_uniform_blocks();
        }
    }

    void Renderer::render()
    {
        std::sort(m_commands.begin(), m_commands.end(), command_cmp);

        m_global_uniforms->bind_uniforms();

        for (RenderCommand &command : m_commands)
        {
            render_command(command);
//            command.del_uniform_blocks();
        }

//        log::info("% commands rendered", m_commands.size());

        m_commands.clear();
    }

    void Renderer::render_command(RenderCommand &command)
    {
        m_instance_uniforms->set_model_mat(command.model_mat);
        m_instance_uniforms->bind_uniforms();

        m_light_uniforms->set_light_count(command.light_count);
        for (size_t i = 0; i < command.light_count; i++)
        {
            m_light_uniforms->set_position(i, command.lights[i].position, command.lights[i].type);
            m_light_uniforms->set_color(i, command.lights[i].color_energy.xyz(), command.lights[i].color_energy.w);
            m_light_uniforms->set_radius(i, command.lights[i].radius);
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
