
#include "SceneUpdateServer.h"
#include "../Log.h"

namespace der
{

    SceneUpdateServer::SceneUpdateServer()
        : UDPServer(30001, 1024)
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
    {

    }

    void SceneUpdateServer::message_received(const char* buffer)
    {
        for (size_t i=0; i<m_buffer_size && buffer[i] != '\0'; i++)
        {
            log::debug("Network command: % (%)", buffer[i], (unsigned int)buffer[i]);
        }
    }

} // der
