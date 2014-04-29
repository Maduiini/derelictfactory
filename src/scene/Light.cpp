
#include "Light.h"

namespace der
{

    Light::Light()
        : m_type(LightType::Point)
        , m_position(Vector3::zero)
        , m_color(Vector3(1.0f, 1.0f, 1.0f))
        , m_energy(1.0f)
        , m_radius(10.0f)
    { }

    Light::Light(LightType type)
        : m_type(type)
        , m_position(Vector3::zero)
        , m_color(Vector3(1.0f, 1.0f, 1.0f))
        , m_energy(1.0f)
        , m_radius(10.0f)
    { }

    void Light::set_type(LightType type)
    { m_type = type; }

    LightType Light::get_type() const
    { return m_type; }

    void Light::set_color(const Vector3 &color)
    { m_color = color; }

    Vector3 Light::get_color() const
    { return m_color; }

    void Light::set_radius(float radius)
    { m_radius = radius; }

    float Light::get_radius() const
    { return m_radius; }

    void Light::set_energy(float energy)
    { m_energy = energy; }

    float Light::get_energy() const
    { return m_energy; }

} // der
