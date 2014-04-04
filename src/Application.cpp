
#include "Application.h"
#include "Log.h"

// Included just for log testing
#include "math/Math.h"

#include <GLFW/glfw3.h>

namespace der
{

    Application::Application()
        : m_config()
        , m_window()
        , m_resource_cache()
        , m_glfw_ready(false)
        , m_ready(false)
    {
//        m_config.read("config.conf");


        m_resource_cache.scan_directories();

        log::fatal("The matrix I_3 = \n%", Matrix3::identity);
        log::info("The matrix I_3x4 = \n%", Matrix3x4::identity);
        log::debug("The vector e_1 = %", Vector3::unit_x);
        log::warning("Quaternion = %", Quaternion::identity);

        m_glfw_ready = (::glfwInit() == GL_TRUE);
        if (!m_glfw_ready)
            log::fatal("Could not initialize GLFW");
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

            m_window.set_v_sync(m_config.m_v_sync);
        }
        return is_ready();
    }

    void Application::run()
    {
        if (!is_ready()) return;

        while (!m_window.should_close())
        {
            m_window.swap_buffer();
            m_window.poll_events();
        }
    }

    bool Application::is_ready() const
    { return m_glfw_ready && m_ready; }


} // der
