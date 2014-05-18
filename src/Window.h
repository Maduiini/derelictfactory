
#ifndef H_DER_WINDOW_H
#define H_DER_WINDOW_H

#include "Input.h"
#include "Types.h"

struct GLFWwindow;
struct GLFWmonitor;

namespace der
{

    class Window
    {
    public:
        Window();
        ~Window();

        bool create(int w, int h, const char * const title, bool fullscreen = false);
        void destroy();

        void set_title(const char * const title);

        void set_size(int w, int h);
        void get_size(int *w, int *h) const;

//        void set_fullscreen(bool fullscreen);
        bool is_fullscreen() const;

        void poll_events();

        bool should_close() const;

        void make_current();
        void swap_buffer();
        void set_v_sync(bool enabled);
//        bool has_v_sync() const;

        bool has_resized();

        bool key_pressed(Key key) const;
        bool key_down(Key key) const;
        bool button_down(MouseButton button) const;
        float get_mouse_x() const;
        float get_mouse_y() const;
        float get_mouse_dx() const;
        float get_mouse_dy() const;
        float get_delta_scroll() const;

        void set_mouse_captured(bool captured);
        void toggle_mouse_captured();
        bool is_mouse_captured() const;

    private:
        friend void on_window_resize(GLFWwindow*, int, int);
        friend void on_window_key(GLFWwindow*, int, int, int, int);
        friend void on_window_mouse_button(GLFWwindow*, int, int, int);
        friend void on_window_mouse_move(GLFWwindow*, double, double);
        friend void on_window_scroll(GLFWwindow*, double, double);

        void on_resize(int w, int h);
        void on_key(int key, int scancode, int action, int mods);
        void on_mouse_button(int button, int action, int mods);
        void on_mouse_move(double x, double y);
        void on_scroll(double delta);

    private:
        GLFWwindow *    m_window;
        GLFWmonitor *   m_monitor;
        bool            m_has_resized;

        static const size_t KEY_COUNT = 348; // The last GLFW_KEY_* is 348
        int m_keys[KEY_COUNT];

        static const size_t BUTTON_COUNT = 8;
        int m_mouse_button[BUTTON_COUNT];
        float m_mouse_x;
        float m_mouse_y;
        float m_mouse_dx;
        float m_mouse_dy;
        float m_delta_scroll;

        bool m_mouse_captured;
    };

} // der

#endif // H_DER_WINDOW_H

