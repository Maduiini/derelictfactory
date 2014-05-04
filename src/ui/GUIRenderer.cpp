
#include "GUIRenderer.h"
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

    ResourceID GUIRenderer::m_button_tex = make_resource("test_button.tga");

    GUIRenderer::GUIRenderer(GUIManager *gui, Window *window)
        : m_gui(gui)
        , m_window(window)
    {
        build();
    }

    GUIRenderer::~GUIRenderer()
    { }

    void GUIRenderer::render(Graphics *graphics, ResourceCache &cache)
    {
        const float test_button_width = 64.0f;
        const float test_button_height = 32.0f;
        const float test_button_x = 10.0f;
        const float test_button_y = 10.0f;

        int win_width, win_height;
        float win_width_f, win_height_f;
        m_window->get_size(&win_width, &win_height);
        win_width_f = static_cast<float>(win_width);
        win_height_f = static_cast<float>(win_height);

        Vector2 scale = {
            test_button_width / win_width_f,
            test_button_height / win_height_f
        };

        Vector2 position = {
            test_button_x / win_width_f,
            test_button_y / win_height_f
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
            graphics->set_texture(0, cache.get<Texture2D>(m_button_tex));
            graphics->update_state();

            graphics->draw_triangles(0, 6);
        }
    }

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
