
#include "Application.h"
#include "Log.h"
#include "Debug.h"

//// Included just for log testing
//#include "math/Math.h"
#include "renderer/Mesh.h"
#include "resource/ResourceCache.h"
#include "resource/MeshCache.h"

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
        , m_glfw_ready(false)
        , m_ready(false)
    {
//        m_config.read("config.conf");


        m_resource_cache.set_directory(ResourceType::Model, "assets\\models");
        m_resource_cache.scan_directories();

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
        Mesh *logo = m_resource_cache.get<Mesh>(logo_id); // <-- onko mahdollista toteuttaa tuollainen funktio?
        log::info("Logo loaded: %", (logo != nullptr) ? "yes" : "no");

        while (!m_window.should_close())
        {
            render();

            m_window.poll_events();
            if (m_window.has_resized())
            {
                int w = 0, h = 0;
                m_window.get_size(&w, &h);
                m_graphics.set_viewport(0, 0, w, h);
            }
        }
    }

    bool Application::is_ready() const
    { return m_glfw_ready && m_ready; }

    void Application::render()
    {
        m_graphics.clear();

        m_window.swap_buffer();
    }


} // der
