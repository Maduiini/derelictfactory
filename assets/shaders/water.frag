#version 330

#include "common.glsl"

uniform sampler2D tex_albedo;
uniform sampler2D tex_normal;
uniform sampler2D tex_roughness;
uniform sampler2D tex_metallic;
uniform samplerCube tex_env;

layout(row_major) uniform Globals
{
    mat4 mat_proj;
    mat4 mat_view;
    vec3 camera_pos;
    float time;
    float window_width;
    float window_height;
};

#include "param_uniforms.glsl"


in vec3 position;
in vec3 normal;
in vec4 tangent;
in vec2 tcoord;
in vec3 view_vec;
in vec3 view_normal;

out vec4 out_color;
out vec4 out_normal;
out vec4 out_extra;

//vec3 get_normal()
//{
//    return der_get_normal(tex_normal, tcoord * 2.0e-2, normal, tangent);
//}

vec3 get_albedo()
{
    return der_get_albedo(tex_albedo, tcoord);
}

#include "pbs.glsl"


vec3 waveN_lin(const vec2 wave_dir, const float amplitude, const float speed)
{
    vec2 D = normalize(wave_dir);
    float A = amplitude;
    const float F = 10.0;

    vec2 pos = position.xz;
    float theta = time * speed;

    vec2 Wn = vec2(F * A * cos(dot(D, pos) * F + theta));
//    const float k = 1.5;
//    vec2 Wn = vec2(F * A * k * pow(sin(dot(D, pos) * F + theta), k-1) * cos(dot(D, pos) * F + theta));

    vec2 d = Wn * D;

    vec3 n = vec3(-d.x, 1, -d.y);
    return normalize(n);
}

vec3 waveN_circ(vec2 wave_center, const float amplitude, float speed)
{
    vec2 C = wave_center;
    float A = amplitude;
    const float F = 10.0;

    vec2 pos = position.xz;
    float theta = time * -speed;

    vec2 Wn = vec2(F * A * cos(distance(C, pos) * F + theta));

    vec2 d = Wn;

    vec3 n = vec3(-d.x, 1, -d.y);
    return normalize(n);
}

vec3 waveN(float f)
{
    vec3 n = waveN_lin(vec2(0.1, 1.0), 0.002 * f, 2.4)
        + waveN_lin(vec2(0.5, 0.5), 0.002 * f, 1.2);
//    vec3 n = waveN_circ(vec2(38.0, -35), 0.002, 1.2);
    return normalize(n);
}

void main()
{
    float d = length(view_vec);

    float f = 20.0 / (d + 1.0);

//    vec3 N = normal;
//    vec3 N = normalize(mix(normal, get_normal(), nm_influence * 0.02));
    vec3 N = waveN(f);
    vec3 V = normalize(view_vec);

//    vec3 albedo = vec3(0.03, 0.14, 0.04);
    vec3 albedo = vec3(0.04);
    float r = 0.0;

    vec3 c_spec = (vec3(1.0) - albedo) * vec3(0.04037);
//    vec3 c_spec = vec3(0.18037);
//    vec3 c_spec = vec3(0.08037);
//    vec3 c_spec = vec3(0.04037);
    vec3 c_diff = albedo; //vec3(0.04, 0.02, 0.01);
//    vec3 c_diff = vec3(0.24, 0.22, 0.21);

    vec3 color = lighting(c_diff, c_spec, N, V, r);

    float lum = dot(color, vec3(1.0));
//    float lum = dot(color, vec3(1.0)) + 0.2;
//    float lum = dot(color, vec3(1.0)) + albedo.g;
//    float lum = dot(color, vec3(1.0)) + 0.44 + r;

    // gamma corrected output
    out_color = gamma_correct(color, lum);
//    out_normal = vec4(N, length(view_vec));
//    out_normal = vec4(mat3(mat_view) * N, length(view_vec)*2);
    out_extra = vec4(0.0, 0.0, 0.0, 1.0);
}



