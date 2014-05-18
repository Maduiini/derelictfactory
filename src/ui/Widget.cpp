
#include "Widget.h"
#include "GUIRenderer.h"

namespace der
{

    Widget::Widget()
        : m_render_cmds()
    { }

    Widget::~Widget()
    {
        for (GUIRendererCommand *cmd : m_render_cmds)
        {
            delete cmd;
        }
    }

    void Widget::update(double delta_time, Vector2 mouse)
    { }

    void Widget::mouse_pressed(Vector2 point)
    { }

    void Widget::mouse_released(Vector2 point)
    { }

    std::vector<GUIRendererCommand*> Widget::get_render_commands(GUIRenderer *renderer) const
    {
        return m_render_cmds;
    }

} // der
