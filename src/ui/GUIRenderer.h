
#ifndef H_DER_GUI_RENDERER_H
#define H_DER_GUI_RENDERER_H

#include "../math/Vector.h"

#include "../resource/Resource.h"

#include "../ui/Widget.h"

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

        void set_visible(const bool visible);
        bool is_visible() const;

    private:
        void render_widget(Graphics *graphics, ResourceCache &cache, WidgetRenderCommand &cmd);
        void update_window_size();

        static void build();

    private:
        GUIManager *m_gui;
        Window *m_window;
        bool m_visible;
        float m_win_width;
        float m_win_height;

        static ResourceID m_vert_shader;
        static ResourceID m_frag_shader;
        static VertexArrayObject *m_vao;
        static VertexBuffer *m_vbuffer;
    };

} // der

#endif // H_DER_GUI_RENDERER_H
