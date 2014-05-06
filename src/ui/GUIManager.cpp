
#include "GUIManager.h"
#include "Widget.h"

#include "../Debug.h"

namespace der
{

    GUIManager::GUIManager()
        : m_widgets()
        , m_previous_pressed(false)
    { }

    GUIManager::~GUIManager()
    {
        for (Widget *widget : m_widgets)
        {
            delete widget;
        }
    }

    void GUIManager::add_widget(Widget *widget)
    {
        DER_ASSERT(widget);
        m_widgets.push_back(widget);
    }

    void GUIManager::get_widgets(std::vector<Widget*> &widgets)
    {
        for (Widget *widget : m_widgets)
        {
            widgets.push_back(widget);
        }
    }

    void GUIManager::update(double delta_time, Vector2 point, bool left_pressed)
    {
        for (Widget *widget : m_widgets)
        {
            if (left_pressed && !m_previous_pressed)
                widget->mouse_pressed(point);

            if (!left_pressed && m_previous_pressed)
                widget->mouse_released(point);

            widget->update(delta_time, point);
        }

        m_previous_pressed = left_pressed;
    }

} // der
