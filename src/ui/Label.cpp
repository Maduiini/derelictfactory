
#include "Label.h"

namespace der
{

    Label::Label(const Vector2 position, const std::string &text)
        : m_position(position)
        , m_text(text)
    {
        m_render_cmds.push_back({ InvalidResource, Vector2::zero, Vector2::zero, m_text.c_str(), m_position});
    }

    Label::~Label()
    { }

    void Label::update(double delta_time, Vector2 mouse)
    {
        m_render_cmds[0].text = m_text.c_str();
    }

} // der
