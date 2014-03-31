
#include "Application.h"

int main(int argc, const char *argv[])
{
    der::Application app;
    app.init("derelict");
    app.run();
    return 0;

//    ::GLFWwindow *window = nullptr;

//    if (!::glfwInit())
//        return -1;
//
//    ::glfwSetErrorCallback(&error_cb);

//    der::Window window;
//    if (window.create(config.m_screen_w, config.m_screen_h, "derelict"))
//    {
//        while (!window.should_close())
//        {
//            window.swap_buffer();
//            window.poll_events();
//        }
//    }

//    window = ::glfwCreateWindow(config.m_screen_w,
//                                config.m_screen_h,
//                                "derelict",
//                                nullptr, nullptr);
//    if (!window)
//    {
//        ::glfwTerminate();
//        return -1;
//    }
//
//    ::glfwMakeContextCurrent(window);
//
//    ::glfwSwapInterval(config.m_v_sync ? 1 : 0);
//
//    while (!::glfwWindowShouldClose(window))
//    {
//        ::glfwSwapBuffers(window);
//        ::glfwPollEvents();
//    }

//    ::glfwTerminate();
//    return 0;
}
