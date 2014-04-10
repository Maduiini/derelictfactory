
#include "Application.h"
#include "Log.h"
#include "Debug.h"

//// Included just for log testing
//#include "math/Math.h"
#include "renderer/Mesh.h"
#include "renderer/MeshRenderer.h"
#include "resource/ResourceCache.h"
#include "resource/MeshCache.h"

#include "renderer/Shader.h"

#include "scene/Camera.h"

#include <GLFW/glfw3.h>

namespace der
{

    // Some globals for testing purpses
    Mesh *g_logo = nullptr;
    MeshRenderer* g_mesh_renderer = nullptr;
    Program *g_program = nullptr;

    Texture *g_texture;

    int g_proj_loc = 0;
    int g_view_loc = 0;
    int g_model_loc = 0;
    int g_tex_loc = 0;

    Matrix4 g_view_mat;
    Matrix4 g_model_mat;

    Camera g_camera;


    void glfw_error_callback(int err, const char * const msg)
    {
        log::error("GLFW: %: %", err, msg);
    }


    Application::Application()
        : m_config()
        , m_window()
        , m_resource_cache()
        , m_glfw_ready(false)
        , m_ready(false)
    {
//        m_config.read("config.conf");

        ::glfwSetErrorCallback(&glfw_error_callback);
        m_glfw_ready = (::glfwInit() == GL_TRUE);
        if (!m_glfw_ready)
        {
            log::fatal("Could not initialize GLFW");
        }
    }

    Application::~Application()
    {
        if (m_ready)        m_window.destroy();
        if (m_glfw_ready)   ::glfwTerminate();
    }

    bool Application::init(const char * const title)
    {
        if (m_glfw_ready)
        {
            m_ready = m_window.create(m_config.m_screen_w,
                                      m_config.m_screen_h,
                                      title,
                                      m_config.m_fullscreen);

            if (m_ready)
            {
                m_window.make_current();
                m_ready = m_graphics.init();
                m_window.set_v_sync(m_config.m_v_sync);
            }
        }
        return is_ready();
    }

    void Application::run()
    {
        if (!is_ready()) return;

        const ResourceID logo_id = make_resource("logo.obj");
        g_logo = m_resource_cache.get<Mesh>(logo_id);
        log::info("Logo loaded: %", (g_logo != nullptr) ? "yes" : "no");

        g_mesh_renderer = new MeshRenderer();
        g_mesh_renderer->set_mesh(g_logo);

        const ResourceID asphalt_id = make_resource("asphalt.tga");
        g_texture = m_resource_cache.get<Texture2D>(asphalt_id);
        log::info("Asphalt texture loaded: %", (g_texture != nullptr) ? "yes" : "no");

        g_program = new Program();

        Shader *v_shader = new Shader(Shader::TYPE_Vert);
        Shader *f_shader = new Shader(Shader::TYPE_Frag);

        #define GLSL(src) "#version 330\n" #src

        const char * vert_src = GLSL(
            uniform mat4 mat_proj;
            uniform mat4 mat_view;
            uniform mat4 mat_model;
            in vec3 in_position;
            in vec2 in_tex_coord;
            in vec3 in_normal;
            in vec4 in_tangent;

            out vec3 position;
            out vec3 normal;
            out vec2 tcoord;
            void main()
            {
                mat4 mvp = mat_proj * mat_view * mat_model;
                vec4 pos = mvp * vec4(in_position, 1.0);

                position = pos.xyz;
                normal = in_normal;
                tcoord = in_tex_coord;

                gl_Position = pos;
            }
        );
        const char * frag_src = GLSL(
            uniform sampler2D tex_color;
            in vec3 position;
            in vec3 normal;
            in vec2 tcoord;
            void main()
            {
                vec4 color = texture2D(tex_color, tcoord);

                vec3 no = normalize(normal);
                gl_FragData[0] = mix(vec4(abs(no), 1.0), color, 0.9);
            }
        );

        #undef GLSL

        if (!v_shader->compile(vert_src))
        {
            log::error("Error compiling vertex shader");
            return;
        }
        if (!f_shader->compile(frag_src))
        {
            log::error("Error compiling fragment shader");
            return;
        }

        g_program->attach(v_shader);
        g_program->attach(f_shader);
        if (!g_program->link())
        {
            log::error("Error linking shader program");
            return;
        }
        g_proj_loc = g_program->get_uniform_location("mat_proj");
        g_view_loc = g_program->get_uniform_location("mat_view");
        g_model_loc = g_program->get_uniform_location("mat_model");
        g_tex_loc = g_program->get_uniform_location("tex_color");


        float fov0 = 45.0f;
        float t = 0.0f;

        g_view_mat = Matrix4::identity;

        while (!m_window.should_close())
        {
            float fov = fov0 + std::sin(t) * 20.0f;
            t += 0.016f * 1e-1;
            g_camera.set_fov(fov);

            g_model_mat.translation(0.0f, 0.0f, 5.0f);
            Matrix4 rot, rot_x90;
            Vector3 axis(0.0f, 1.0f, 0.15f);
            axis.normalize();
            rot.rotation_from_axis_angle(axis, t);
            rot_x90.rotation_x(Math::PI * 0.5f);
            g_model_mat = g_model_mat * rot * rot_x90;

            render();

            m_window.poll_events();
            if (m_window.has_resized())
            {
                int w = 0, h = 0;
                m_window.get_size(&w, &h);
                m_graphics.set_viewport(0, 0, w, h);
                g_camera.set_aspect_ratio(float(w) / h);
            }
        }
    }

    bool Application::is_ready() const
    { return m_glfw_ready && m_ready; }

    void Application::render()
    {
        m_graphics.clear();

        g_program->use();
        g_program->uniform(g_proj_loc, g_camera.get_projection());
        g_program->uniform(g_view_loc, g_view_mat);
        g_program->uniform(g_model_loc, g_model_mat);

        m_graphics.set_texture(0, g_texture);
        g_program->uniform_sampler2D(g_tex_loc, 0);

        g_mesh_renderer->render();

        m_window.swap_buffer();
    }


} // der
