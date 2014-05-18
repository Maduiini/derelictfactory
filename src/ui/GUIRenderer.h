
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
    class Texture;

    class GUIRenderer;

    class GUIRendererCommand
    {
    public:
        virtual ~GUIRendererCommand() { }
        virtual void render(GUIRenderer *gui_renderer, Graphics *graphics, ResourceCache &cache) = 0;
    };

    class RenderCachedTexture : public GUIRendererCommand
    {
    public:
        RenderCachedTexture(ResourceID tex_id, Vector2 position, Vector2 size)
            : m_texture_id(tex_id)
            , m_position(position)
            , m_size(size)
        { }

        void set_texture_id(const ResourceID tex_id)
        { m_texture_id = tex_id; }

        void set_position(const Vector2 position)
        { m_position = position; }

        void set_size(const Vector2 size)
        { m_size = size; }

        virtual void render(GUIRenderer *gui_renderer, Graphics *graphics, ResourceCache &cache) override;

    private:
        ResourceID m_texture_id;
        Vector2 m_position;
        Vector2 m_size;
    };

    class RenderTexture : public GUIRendererCommand
    {
    public:
        RenderTexture(Vector2 position, Vector2 size)
            : m_texture(nullptr)
            , m_position(position)
            , m_size(size)
        { }

        void set_texture(Texture *texture)
        { m_texture = texture; }

        void set_position(const Vector2 position)
        { m_position = position; }

        void set_size(const Vector2 size)
        { m_size = size; }

        virtual void render(GUIRenderer *gui_renderer, Graphics *graphics, ResourceCache &cache) override;

    private:
        Texture *m_texture;
        Vector2 m_position;
        Vector2 m_size;
    };


    class RenderText : public GUIRendererCommand
    {
    public:
        RenderText(const char* text, Vector2 text_position)
            : m_text(text)
            , m_text_position(text_position)
        { }

        void set_text(const char* text)
        { m_text = text; }

        void set_text_position(const Vector2 text_position)
        { m_text_position = text_position; }

        virtual void render(GUIRenderer *gui_renderer, Graphics *graphics, ResourceCache &cache) override;

    private:
        const char* m_text;
        Vector2 m_text_position;
    };

    class GUIRenderer
    {
        friend class RenderText;
        friend class RenderCachedTexture;
        friend class RenderTexture;

    public:
        explicit GUIRenderer(GUIManager *gui, Window *window);
        ~GUIRenderer();

        bool init();

        void render(Graphics *graphics, ResourceCache &cache);

        void set_visible(const bool visible);
        bool is_visible() const;

        unsigned int get_text_width(const char * const text) const;

    private:
        void render_quad(Graphics *graphics, ResourceCache &cache, const Vector2 &position, const Vector2 &scale, Texture *texture, const Vector4 uv = {0.0,0.0,1.0,1.0});
        void render_text(Graphics *graphics, ResourceCache &cache, const Vector2 &position, const char * const buffer);
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
        float m_z;
        const float m_font_texture_size;

        VertexArrayObject *m_vao;
        VertexBuffer *m_vbuffer;

        static ResourceID m_font_texture;
        static ResourceID m_vert_shader;
        static ResourceID m_frag_shader;
    };

} // der

#endif // H_DER_GUI_RENDERER_H
