
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
        , m_current_controller(nullptr)
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
        delete m_current_controller;
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

            if (m_current_controller)
                m_current_controller->update(delta_time);

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

        m_current_controller = new CameraController();
        m_current_controller->set_window(&m_window);
        m_current_controller->set_object(camera_object);

        return true;
    }

    void Application::render()
    {
        m_graphics.clear();

        m_scene_renderer->render(&m_graphics, m_resource_cache);

        m_window.swap_buffer();
    }


} // der
