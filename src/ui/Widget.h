
#ifndef H_DER_WIDGET_H
#define H_DER_WIDGET_H

#include <vector>
#include <string>

#include "../math/Vector.h"

#include "../resource/Resource.h"

namespace der
{
    class Widget;
    class GUIRenderer;
    class GUIRendererCommand;

    class GUIEventHandler
    {
    public:
        virtual ~GUIEventHandler() { }
        virtual void handle(Widget *widget) = 0;
    };

    class Widget
    {
    public:
        Widget();
        virtual ~Widget();

        virtual void update(double delta_time, Vector2 mouse);

        /// Mouse was pressed.
        /// \note \c point might not be on top of the widget.
        virtual void mouse_pressed(Vector2 point);

        /// Mouse was released.
        /// \note \c point might not be on top of the widget.
        virtual void mouse_released(Vector2 point);

        virtual std::vector<GUIRendererCommand*> get_render_commands(GUIRenderer *renderer) const;

    protected:
        std::vector<GUIRendererCommand*> m_render_cmds;

    };

} // der

#endif // H_DER_WIDGET_H
