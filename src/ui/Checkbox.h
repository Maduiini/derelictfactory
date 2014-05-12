
#ifndef H_DER_CHECKBOX_H
#define H_DER_CHECKBOX_H

#include "Widget.h"

#include <string>

namespace der
{

    class Checkbox : public Widget
    {
    public:
        Checkbox(Vector2 position, std::string title);
        virtual ~Checkbox();

        void set_title(std::string title)
        { m_title = title; }

        std::string get_title() const
        { return m_title; }

        virtual void mouse_released(Vector2 point) override;

        bool is_inside(Vector2 point) const;

        void set_checked(bool checked);
        bool is_checked() const;

        void set_state_changed_handler(GUIEventHandler *handler)
        { m_state_changed = handler; }

    private:
        void switch_state();

    private:
        Vector2 m_position;
        Vector2 m_size;
        std::string m_title;
        bool m_checked;

        GUIEventHandler *m_state_changed;

        static ResourceID m_checkbox_texture;
        static ResourceID m_checkbox_mark_texture;

    };

} // der

#endif // H_DER_CHECKBOX_H
