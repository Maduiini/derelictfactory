
#ifndef H_DER_INPUT_CONTROLLER_H
#define H_DER_INPUT_CONTROLLER_H

#include "../Input.h"

#include "../math/Vector.h"

namespace der
{

    class Window;
    class GameObject;

    class InputController
    {
    public:
        InputController();
        virtual ~InputController();

        virtual void update(float delta_time) = 0;

        void set_window(Window *window);
        Window* get_window();

        void set_object(GameObject *object);
        GameObject* get_object();

    protected:
        bool key_pressed(Key key) const;
        bool key_down(Key key) const;

        bool button_down(MouseButton button) const;
        Vector2 get_mouse_pos() const;
        Vector2 get_mouse_delta() const;

        void set_capture_mouse(bool captured);
        void toggle_mouse_captured();
        bool is_mouse_captured() const;

    private:
        Window *m_window;
        GameObject *m_object;
    };

} // der

#endif // H_DER_INPUT_CONTROLLER_H

