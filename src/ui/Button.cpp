
#include "Button.h"

#include "../Log.h"

namespace der
{
    ResourceID Button::m_button_texture = make_resource("button.tga");
    ResourceID Button::m_button_highlight_texture = make_resource("button_highlight.tga");
    ResourceID Button::m_button_pressed_texture = make_resource("button_pressed.tga");

    Button::Button(Vector2 position, Vector2 size, std::string title)
        : m_position(position)
        , m_size(size)
        , m_title(title)
        , m_down(false)
    {
        m_render_cmds.push_back({ Button::m_button_texture, m_position, m_size, nullptr });
    }

    Button::~Button()
    { }

    void Button::update(double delta_time, Vector2 mouse)
    {
        if (is_inside(mouse))
        {
            if (m_down)
                m_render_cmds[0].texture_id = Button::m_button_pressed_texture;
            else
                m_render_cmds[0].texture_id = Button::m_button_highlight_texture;
        }
        else
            m_render_cmds[0].texture_id = Button::m_button_texture;

        /*
        if (m_down)
        {

        }
        else
            m_render_cmds[0].texture_id = Button::m_button_texture;
        */
    }

    void Button::mouse_pressed(Vector2 point)
    {
        if (is_inside(point))
        {
            log::debug("Button pressed.");
            m_down = true;
        }
    }

    void Button::mouse_released(Vector2 point)
    {
        m_down = false;
        if (is_inside(point))
            log::debug("Button released.");
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
