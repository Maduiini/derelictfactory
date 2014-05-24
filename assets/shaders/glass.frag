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

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_normal;
layout (location = 2) out vec4 out_extra;

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

void main()
{
    float d = length(view_vec);

//    vec3 N = normal;
    vec3 N = normalize(mix(normal, get_normal(), nm_influence * 0.02));
    vec3 V = normalize(view_vec);

    vec3 albedo = get_albedo() * 10.0;
    float r = albedo.r;

    vec3 c_spec = (vec3(1.0) - albedo) * vec3(0.18037);
//    vec3 c_spec = vec3(0.18037);
//    vec3 c_spec = vec3(0.08037);
//    vec3 c_spec = vec3(0.04037);
    vec3 c_diff = albedo; //vec3(0.04, 0.02, 0.01);
//    vec3 c_diff = vec3(0.24, 0.22, 0.21);

    vec3 color = lighting(c_diff, c_spec, N, V, r);

    float lum = dot(color, vec3(1.0)) + 0.44 + r;

    out_color = vec4(gamma_correct(color, lum));
    //out_normal = vec4(normalize(view_normal), 0.0);
    out_extra = vec4(0.0, 0.0, 0.0, 1.0);
}



