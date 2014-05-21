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

out vec4 out_color;

vec3 get_normal(float wet)
{
    return mix(der_get_normal(tex_normal, tcoord, normal, tangent), normal, wet);
//    vec3 n0 = texture(tex_normal, tcoord).xyz * vec3(2.0, 2.0, 1.0) - vec3(1.0, 1.0, 0.0);
//    vec3 n1 = texture(tex_normal, tcoord * 1e-1).xyz * vec3(2.0, 2.0, 1.0) - vec3(1.0, 1.0, 0.0);
//    float mask = texture2D(tex_metallic, tcoord * 1e-2 + vec2(0.5)).x;
//    vec3 n = normalize(mix(n0, n1, mask * mask));
//    return tangent_space() * n;
}

vec3 get_albedo(vec2 offset) //float d)
{
    return der_get_albedo(tex_albedo, tcoord + offset);
//    vec3 color0 = texture(tex_albedo, tcoord).rgb;
//    vec3 color01 = texture(tex_albedo, tcoord * 1.258e-1).rgb;
//    vec3 color1 = color01 * vec3(0.5, 0.6, 0.4);
////    vec3 color1 = texture(tex_roughness, tcoord).rgb;
//    float mask = texture(tex_metallic, tcoord * 1e-2 + vec2(0.5)).x;
//    vec3 color = mix(mix(color0, color01, d), color1, mask);
////    vec3 color = vec3(mask);
//    // linearize gamma
//    return linearize(color);
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

    float wet = 1.0f - texture(tex_roughness, tcoord * 2.5e-2 + vec2(0.005)).x;


    vec3 N = normalize(mix(normal, get_normal(wet), nm_influence));
    vec3 V = -normalize(view_vec);

    vec2 offset = normalize((der_tangent_space(normal, tangent) * V)).xz;
    vec3 albedo = get_albedo(wet * offset * 0.0); //min(d / 60.0, 0.8));

//    wet *= 0.65;

//    float m = 1.0 - wet; //0.0; //texture(tex_metallic, tcoord).x;
    float r = 1.0 - wet;

    vec3 c_diff = mix(albedo, albedo * vec3(0.95), wet);
//    vec3 c_spec = mix(vec3(0.04), albedo, wet);
//    vec3 c_spec = mix(vec3(0.04), vec3(1.0), wet);
//    vec3 c_spec = mix(vec3(0.04), vec3(0.02037), wet);
    vec3 c_spec = mix(vec3(0.04), vec3(0.04037), wet);

    vec3 color = lighting(c_diff, c_spec, N, V, r);

    // gamma corrected output
    out_color = vec4(pow(color, vec3(1.0 / 2.2)), 1.0);
}


