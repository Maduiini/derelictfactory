
#ifndef H_DER_APPLICATION_H
#define H_DER_APPLICATION_H

#include "Config.h"
#include "Window.h"
#include "renderer/Graphics.h"
#include "resource/ResourceCache.h"

namespace der
{

    class Scene;
    class SceneRenderer;

    class Application
    {
    public:
        Application();
        ~Application();

        bool init(const char * const title);
        void run();

        bool is_ready() const;

    private:
        bool init_scene();
        void render();

    private:
        Config m_config;
        Window m_window;
        Graphics m_graphics;
        ResourceCache m_resource_cache;

        Scene *m_scene;
        SceneRenderer *m_scene_renderer;

        bool m_glfw_ready;
        bool m_ready;
    };

} // der

#endif // H_DER_APPLICATION_H

