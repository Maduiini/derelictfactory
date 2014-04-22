
#ifndef H_DER_SCENE_RENDERER_H
#define H_DER_SCENE_RENDERER_H

namespace der
{

    class Scene;

    class SceneRenderer
    {
    public:
        explicit SceneRenderer(Scene *scene);

        void render();

    private:
        Scene *m_scene;
    };

} // der

#endif // H_DER_SCENE_RENDERER_H

