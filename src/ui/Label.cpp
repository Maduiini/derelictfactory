
#include "Label.h"

#include "GUIRenderer.h"

namespace der
{

    Label::Label(const Vector2 position, const std::string &text)
        : m_position(position)
        , m_text(text)
    {
        m_render_cmds.push_back(new RenderText(m_text.c_str(), m_position));
    }

    Label::~Label()
    { }

    void Label::update(double delta_time, Vector2 mouse)
    {
        static_cast<RenderText*>(m_render_cmds[0])->set_text(m_text.c_str());
    }

} // der
