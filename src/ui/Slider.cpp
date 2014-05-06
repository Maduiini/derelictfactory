
#include "Slider.h"

#include "../Log.h"

namespace der
{

    ResourceID Slider::slider_bar_texture = make_resource("button.tga");
    ResourceID Slider::slider_knob_texture = make_resource("slider_knob.tga");

    Slider::Slider(Vector2 position, float width, float min_value /* = 0.0f */, float max_value /* = 1.0f */)
        : m_position(position)
        , m_width(width)
        , m_height(8.0f)
        , m_min_value(min_value)
        , m_max_value(max_value)
        , m_knob_position(0.0f)
        , m_knob_radius(16.0f)
        , m_knob_grabbed(false)
    {
        m_render_cmds.push_back({ Slider::slider_bar_texture, m_position, Vector2(m_width, m_height) });
        m_render_cmds.push_back({ Slider::slider_knob_texture, m_position, Vector2(m_knob_radius, m_knob_radius) });
    }

    Slider::~Slider()
    { }

    void Slider::mouse_pressed(Vector2 point)
    {
        if (is_inside_knob(point))
            m_knob_grabbed = true;
    }

    void Slider::mouse_released(Vector2 point)
    {
        m_knob_grabbed = false;
    }

    void Slider::update(double delta_time, Vector2 mouse)
    {
        if (m_knob_grabbed)
        {
            m_knob_position = mouse.x - m_position.x;
            if (m_knob_position < 0)
                m_knob_position = 0;
            if (m_knob_position > m_width)
                m_knob_position = m_width;

            log::debug("Slider value: % (%)",
                      (int)(get_absolute_value() * 100.0f) / 100.0f,
                      (int)(get_relative_value() * 100.0f) / 100.0f);
        }

        m_render_cmds[0] = {
            Slider::slider_bar_texture,
            m_position + Vector2(0.0f, -m_height * 0.5f),
            Vector2(m_width, m_height)
        };

        m_render_cmds[1] = {
            Slider::slider_knob_texture,
            get_knob_center() - Vector2(m_knob_radius, m_knob_radius + m_height * 0.5f),
            Vector2(m_knob_radius * 2.0f, m_knob_radius * 2.0f)
        };
    }

    bool Slider::is_inside_knob(Vector2 point) const
    {
        Vector2 point_moved = point - get_knob_center();
        return point_moved.length2() <= m_knob_radius * m_knob_radius;
    }

    float Slider::get_relative_value() const
    {
        return m_knob_position / m_width;
    }

    float Slider::get_absolute_value() const
    {
        return m_min_value + (m_max_value - m_min_value) * get_relative_value();
    }

    Vector2 Slider::get_knob_center() const
    {
        return m_position + Vector2(m_knob_position, m_height * 0.5f);
    }

} // der
