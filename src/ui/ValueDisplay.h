
#ifndef H_DER_VALUE_DISPLAY_H
#define H_DER_VALUE_DISPLAY_H

#include "Widget.h"

namespace der
{

    class ValueDisplay : public Widget
    {
    public:
        ValueDisplay(const Vector2 position, const std::string &text);
        virtual ~ValueDisplay();

        void set_text(const std::string &text)
        { m_text = text; }

        std::string get_text() const
        { return m_text; }

        void set_value(float value)
        { m_value = value; }

        float get_value() const
        { return m_value; }

        virtual void update(double delta_time, Vector2 mouse) override;

        virtual std::vector<WidgetRenderCommand> get_render_commands(GUIRenderer *renderer) const override;

    private:
        Vector2 m_position;
        std::string m_text;
        float m_value;
        std::string m_value_str;

    };

} // der

#endif // H_DER_VALUE_DISPLAY_H

