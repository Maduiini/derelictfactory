
#include "ValueDisplay.h"
#include "GUIRenderer.h"

namespace der
{

    ValueDisplay::ValueDisplay(const Vector2 position, const std::string &text)
        : m_position(position)
        , m_text(text)
        , m_value(0.0f)
        , m_value_str()
    {
        m_render_cmds.push_back({ InvalidResource, Vector2::zero, Vector2::zero, m_text.c_str(), m_position});
        const Vector2 offset(12.0f * m_text.length() + 8.0f, 0.0f);
        m_render_cmds.push_back({ InvalidResource, Vector2::zero, Vector2::zero, m_value_str.c_str(), m_position + offset});
    }

    ValueDisplay::~ValueDisplay()
    { }

    void ValueDisplay::update(double delta_time, Vector2 mouse)
    {
        m_render_cmds[0].text = m_text.c_str();

        m_value_str = to_string(m_value);
        m_render_cmds[1].text = m_value_str.c_str();
    }

    std::vector<WidgetRenderCommand> ValueDisplay::get_render_commands(GUIRenderer *renderer) const
    {
        const Vector2 offset(renderer->get_text_width(m_text.c_str()), 0.0f);
        const_cast<WidgetRenderCommand&>(m_render_cmds[1]).text_position = m_position + offset;
        return m_render_cmds;
    }

} // der
