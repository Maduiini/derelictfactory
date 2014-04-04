
#ifndef H_DER_APPLICATION_H
#define H_DER_APPLICATION_H

#include "Config.h"
#include "Window.h"
#include "resource/ResourceCache.h"

namespace der
{

    class Application
    {
    public:
        Application();
        ~Application();

        bool init(const char * const title);
        void run();

        bool is_ready() const;

    private:
        Config m_config;
        Window m_window;
        ResourceCache m_resource_cache;

        bool m_glfw_ready;
        bool m_ready;
    };

} // der

#endif // H_DER_APPLICATION_H

