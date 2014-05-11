
#include "Application.h"
#include "Log.h"
#include "Debug.h"

#include "renderer/Mesh.h"
#include "renderer/MeshRenderer.h"
#include "resource/ResourceCache.h"
#include "resource/MeshCache.h"

#include "renderer/Renderer.h"
#include "renderer/SceneRenderer.h"

#include "scene/Scene.h"
#include "scene/GameObject.h"
#include "scene/Camera.h"

#include "input/CameraController.h"

#include "ui/GUIManager.h"
#include "ui/GUIRenderer.h"
#include "ui/Button.h"
#include "ui/Slider.h"
#include "ui/Checkbox.h"
#include "ui/Label.h"

#include <GLFW/glfw3.h>
#include <sstream>

namespace der
{

    // NOTE: The c++11 value to string conversion functionality
    // std::to_string does not work in MinGW (at least <4.8.1).
    template <class T>
    std::string to_string(const T value)
    {
        std::ostringstream out;
        out << value;
        return out.str();
    }


    void glfw_error_callback(int err, const char * const msg)
    {
        log::error("GLFW: %: %", err, msg);
    }


    Application::Application()
        : m_config()
        , m_window()
        , m_resource_cache()
        , m_scene(nullptr)
        , m_renderer(nullptr)
        , m_scene_renderer(nullptr)
        , m_scene_loader(m_resource_cache)
        , m_current_controller(nullptr)
        , m_scene_update_server()
        , m_glfw_ready(false)
        , m_ready(false)
        , m_queued_render(true)
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
        delete m_renderer;
        delete m_scene_renderer;
        delete m_scene;
        delete m_gui_renderer;
        delete m_gui;

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

                m_renderer = new Renderer(&m_graphics, &m_resource_cache);
            }
        }
        return is_ready();
    }

    void Application::run()
    {
        if (!is_ready()) return;

        if (!init_scene())
        {
            log::error("Could not initialize scene.");
            return;
        }

        m_scene_update_server.set_scene(m_scene);
        m_scene_update_server.set_cache(&m_resource_cache);

        if (!init_gui())
        {
            log::error("Could not initialize GUI.");
            return;
        }

        double last_time = ::glfwGetTime();

        while (!m_window.should_close())
        {
            double cur_time = ::glfwGetTime();
            double delta_time = cur_time - last_time;
            last_time = cur_time;

            if (m_window.key_pressed(Key::Escape))
                break;

            if (m_window.key_pressed(Key::F5))
                m_resource_cache.refresh_all();
            if (m_window.key_pressed(Key::F6))
                m_resource_cache.reload_all();
            if (m_window.key_pressed(Key::F1))
                m_queued_render = !m_queued_render;

            if (m_current_controller)
                m_current_controller->update(delta_time);

            m_scene->update(&m_resource_cache);

            m_scene_renderer->set_time(cur_time);
            render();

            update_title(cur_time);

            m_window.poll_events();
            if (m_window.has_resized())
            {
                int w = 0, h = 0;
                m_window.get_size(&w, &h);
                m_graphics.set_viewport(0, 0, w, h);
                m_scene->reshape(w, h);
            }

            m_gui->update(delta_time,
                          Vector2(m_window.get_mouse_x(), m_window.get_mouse_y()),
                          m_window.button_down(MouseButton::Left));
            m_gui_renderer->set_visible(!m_window.is_mouse_captured());

            m_scene_update_server.pump();
        }
    }

    bool Application::is_ready() const
    { return m_glfw_ready && m_ready; }

    bool Application::init_scene()
    {
        m_scene = new Scene();
        m_scene_renderer = new SceneRenderer(m_scene, &m_resource_cache);

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

    bool Application::init_gui()
    {
        m_gui = new GUIManager();
        m_gui_renderer = new GUIRenderer(m_gui, &m_window);

        if (!m_gui_renderer->init())
        {
            log::error("Failed to initialize GUIRenderer.");
            return false;
        }

        m_gui->add_widget(new Button(Vector2(32, 32), Vector2(128, 32), "Hello World"));
        m_gui->add_widget(new Slider(Vector2(15, 200), 150.0f, -20.0f, 20.0));
        m_gui->add_widget(new Checkbox(Vector2(15, 250), "test checkbox"));
        m_gui->add_widget(new Label(Vector2(70, 260), "Testing"));

        return true;
    }

    void Application::render()
    {
        m_graphics.reset_state_changes();
        m_graphics.clear();

        if (m_queued_render)
            m_scene_renderer->render(m_renderer);
        else
            m_scene_renderer->render(&m_graphics, m_resource_cache);

        m_gui_renderer->render(&m_graphics, m_resource_cache);

        m_window.swap_buffer();
    }


    void Application::update_title(double current_time)
    {
        static double last_time = current_time;
        static size_t frames = 0;

        if (current_time - last_time >= 0.5)
        {
            std::string title = "derelict fps: ";

            const double fps = double(frames) / (current_time - last_time);
            title += to_string(fps);
            title += ", state changes: " + to_string(m_graphics.get_state_changes());

            m_window.set_title(title.c_str());

            last_time = current_time;
            frames = 0;
        }
        frames++;
    }


} // der
