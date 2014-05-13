
#ifndef H_DER_SLIDER_H
#define H_DER_SLIDER_H

#include <string>

#include "Widget.h"

namespace der
{

    class Slider : public Widget
    {
    public:
        Slider(Vector2 position, float width, float min_value = 0.0f, float max_value = 1.0f);
        virtual ~Slider();

        virtual void mouse_pressed(Vector2 point) override;

        virtual void mouse_released(Vector2 point) override;

        virtual void update(double delta_time, Vector2 mouse) override;

        /// Returns true if the point is inside the knob.
        bool is_inside_knob(Vector2 point) const;

        /// Sets the absolute value of the slider.
        void set_value(float value);

        /// Returns slider value in range [0.0, 1.0]
        float get_relative_value() const;

        /// Returns slider value in range [min, max]
        float get_absolute_value() const;

        void set_value_changed_handler(GUIEventHandler *handler)
        { m_value_changed = handler; }

    private:
        Vector2 get_knob_center() const;

    private:
        Vector2 m_position;
        const float m_width;
        const float m_height;
        float m_min_value;
        float m_max_value;
        float m_knob_position;
        float m_knob_radius;
        bool m_knob_grabbed;

        GUIEventHandler *m_value_changed;

        static ResourceID slider_bar_texture;
        static ResourceID slider_knob_texture;
    };
} // der

#endif // H_DER_SLIDER_H
