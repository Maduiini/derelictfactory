
#ifndef H_DER_SCENE_RENDERER_H
#define H_DER_SCENE_RENDERER_H

namespace der
{

    struct Vector3;

    class ResourceCache;

    class Scene;

    class Graphics;
    class GlobalUniformBlock;
    class InstanceUniformBlock;
    class LightUniformBlock;

    class SceneRenderer
    {
    public:
        explicit SceneRenderer(Scene *scene);
        ~SceneRenderer();

        void render(Graphics *graphics, ResourceCache &cache);

        void set_time(float time);

    private:
        void set_lights(const Vector3 &position);

    private:
        Scene *m_scene;
        GlobalUniformBlock *    m_global_uniforms;
        InstanceUniformBlock *  m_instance_uniforms;
        LightUniformBlock *     m_light_uniforms;
    };

} // der

#endif // H_DER_SCENE_RENDERER_H

