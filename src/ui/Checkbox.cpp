
#include "Checkbox.h"

#include "../Log.h"

namespace der
{

    ResourceID Checkbox::m_checkbox_texture = make_resource("checkbox.tga");
    ResourceID Checkbox::m_checkbox_mark_texture = make_resource("checkbox_mark.tga");

    Checkbox::Checkbox(Vector2 position, std::string title)
        : m_position(position)
        , m_size(Vector2(50.0f, 50.0f))
        , m_title(title)
        , m_checked(false)
        , m_state_changed(nullptr)
    {
        m_render_cmds.push_back({Checkbox::m_checkbox_texture, m_position, m_size, nullptr});
        m_render_cmds.push_back({InvalidResource, m_position + Vector2(50.0f, 10.0f), Vector2::zero, m_title.c_str()});
        m_render_cmds[1].text_position = m_render_cmds[1].position;
    }

    Checkbox::~Checkbox()
    {
        delete m_state_changed;
    }

    void Checkbox::mouse_released(Vector2 point)
    {
        if (is_inside(point))
        {
            switch_state();
            log::debug("checkbox state: %", (m_checked? "pressed" : "not pressed"));
        }
    }

    bool Checkbox::is_inside(Vector2 point) const
    {
        Vector2 point_moved = point - m_position;
        return point_moved.x >= 0
            && point_moved.y >= 0
            && point_moved.x < m_size.x
            && point_moved.y < m_size.y;
    }

    void Checkbox::set_checked(bool checked)
    {
        if (m_checked != checked)
            switch_state();
    }

    bool Checkbox::is_checked() const
    { return m_checked; }

    void Checkbox::switch_state()
    {
        m_checked = !m_checked;
        if (m_state_changed)
            m_state_changed->handle(this);

        if (m_checked)
        {
            m_render_cmds[0].texture_id = m_checkbox_mark_texture;
        }
        else
        {
            m_render_cmds[0].texture_id = m_checkbox_texture;
        }
    }

} // der
