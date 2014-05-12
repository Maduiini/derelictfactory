#version 330

uniform sampler2D tex_albedo;
uniform sampler2D tex_normal;
uniform sampler2D tex_roughness;
uniform sampler2D tex_metallic;
uniform samplerCube tex_env;

struct Light
{
    vec4 position;      // Position(w=1) or direction(w=0)
    vec4 color_energy;  // rgb = color, w = energy
    float radius;
};

#define MAX_LIGHTS 16
layout(row_major) uniform Lights
{
    int light_count;
    Light lights[MAX_LIGHTS];
};


in vec3 view_vec;

out vec4 out_color;

vec3 get_color()
{
    vec3 color = texture(tex_env, view_vec * vec3(1.0, -1.0, 1.0)).rgb;
    // linearize gamma
//    return pow(color, vec3(2.2));
    return color;
}

void main()
{
    vec3 color = get_color();
    // gamma corrected output
//    out_color = vec4(pow(color, vec3(1.0 / 2.2)), 1.0);
    out_color = vec4(color, 1.0);
}


