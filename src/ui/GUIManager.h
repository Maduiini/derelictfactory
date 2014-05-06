
#ifndef H_DER_GUI_MANAGER_H
#define H_DER_GUI_MANAGER_H

#include <vector>

#include "../math/Vector.h"

namespace der
{
    class Widget;

    class GUIManager
    {
    public:
        GUIManager();
        ~GUIManager();

        void add_widget(Widget *widget);

        void get_widgets(std::vector<Widget*> &widgets);

        /// Updates all widgets.
        void update(double delta_time, Vector2 mouse, bool left_pressed);

    private:
        std::vector<Widget*> m_widgets;
        bool m_previous_pressed;

    };

} // der

#endif // H_DER_GUI_MANAGER_H
