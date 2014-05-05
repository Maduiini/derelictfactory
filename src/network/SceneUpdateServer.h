
#ifndef H_DER_SCENE_UPDATE_SERVER_H
#define H_DER_SCENE_UPDATE_SERVER_H

#include "UDPServer.h"
#include <string>

namespace der
{

    class Scene;
    class ResourceCache;

    class SceneUpdateServer : public UDPServer
    {
    public:
        SceneUpdateServer();
        ~SceneUpdateServer();

        virtual void message_received(const char* buffer) override;

        void set_scene(Scene *scene);
        void set_cache(ResourceCache *cache);

    private:
        void reload();

    private:
        Scene *m_scene;
        ResourceCache *m_cache;
    };

} // der

#endif // H_DER_SCENE_UPDATE_SERVER_H
