
#ifndef H_DER_SCENE_RENDERER_H
#define H_DER_SCENE_RENDERER_H

namespace der
{

    class ResourceCache;

    class Scene;

    class Graphics;
    class GlobalUniformBlock;
    class InstanceUniformBlock;

    class SceneRenderer
    {
    public:
        explicit SceneRenderer(Scene *scene);
        ~SceneRenderer();

        void render(Graphics *graphics, ResourceCache &cache);

    private:
        Scene *m_scene;
        GlobalUniformBlock *    m_global_uniforms;
        InstanceUniformBlock *  m_instance_uniforms;
    };

} // der

#endif // H_DER_SCENE_RENDERER_H

