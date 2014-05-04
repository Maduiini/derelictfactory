
#ifndef H_DER_APPLICATION_H
#define H_DER_APPLICATION_H

#include "Config.h"
#include "Window.h"
#include "renderer/Graphics.h"
#include "resource/ResourceCache.h"
#include "resource/SceneLoader.h"
#include "network/SceneUpdateServer.h"

namespace der
{

    class Scene;
    class SceneRenderer;
    class InputController;
    class GUIManager;
    class GUIRenderer;

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
        bool init_gui();
        void render();

    private:
        Config m_config;
        Window m_window;
        Graphics m_graphics;
        ResourceCache m_resource_cache;

        Scene *m_scene;
        SceneRenderer *m_scene_renderer;

        SceneLoader m_scene_loader;

        InputController *m_current_controller;

        SceneUpdateServer m_scene_update_server;

        GUIManager *m_gui;
        GUIRenderer *m_gui_renderer;

        bool m_glfw_ready;
        bool m_ready;
    };

} // der

#endif // H_DER_APPLICATION_H

