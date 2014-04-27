
#include "InputController.h"
#include "../Window.h"

namespace der
{

    InputController::InputController()
        : m_window(nullptr)
    { }

    InputController::~InputController()
    { }

    void InputController::set_window(Window *window)
    { m_window = window; }

    Window* InputController::get_window()
    { return m_window; }

    bool InputController::key_pressed(Key key) const
    { return m_window->key_pressed(key); }

    bool InputController::key_down(Key key) const
    { return m_window->key_down(key); }

    bool InputController::button_down(MouseButton button) const
    { return m_window->button_down(button); }

    Vector2 InputController::get_mouse_pos() const
    { return Vector2(m_window->get_mouse_x(), m_window->get_mouse_y()); }

    Vector2 InputController::get_mouse_delta() const
    { return Vector2(m_window->get_mouse_dx(), m_window->get_mouse_dy()); }

    void InputController::set_capture_mouse(bool captured)
    { return m_window->set_mouse_captured(captured); }

    void InputController::toggle_mouse_captured()
    { m_window->toggle_mouse_captured(); }

    bool InputController::is_mouse_captured() const
    { return m_window->is_mouse_captured(); }

} // der
