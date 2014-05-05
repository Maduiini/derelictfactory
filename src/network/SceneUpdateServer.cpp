
#include "SceneUpdateServer.h"
#include "../scene/Scene.h"
#include "../resource/SceneLoader.h"
#include "../Log.h"

namespace der
{

    SceneUpdateServer::SceneUpdateServer()
        : UDPServer(30001, 1024)
        , m_scene(nullptr)
        , m_cache(nullptr)
    {
        if (!initialize())
        {
            log::warning("Failed to initialize scene update server.");
        }
        else
        {
            log::info("Scene update server running at port %.", m_port);
        }
    }

    SceneUpdateServer::~SceneUpdateServer()
    { }

    void SceneUpdateServer::message_received(const char* buffer)
    {
        for (size_t i=0; i<m_buffer_size && buffer[i] != '\0'; i++)
        {
            char cmd = buffer[i];
            log::debug("Network command: % (%)", cmd, (unsigned int)cmd);

            if (cmd == 'R') reload();
        }
    }

    void SceneUpdateServer::set_scene(Scene *scene)
    { m_scene = scene; }

    void SceneUpdateServer::set_cache(ResourceCache *cache)
    { m_cache = cache; }

    void SceneUpdateServer::reload()
    {
        SceneLoader loader(*m_cache);
        GameObject *camera_obj = m_scene->get_camera_object();
        m_scene->detach_object(camera_obj->getID());
        loader.load(m_scene->get_source().c_str(), m_scene);
        m_scene->add_object(camera_obj);
        m_scene->set_camera_object(camera_obj->getID());
    }

} // der
