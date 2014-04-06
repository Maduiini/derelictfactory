
#ifndef H_DER_WINDOW_H
#define H_DER_WINDOW_H

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

        void poll_events() const;

        bool should_close() const;

        void make_current();
        void swap_buffer();
        void set_v_sync(bool enabled);
//        bool has_v_sync() const;

        bool has_resized();

    private:
        friend void on_window_resize(GLFWwindow*, int, int);

        void on_resize(int w, int h);

    private:
        GLFWwindow *    m_window;
        GLFWmonitor *   m_monitor;
        bool            m_has_resized;
    };

} // der

#endif // H_DER_WINDOW_H

