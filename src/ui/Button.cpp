
#include "Button.h"

#include "GUIRenderer.h"

#include "../Log.h"

namespace der
{
    ResourceID Button::m_button_texture = make_resource("UI/button.tga");
    ResourceID Button::m_button_highlight_texture = make_resource("UI/button_highlight.tga");
    ResourceID Button::m_button_pressed_texture = make_resource("UI/button_pressed.tga");

    Button::Button(Vector2 position, Vector2 size, std::string title)
        : m_position(position)
        , m_size(size)
        , m_title(title)
        , m_down(false)
        , m_released_handler(nullptr)
    {
        m_render_cmds.push_back(new RenderCachedTexture(m_button_texture, m_position, m_size));
        m_render_cmds.push_back(new RenderText(m_title.c_str(), m_position + Vector2(10.0f, 10.0f)));
    }

    Button::~Button()
    { }

    void Button::update(double delta_time, Vector2 mouse)
    {
        if (is_inside(mouse))
        {
            if (m_down)
                 static_cast<RenderCachedTexture*>(m_render_cmds[0])->set_texture_id(Button::m_button_pressed_texture);
            else
                static_cast<RenderCachedTexture*>(m_render_cmds[0])->set_texture_id(Button::m_button_highlight_texture);
        }
        else
            static_cast<RenderCachedTexture*>(m_render_cmds[0])->set_texture_id(Button::m_button_texture);
    }

    void Button::mouse_pressed(Vector2 point)
    {
        if (is_inside(point))
        {
            m_down = true;
        }
    }

    void Button::mouse_released(Vector2 point)
    {
        m_down = false;
        if (is_inside(point) && m_released_handler)
            m_released_handler->handle(this);
    }

    bool Button::is_inside(Vector2 point) const
    {
        Vector2 point_moved = point - m_position;
        return point_moved.x >= 0
            && point_moved.y >= 0
            && point_moved.x < m_size.x
            && point_moved.y < m_size.y;
    }

} // der
