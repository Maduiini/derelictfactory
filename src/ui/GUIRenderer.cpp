
#include "GUIRenderer.h"
#include "GUIManager.h"
#include "Widget.h"
#include "BitmapFontLoader.h"
#include "BitmapFont.h"

#include "../renderer/Graphics.h"
#include "../renderer/VertexBuffer.h"
#include "../renderer/VertexArrayObject.h"
#include "../resource/ResourceCache.h"

#include "../math/Vector.h"

#include "../Window.h"

namespace der
{

namespace gui_renderer_internal
{

    struct Vertex
    {
        Vector2 position;
        Vector2 uv;
    };

} // gui_renderer_internal

    ResourceID GUIRenderer::m_font_texture = make_resource("gauge_fix.tga");
    ResourceID GUIRenderer::m_vert_shader = make_resource("gui.vert");
    ResourceID GUIRenderer::m_frag_shader = make_resource("gui.frag");

    GUIRenderer::GUIRenderer(GUIManager *gui, Window *window)
        : m_gui(gui)
        , m_window(window)
        , m_font()
        , m_visible(true)
        , m_win_width(0.0f)
        , m_win_height(0.0f)
        , m_z(0.0f)
        , m_font_texture_size(256.0f)
        , m_vao(nullptr)
        , m_vbuffer(nullptr)
    {
        m_vbuffer = new VertexBuffer();
        m_vao = new VertexArrayObject();
        init_buffers();
    }

    GUIRenderer::~GUIRenderer()
    {
        delete m_vao;
        delete m_vbuffer;
    }

    bool GUIRenderer::init()
    {
        if (!BitmapFontLoader::load("assets\\font\\gauge.fnt", m_font))
        {
            log::error("GuiRenderer: failed to load font.");
            return false;
        }
        return true;
    }

    void GUIRenderer::render(Graphics *graphics, ResourceCache &cache)
    {
        if (!m_visible) return;

        update_window_size();
        m_z = 0.0f;

        std::vector<Widget*> widgets;
        m_gui->get_widgets(widgets);

//        render_text(graphics, cache, {300.0f, 300.0f}, "abvwxyz 1234567890");
//        render_text(graphics, cache, {300.0f, 350.0f}, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");

        for (Widget *widget : widgets)
        {
            for (WidgetRenderCommand &cmd : widget->get_render_commands())
            {
                render_widget(graphics, cache, cmd);
            }
        }
    }

    void GUIRenderer::render_quad(Graphics *graphics, ResourceCache &cache, const Vector2 &position, const Vector2 &scale, const ResourceID texture_id, const Vector4 uv /* = {0.0,0.0,1.0,1.0} */)
    {
        if (texture_id == InvalidResource)
            return;

        update_buffers(uv);

        Vector3 scaled_position = {
            position.x / m_win_width,
            position.y / m_win_height,
            m_z
        };

        Vector2 scaled_scale = {
            scale.x / m_win_width,
            scale.y / m_win_height
        };

        m_vao->bind();
        Program *program = cache.get_program(m_vert_shader, m_frag_shader);
        if (program)
        {
            program->use();
            const int scale_loc = program->get_uniform_location("gui_scale");
            const int position_loc = program->get_uniform_location("gui_position");
            const int texture_loc = program->get_uniform_location("gui_texture");
            program->uniform(scale_loc, scaled_scale);
            program->uniform(position_loc, scaled_position);
            program->uniform_sampler2D(texture_loc, 0);
            graphics->set_blend_enabled(true);
            graphics->set_texture(0, cache.get<Texture2D>(texture_id));
            graphics->update_state();

            graphics->draw_triangles(0, 6);
            m_z -= 0.005f;
        }
    }

    void GUIRenderer::render_text(Graphics *graphics, ResourceCache &cache, const Vector2 &position, const char * const buffer)
    {
        const Vector2 space_advance(m_font.get_line_height() * 0.25f, 0.0f);
        Vector2 cursor = position;

        for (size_t i=0; buffer[i] != '\0'; i++)
        {
            const BitmapFontCharacter fontchar = m_font.get_character(static_cast<unsigned int>(buffer[i]));
            if (buffer[i] != ' ')
            {
                Vector2 offset(fontchar.offset_x, fontchar.offset_y);
                Vector2 size(fontchar.width, fontchar.height);

                Vector4 uv(fontchar.x, fontchar.y + fontchar.height,
                           fontchar.width, fontchar.height);
                uv /= m_font_texture_size;
                uv.y = 1.0f - uv.y;

                render_quad(graphics, cache, offset + cursor, size, m_font_texture, uv);
                cursor += Vector2(fontchar.advance_x, 0.0f);
            }
            else
            {
                cursor += space_advance;
            }
        }
    }

    void GUIRenderer::render_widget(Graphics *graphics, ResourceCache &cache, WidgetRenderCommand &cmd)
    {
        render_quad(graphics, cache, cmd.position, cmd.size, cmd.texture_id);

        if (cmd.text != nullptr)
            render_text(graphics, cache, cmd.text_position, cmd.text);
    }

    void GUIRenderer::update_window_size()
    {
        int win_width, win_height;
        m_window->get_size(&win_width, &win_height);
        m_win_width = static_cast<float>(win_width);
        m_win_height = static_cast<float>(win_height);
    }

    void GUIRenderer::set_visible(const bool visible)
    { m_visible = visible; }

    bool GUIRenderer::is_visible() const
    { return m_visible; }

    void GUIRenderer::init_buffers()
    {
        using namespace gui_renderer_internal;

        Vertex vertices[] = {
            {{ -1.0f, -1.0f }, { 0.0f, 0.0f }},
            {{ 1.0f, -1.0f }, { 1.0f, 0.0f }},
            {{ -1.0f, 1.0f }, { 0.0f, 1.0f }},
            {{ -1.0f, 1.0f }, { 0.0f, 1.0f }},
            {{ 1.0f, -1.0f }, { 1.0f, 0.0f }},
            {{ 1.0f, 1.0f }, { 1.0f, 1.0f }}
        };

        m_vbuffer->bind();
        m_vbuffer->resize(sizeof(vertices), true);
        m_vbuffer->write(0, sizeof(vertices), &vertices);

        m_vao->bind();
        m_vao->set_attribute(VertexAttrib::Position, 0, sizeof(Vertex), 2);
        m_vao->set_attribute(VertexAttrib::TexCoord, sizeof(Vector2), sizeof(Vertex), 2);
    }

    void GUIRenderer::update_buffers(const Vector4 &uv)
    {
        using namespace gui_renderer_internal;

        Vector2 top_left = { uv.x, uv.y };
        Vector2 top_right = { uv.x + uv.z, uv.y };
        Vector2 bot_left = { uv.x, uv.y + uv.w };
        Vector2 bot_right = { uv.x + uv.z, uv.y + uv.w };

        Vertex vertices[] = {
            {{ -1.0f, -1.0f }, top_left },
            {{ 1.0f, -1.0f }, top_right },
            {{ -1.0f, 1.0f }, bot_left },
            {{ -1.0f, 1.0f }, bot_left },
            {{ 1.0f, -1.0f }, top_right },
            {{ 1.0f, 1.0f }, bot_right }
        };

        m_vbuffer->bind();
        m_vbuffer->write(0, sizeof(vertices), &vertices);
    }

} // der
