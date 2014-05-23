#version 330

#include "common.glsl"

uniform sampler2D tex_albedo;
uniform sampler2D tex_normal;
uniform sampler2D tex_roughness;
uniform sampler2D tex_metallic;
uniform sampler2D tex_env_brdf;
uniform samplerCube tex_env;

layout(row_major) uniform Globals
{
    mat4 mat_proj;
    mat4 mat_view;
    vec3 camera_pos;
    float time;
};

uniform Params
{
    float nm_influence; // Normalmap influence
};

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
    int light_count;
    Light lights[MAX_LIGHTS];
};


in vec3 position;
in vec3 normal;
in vec4 tangent;
in vec2 tcoord;
in vec3 view_vec;
in vec3 view_normal;

out vec4 out_color;
out vec4 out_normal;

vec3 get_normal()
{
    return der_get_normal(tex_normal, tcoord * 2.0e-2, normal, tangent);
}

vec3 get_albedo()
{
    return der_get_albedo(tex_albedo, tcoord);
}

vec3 get_env(const vec3 v, const float lod)
{
    return der_get_env(tex_env, v, lod);
}

#include "pbs.glsl"

vec3 light(const int i, const vec3 c_diff, const vec3 c_spec, const vec3 N, const vec3 V, const float roughness)
{
    vec4 pos = lights[i].position;
    vec4 dir = lights[i].direction;
    vec3 L = mix(dir.xyz, normalize(pos.xyz - position), pos.w);

    float r = lights[i].radius;
    float cos_spot = dir.w;

    float NoL = dot(N, L);
    float spot_f = dot(dir.xyz, L);
    spot_f = smoothstep(cos_spot-0.02, cos_spot+0.002, spot_f);
    spot_f = mix(1.0, spot_f, cos_spot > 0.0);

    if (NoL * r * spot_f <= 0.0) return vec3(0.0);

    float dist = distance(pos.xyz, position);

//    const float df = 0.05;
    const float df = 0.5;
//    const float df = 1.0;
    float dist2 = dist * dist * df;
    float x = dist2 / (r * r); // * df);
    float v = max(1.0 - x * x, 0.0);
    float attenuation = mix(1.0, (v * v) / (dist2 + 0.5), pos.w);
    attenuation *= spot_f;

    vec3 color = BRDF(c_diff, c_spec, N, L, V, roughness);
    vec4 lcolor = lights[i].color_energy;
    return color * lcolor.rgb * lcolor.w * attenuation;
}

vec3 lighting(vec3 c_diff, vec3 c_spec, const vec3 N, const vec3 V, const float roughness)
{
    vec3 color = vec3(0.0);
    for (int i = 0; i < light_count; i++)
    {
        color += light(i, c_diff, c_spec, N, V, roughness);
    }
    color += IBL(c_diff, c_spec, N, V, roughness);
    return color;
}


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

vec3 waveN()
{
    vec3 n = waveN_lin(vec2(0.1, 1.0), 0.005, 2.4)
        + waveN_lin(vec2(0.5, 0.5), 0.005, 1.2);
//        + waveN_circ(vec2(38.0, -35), 0.002, 1.2);
    return normalize(n);
}

void main()
{
    float d = length(view_vec);

//    vec3 N = normal;
//    vec3 N = normalize(mix(normal, get_normal(), nm_influence * 0.02));
    vec3 N = waveN();
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
//    out_normal = vec4(normalize(view_normal), length(view_vec));
//    out_normal = vec4(mat3(mat_view) * N, length(view_vec)*2);
}



