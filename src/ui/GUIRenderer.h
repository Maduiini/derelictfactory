
#ifndef H_DER_GUI_RENDERER_H
#define H_DER_GUI_RENDERER_H

#include "../math/Vector.h"

#include "../resource/Resource.h"

#include "../ui/Widget.h"
#include "../ui/BitmapFont.h"

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

        bool init();

        void render(Graphics *graphics, ResourceCache &cache);

        void set_visible(const bool visible);
        bool is_visible() const;

    private:
        void render_quad(Graphics *graphics, ResourceCache &cache, const Vector2 &position, const Vector2 &scale, const ResourceID texture_id, const Vector4 uv = {0.0,0.0,1.0,1.0});
        void render_text(Graphics *graphics, ResourceCache &cache, const Vector2 &position, const char * const buffer);
        void render_widget(Graphics *graphics, ResourceCache &cache, WidgetRenderCommand &cmd);
        void update_window_size();

        void init_buffers();
        void update_buffers(const Vector4 &uv);

    private:
        GUIManager *m_gui;
        Window *m_window;

        BitmapFont m_font;
        bool m_visible;
        float m_win_width;
        float m_win_height;
        const float m_font_texture_size;

        VertexArrayObject *m_vao;
        VertexBuffer *m_vbuffer;

        static ResourceID m_font_texture;
        static ResourceID m_vert_shader;
        static ResourceID m_frag_shader;
    };

} // der

#endif // H_DER_GUI_RENDERER_H
