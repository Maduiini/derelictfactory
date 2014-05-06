
#include "GUIRenderer.h"
#include "GUIManager.h"
#include "Widget.h"

#include "../renderer/Graphics.h"
#include "../renderer/VertexBuffer.h"
#include "../renderer/VertexArrayObject.h"
#include "../resource/ResourceCache.h"

#include "../math/Vector.h"

#include "../Window.h"

namespace der
{

    ResourceID GUIRenderer::m_vert_shader = make_resource("gui.vert");
    ResourceID GUIRenderer::m_frag_shader = make_resource("gui.frag");
    VertexArrayObject *GUIRenderer::m_vao = nullptr;
    VertexBuffer *GUIRenderer::m_vbuffer = nullptr;

    GUIRenderer::GUIRenderer(GUIManager *gui, Window *window)
        : m_gui(gui)
        , m_window(window)
        , m_visible(true)
        , m_win_width(0.0f)
        , m_win_height(0.0f)
    {
        build();
    }

    GUIRenderer::~GUIRenderer()
    { }

    void GUIRenderer::render(Graphics *graphics, ResourceCache &cache)
    {
        if (!m_visible) return;

        update_window_size();

        std::vector<Widget*> widgets;
        m_gui->get_widgets(widgets);

        for (Widget *widget : widgets)
        {
            for (WidgetRenderCommand &cmd : widget->get_render_commands())
            {
                render_widget(graphics, cache, cmd);
            }
        }
    }

    void GUIRenderer::render_widget(Graphics *graphics, ResourceCache &cache, WidgetRenderCommand &cmd)
    {
        Vector2 position = {
            cmd.position.x / m_win_width,
            cmd.position.y / m_win_height
        };

        Vector2 scale = {
            cmd.size.x / m_win_width,
            cmd.size.y / m_win_height
        };

        m_vao->bind();
        Program *program = cache.get_program(m_vert_shader, m_frag_shader);
        if (program)
        {
            program->use();
            const int scale_loc = program->get_uniform_location("gui_scale");
            const int position_loc = program->get_uniform_location("gui_position");
            const int texture_loc = program->get_uniform_location("gui_texture");
            program->uniform(scale_loc, scale);
            program->uniform(position_loc, position);
            program->uniform_sampler2D(texture_loc, 0);
            graphics->set_texture(0, cache.get<Texture2D>(cmd.texture_id));
            graphics->update_state();

            graphics->draw_triangles(0, 6);
        }
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

    // static
    void GUIRenderer::build()
    {
        float vertices[] = {
            -1.0f, -1.0f,
             1.0f, -1.0f,
            -1.0f,  1.0f,
            -1.0f,  1.0f,
             1.0f, -1.0f,
             1.0f,  1.0f
        };

        m_vbuffer = new VertexBuffer();
        m_vbuffer->bind();
        m_vbuffer->resize(sizeof(vertices), false);
        m_vbuffer->write(0, sizeof(vertices), &vertices);

        m_vao = new VertexArrayObject();
        m_vao->bind();
        m_vao->set_attribute(VertexAttrib::Position, 0, 0, 2);
    }

} // der
