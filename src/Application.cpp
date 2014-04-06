
#include "Application.h"
#include "Log.h"
#include "Debug.h"

// Included just for log testing
#include "math/Math.h"

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


//        m_resource_cache.scan_directories();

//        log::fatal("The matrix I_3 = \n%", Matrix3::identity);
//        log::info("The matrix I_3x4 = \n%", Matrix3x4::identity);
//        log::debug("The vector e_1 = %", Vector3::unit_x);
//        log::warning("Quaternion = %", Quaternion::identity);

        DER_ASSERT(1 == 0);
        DER_ASSERT_EQ(1, 0);

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

            // NOTE: Does not work correclty. Context needs to be current before setting vsync, but
            // making context current at this stage results in program exit.
//            m_window.make_current();
//            m_window.set_v_sync(m_config.m_v_sync);

            if (m_ready)
            {
                m_ready = m_graphics.init();
            }
        }
        return is_ready();
    }

    void Application::run()
    {
        if (!is_ready()) return;

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
        m_window.make_current();
        m_graphics.clear();

        m_window.swap_buffer();
    }


} // der
