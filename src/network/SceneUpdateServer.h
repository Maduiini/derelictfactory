
#ifndef H_DER_SCENE_UPDATE_SERVER_H
#define H_DER_SCENE_UPDATE_SERVER_H

#include "UDPServer.h"

namespace der
{

    class SceneUpdateServer : public UDPServer
    {
    public:
        SceneUpdateServer();
        ~SceneUpdateServer();

        virtual void message_received(const char* buffer) override;

    };

} // der

#endif // H_DER_SCENE_UPDATE_SERVER_H
