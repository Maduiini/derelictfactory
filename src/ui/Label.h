
#ifndef H_DER_LABEL_H
#define H_DER_LABEL_H

#include "Widget.h"

#include <string>

namespace der
{

    class Label : public Widget
    {
    public:
        Label(const Vector2 position, const std::string &text);
        virtual ~Label();

        void set_text(const std::string &text)
        { m_text = text; }

        std::string get_text() const
        { return m_text; }

        virtual void update(double delta_time, Vector2 mouse) override;

    private:
        Vector2 m_position;
        std::string m_text;

    };


} // der

#endif // H_DER_LABEL_H
