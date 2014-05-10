
#ifndef H_DER_BUTTON_H
#define H_DER_BUTTON_H

#include <string>

#include "Widget.h"

namespace der
{

    class Button : public Widget
    {
    public:
        Button(Vector2 position, Vector2 size, std::string title);
        virtual ~Button();

        void set_title(std::string title)
        { m_title = title; }

        std::string get_title() const
        { return m_title; }

        virtual void update(double delta_time, Vector2 mouse) override;

        virtual void mouse_pressed(Vector2 point) override;

        virtual void mouse_released(Vector2 point) override;

        /// Returns true if \c point is inside the button.
        bool is_inside(Vector2 point) const;

    private:
        Vector2 m_position;
        Vector2 m_size;
        std::string m_title;
        bool m_down;

        static ResourceID m_button_texture;
        static ResourceID m_button_highlight_texture;
        static ResourceID m_button_pressed_texture;

    };

} // der

#endif // H_DER_BUTTON_H
