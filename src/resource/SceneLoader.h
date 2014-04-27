
#ifndef H_DER_SCENE_LOADER_H
#define H_DER_SCENE_LOADER_H

#include <string>

namespace der
{

    class ResourceCache;
    class Scene;

    class SceneLoader
    {
    public:
        SceneLoader(ResourceCache &resource_cache);

        bool load(const char *scene_name, Scene *scene);

    private:
        ResourceCache &m_resource_cache;
        std::string m_resource_dir;
    };

} // der

#endif // H_DER_SCENE_LOADER_H

