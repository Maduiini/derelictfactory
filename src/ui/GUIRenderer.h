
#ifndef H_DER_GUI_RENDERER_H
#define H_DER_GUI_RENDERER_H

#include "../resource/Resource.h"

namespace der
{

    class Graphics;
    class ResourceCache;
    class VertexArrayObject;
    class VertexBuffer;
    class GUIManager;
    class Window;

    class GUIRenderer
    {
    public:
        explicit GUIRenderer(GUIManager *gui, Window *window);
        ~GUIRenderer();

        void render(Graphics *graphics, ResourceCache &cache);

    private:
        static void build();

    private:
        GUIManager *m_gui;
        Window *m_window;
        static ResourceID m_vert_shader;
        static ResourceID m_frag_shader;
        static VertexArrayObject *m_vao;
        static VertexBuffer *m_vbuffer;

        static ResourceID m_button_tex;

    };

} // der

#endif // H_DER_GUI_RENDERER_H
