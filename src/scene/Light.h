
#ifndef H_DER_LIGHT_H
#define H_DER_LIGHT_H

#include "../math/Vector.h"
#include "../math/Aabb.h"

namespace der
{

    enum class LightType
    {
        Point, Directional, Spot
    };

    class Light
    {
    public:
        Light();
        Light(LightType type);

        void set_type(LightType type);
        LightType get_type() const;

        void set_direction(const Vector3 &direction);
        Vector3 get_direction() const;

        void set_color(const Vector3 &color);
        Vector3 get_color() const;

        void set_radius(float radius);
        float get_radius() const;

        void set_energy(float energy);
        float get_energy() const;

        void set_spot_angle(float angle_rads);
        float get_spot_angle() const;

        Aabb get_bounding_box() const;

    private:
        LightType m_type;
        Vector3 m_position;
        Vector3 m_direction;
        Vector3 m_color;
        float m_energy;
        float m_radius;
        float m_spot_angle;
    };

} // der

#endif // H_DER_LIGHT_H

