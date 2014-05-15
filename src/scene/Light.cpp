
#include "Light.h"
#include "../math/Math.h"

namespace der
{

    Light::Light()
        : m_type(LightType::Point)
        , m_position(Vector3::zero)
        , m_color(Vector3(1.0f, 1.0f, 1.0f))
        , m_energy(1.0f)
        , m_radius(10.0f)
        , m_spot_angle(30.0f * Math::DEG_2_RAD)
    { }

    Light::Light(LightType type)
        : m_type(type)
        , m_position(Vector3::zero)
        , m_direction(0.0f, -1.0f, 0.0f)
        , m_color(1.0f, 1.0f, 1.0f)
        , m_energy(1.0f)
        , m_radius(10.0f)
        , m_spot_angle(30.0f * Math::DEG_2_RAD)
    { }

    void Light::set_type(LightType type)
    { m_type = type; }

    LightType Light::get_type() const
    { return m_type; }

    void Light::set_direction(const Vector3 &direction)
    { m_direction = direction; }

    Vector3 Light::get_direction() const
    { return m_direction; }

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

    void Light::set_spot_angle(float angle_rads)
    { m_spot_angle = angle_rads; }

    float Light::get_spot_angle() const
    { return m_spot_angle; }

    Aabb Light::get_bounding_box() const
    {
        return Aabb(Vector3(-m_radius), Vector3(m_radius));
    }

} // der
