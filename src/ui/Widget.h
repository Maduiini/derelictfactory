
#ifndef H_DER_WIDGET_H
#define H_DER_WIDGET_H

#include <vector>
#include <string>

#include "../math/Vector.h"

#include "../resource/Resource.h"

namespace der
{

    class Widget;

    class GUIEventHandler
    {
    public:
        virtual ~GUIEventHandler() { }
        virtual void handle(Widget *widget) = 0;
    };


    class GUIRenderer;

    struct WidgetRenderCommand
    {
        ResourceID texture_id;
        Vector2 position;
        Vector2 size;

        const char* text;
        Vector2 text_position;
    };

    class Widget
    {
    public:
        Widget()
            : m_render_cmds()
        { }

        virtual ~Widget()
        { }

        virtual void update(double delta_time, Vector2 mouse)
        { }

        /// Mouse was pressed.
        /// \note \c point might not be on top of the widget.
        virtual void mouse_pressed(Vector2 point)
        { }

        /// Mouse was released.
        /// \note \c point might not be on top of the widget.
        virtual void mouse_released(Vector2 point)
        { }

        virtual std::vector<WidgetRenderCommand> get_render_commands(GUIRenderer *renderer) const
        {
            return m_render_cmds;
        }

    protected:
        std::vector<WidgetRenderCommand> m_render_cmds;

    };

} // der

#endif // H_DER_WIDGET_H
