
#include "Window.h"
#include "Log.h"

#include <GLFW/glfw3.h>

namespace der
{

    void on_window_resize(GLFWwindow *window, int w, int h)
    {
        Window *wnd = static_cast<Window*>(::glfwGetWindowUserPointer(window));
        wnd->on_resize(w, h);
    }

    void on_window_key(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        Window *wnd = static_cast<Window*>(::glfwGetWindowUserPointer(window));
        wnd->on_key(key, scancode, action, mods);
    }

    void on_window_mouse_button(GLFWwindow *window, int button, int action, int mods)
    {
        Window *wnd = static_cast<Window*>(::glfwGetWindowUserPointer(window));
        wnd->on_mouse_button(button, action, mods);
    }

    void on_window_mouse_move(GLFWwindow *window, double x, double y)
    {
        Window *wnd = static_cast<Window*>(::glfwGetWindowUserPointer(window));
        wnd->on_mouse_move(x, y);
    }


    Window::Window()
        : m_window(nullptr)
        , m_monitor(nullptr)
        , m_has_resized(true)
        , m_mouse_x(0.0f)
        , m_mouse_y(0.0f)
        , m_mouse_dx(0.0f)
        , m_mouse_dy(0.0f)
        , m_mouse_captured(false)
    {
        for (size_t i = 0; i < KEY_COUNT; i++) m_keys[i] = GLFW_RELEASE;
        for (size_t i = 0; i < BUTTON_COUNT; i++) m_mouse_button[i] = GLFW_RELEASE;
    }

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

        ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        //::glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        //::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef DER_DEBUG
        ::glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        #endif // DER_DEBUG

        m_window = ::glfwCreateWindow(w, h, title, m_monitor, nullptr);
        if (!m_window)
        {
            log::error("Could not create window");
            return false;
        }

        ::glfwSetWindowUserPointer(m_window, static_cast<void*>(this));

        ::glfwSetWindowSizeCallback(m_window, &on_window_resize);
        ::glfwSetKeyCallback(m_window, &on_window_key);
        ::glfwSetMouseButtonCallback(m_window, &on_window_mouse_button);
        ::glfwSetCursorPosCallback(m_window, &on_window_mouse_move);

        int major = ::glfwGetWindowAttrib(m_window, GLFW_CONTEXT_VERSION_MAJOR);
        int minor = ::glfwGetWindowAttrib(m_window, GLFW_CONTEXT_VERSION_MINOR);
        int debug = ::glfwGetWindowAttrib(m_window, GLFW_OPENGL_DEBUG_CONTEXT);

        log::info("Window created with resolution %x% %", w, h, is_fullscreen() ? "fullscreen" : "windowed");
        log::info("OpenGL context: %.% (%)", major, minor, debug ? "debug" : "no debug");

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

    void Window::poll_events()
    {
        m_mouse_dx = 0;
        m_mouse_dy = 0;
        for (size_t i = 0; i < KEY_COUNT; i++)
            if (m_keys[i] == GLFW_PRESS) m_keys[i] = GLFW_REPEAT;

        ::glfwPollEvents();
    }

    bool Window::should_close() const
    {
        return ::glfwWindowShouldClose(m_window);
    }

    void Window::make_current()
    {
        if (::glfwGetCurrentContext() != m_window)
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

    bool Window::has_resized()
    {
        bool resized = m_has_resized;
        m_has_resized = false;
        return resized;
    }

    bool Window::key_pressed(Key key) const
    { return m_keys[static_cast<int>(key)] == GLFW_PRESS; }

    bool Window::key_down(Key key) const
    { return m_keys[static_cast<int>(key)] != GLFW_RELEASE; }

    bool Window::button_down(MouseButton button) const
    { return m_mouse_button[static_cast<int>(button)] == GLFW_PRESS; }

    float Window::get_mouse_x() const
    { return m_mouse_x; }

    float Window::get_mouse_y() const
    { return m_mouse_y; }

    float Window::get_mouse_dx() const
    { return m_mouse_dx; }

    float Window::get_mouse_dy() const
    { return m_mouse_dy; }


    void Window::set_mouse_captured(bool captured)
    {
        m_mouse_captured = captured;

        const int value = m_mouse_captured ?
            GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL;
        ::glfwSetInputMode(m_window, GLFW_CURSOR, value);
    }

    void Window::toggle_mouse_captured()
    { set_mouse_captured(!m_mouse_captured); }

    bool Window::is_mouse_captured() const
    { return m_mouse_captured; }



    void Window::on_resize(int w, int h)
    { m_has_resized = true; }

    void Window::on_key(int key, int scancode, int action, int mods)
    {
        if (size_t(key) < KEY_COUNT)
        {
            m_keys[key] = action;
        }
    }

    void Window::on_mouse_button(int button, int action, int mods)
    {
        m_mouse_button[button] = action;
    }

    void Window::on_mouse_move(double x, double y)
    {
        float last_x = m_mouse_x;
        float last_y = m_mouse_y;
        m_mouse_x = x;
        m_mouse_y = y;
        m_mouse_dx = m_mouse_x - last_x;
        m_mouse_dy = m_mouse_y - last_y;

        if (m_mouse_captured)
        {
            int w, h;
            get_size(&w, &h);
            const double xpos = 0.5 * w;
            const double ypos = 0.5 * h;
            ::glfwSetCursorPos(m_window, xpos, ypos);
            m_mouse_x = xpos;
            m_mouse_y = ypos;
        }
    }

} // der
