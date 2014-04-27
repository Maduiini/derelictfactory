
#ifndef H_DER_SCENE_RENDERER_H
#define H_DER_SCENE_RENDERER_H

namespace der
{

    class Scene;

    class Graphics;
    class GlobalUniformBlock;

    class SceneRenderer
    {
    public:
        explicit SceneRenderer(Scene *scene);

        void render(Graphics *graphics);

    private:
        Scene *m_scene;
        GlobalUniformBlock *m_global_uniforms;
    };

} // der

#endif // H_DER_SCENE_RENDERER_H

