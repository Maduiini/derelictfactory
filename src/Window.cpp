
#include "Window.h"
#include "Log.h"

#include <GLFW/glfw3.h>

namespace der
{

    Window::Window()
        : m_window(nullptr)
        , m_monitor(nullptr)
    { }

    Window::~Window()
    {
        destroy();
    }

    bool Window::create(int w, int h, const char * const title, bool fullscreen /*= false*/)
    {
        if (fullscreen)
        {
            m_monitor = ::glfwGetPrimaryMonitor();
            if (!m_monitor)
            {
                log::error("Could not get the primary monitor for fullscreen mode");
                return false;
            }
        }
        else
        {
            m_monitor = nullptr;
        }

        m_window = ::glfwCreateWindow(w, h, title, m_monitor, nullptr);
        if (!m_window)
        {
            log::error("Could not create window");
            return false;
        }

        log::info("Window created with resolution %x% %", w, h, is_fullscreen() ? "fullscreen" : "windowed");

        return true;
    }

    void Window::destroy()
    {
        if (m_window)
        {
            ::glfwDestroyWindow(m_window);
            m_window = nullptr;
            m_monitor = nullptr;
        }
    }

    void Window::set_title(const char * const title)
    {
        ::glfwSetWindowTitle(m_window, title);
    }

    void Window::set_size(int w, int h)
    {
        ::glfwSetWindowSize(m_window, w, h);
    }

    void Window::get_size(int *w, int *h) const
    {
        ::glfwGetWindowSize(m_window, w, h);
    }

//    void Window::set_fullscreen(bool fullscreen)
//    { }

    bool Window::is_fullscreen() const
    {
        return m_monitor != nullptr;
    }

    void Window::poll_events() const
    {
        ::glfwPollEvents();
    }

    bool Window::should_close() const
    {
        return ::glfwWindowShouldClose(m_window);
    }

    void Window::make_current()
    {
        ::glfwMakeContextCurrent(m_window);
    }

    void Window::swap_buffer()
    {
        ::glfwSwapBuffers(m_window);
    }

    void Window::set_v_sync(bool enabled)
    {
        ::glfwSwapInterval(enabled ? 1 : 0);
        log::info("V-Sync %", enabled ? "enabled" : "disabled");
    }

//    bool Window::has_v_sync() const
//    { }

} // der
