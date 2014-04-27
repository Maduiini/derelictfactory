
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
#include "renderer/SceneRenderer.h"

#include "scene/Scene.h"
#include "scene/GameObject.h"
#include "scene/Camera.h"

#include "input/CameraController.h"

#include <GLFW/glfw3.h>

namespace der
{

    // Some globals for testing purpses
    Program *g_program = nullptr;
    Texture *g_texture = 0, *g_normal_map = 0;

    int g_tex_loc = 0;
    int g_nor_loc = 0;

    CameraController g_camera_controller;


    void glfw_error_callback(int err, const char * const msg)
    {
        log::error("GLFW: %: %", err, msg);
    }


    Application::Application()
        : m_config()
        , m_window()
        , m_resource_cache()
        , m_scene(nullptr)
        , m_scene_renderer(nullptr)
        , m_scene_loader(m_resource_cache)
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
        delete m_scene_renderer;
        delete m_scene;

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

        if (!init_scene())
        {
            log::error("Could not initialize scene");
            return;
        }


        double last_time = ::glfwGetTime();

        while (!m_window.should_close())
        {
            double cur_time = ::glfwGetTime();
            double delta_time = cur_time - last_time;
            last_time = cur_time;

            g_camera_controller.update(delta_time);

            render();

            m_window.poll_events();
            if (m_window.has_resized())
            {
                int w = 0, h = 0;
                m_window.get_size(&w, &h);
                m_graphics.set_viewport(0, 0, w, h);
                m_scene->reshape(w, h);
            }
        }
    }

    bool Application::is_ready() const
    { return m_glfw_ready && m_ready; }

    bool Application::init_scene()
    {
        m_scene = new Scene();
        m_scene_renderer = new SceneRenderer(m_scene);

//        GameObject *camera_object = m_scene->new_object();
//        camera_object->set_camera(new Camera());
//        m_scene->set_camera_object(camera_object->getID());
//
//        g_camera_controller.set_window(&m_window);
//        g_camera_controller.set_object(camera_object);
//
//        const ResourceID logo_id = make_resource("logo_smooth.obj");
//        Mesh *logo_mesh = m_resource_cache.get<Mesh>(logo_id);
//        log::info("Logo loaded: %", (logo_mesh != nullptr) ? "yes" : "no");
//
//        GameObject *logo = m_scene->new_object();
//        MeshRenderer *renderer = new MeshRenderer();
//        renderer->set_mesh(logo_mesh);
//        logo->set_renderer(renderer);
//        logo->set_position(Vector3(0.0f, 0.0f, 5.0f));

        const ResourceID asphalt_id = make_resource("asphalt.tga");
        g_texture = m_resource_cache.get<Texture2D>(asphalt_id);
        log::info("Asphalt texture loaded: %", (g_texture != nullptr) ? "yes" : "no");
        const ResourceID asphalt_n_id = make_resource("asphalt_n.tga");
        g_normal_map = m_resource_cache.get<Texture2D>(asphalt_n_id);
        log::info("Asphalt normal map loaded: %", (g_normal_map != nullptr) ? "yes" : "no");

        const ResourceID test_vert_id = make_resource("test.vert");
        const ResourceID test_frag_id = make_resource("test.frag");
        g_program = m_resource_cache.get_program(test_vert_id, test_frag_id);

        g_tex_loc = g_program->get_uniform_location("tex_color");
        g_nor_loc = g_program->get_uniform_location("tex_normal");

        // Load the test scene
        if (m_scene_loader.load("test_scene.derscene", m_scene))
            log::info("Scene loaded");
        else
            log::error("Could not load scene");

        // Create camera
        GameObject *camera_object = m_scene->new_object();
        camera_object->set_camera(new Camera());
        camera_object->set_position(Vector3(0.0f, 1.8f, 0.0f));
        m_scene->set_camera_object(camera_object->getID());

        g_camera_controller.set_window(&m_window);
        g_camera_controller.set_object(camera_object);

        return true;
    }

    void Application::render()
    {
        m_graphics.clear();

        g_program->use();

        m_graphics.set_texture(0, g_texture);
        g_program->uniform_sampler2D(g_tex_loc, 0);
        m_graphics.set_texture(1, g_normal_map);
        g_program->uniform_sampler2D(g_nor_loc, 1);

        m_scene_renderer->render(&m_graphics);

        m_window.swap_buffer();
    }


} // der
