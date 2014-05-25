
uniform sampler2DShadow tex_shadowmap;

struct Light
{
    vec4 position;      // Position(w=1) or direction(w=0)
    vec4 direction;     // xyz = direction, w = cos(spot_angle)
    vec4 color_energy;  // rgb = color, w = energy
    float radius;
};

#define MAX_LIGHTS 16
layout(row_major) uniform Lights
{
    mat4 mat_light;
    int light_count;
    Light lights[MAX_LIGHTS];
};
