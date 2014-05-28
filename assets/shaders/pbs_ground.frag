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

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_normal;


vec3 waveN()
{
    return vec3(0.0, 1.0, 0.0);
}

vec2 get_tex_coord()
{
    return tcoord * 50.0;
}

vec2 get_tex_coord2()
{
    return tcoord * 6.0;
}

vec3 get_normal(float wet)
{
    vec3 n1 = der_get_normal(tex_normal, get_tex_coord(), normal, tangent);
    vec3 n2 = der_get_normal(tex_normal, get_tex_coord2(), normal, tangent);
    return mix(n1 + n2, waveN(), wet);
}

vec3 get_albedo(vec4 mask)
{
    vec3 albedo1 = der_get_albedo(tex_albedo, get_tex_coord());
    vec3 albedo2 = der_get_albedo(tex_albedo, get_tex_coord2());
    vec3 albedo_grass = der_get_albedo(tex_metallic, get_tex_coord());

    vec3 mud = mix(vec3(1.0, 1.0, 1.0), vec3(0.55, 0.5, 0.48), mask.x);
//    return mix(albedo1, albedo2, 0.5) * mud;
    vec3 albedo = mix(albedo1, albedo2, 0.5) * mud;
    albedo = mix(albedo, albedo_grass, mask.y);
    return albedo;
}

#include "pbs.glsl"

void main()
{
    float d = length(view_vec);

    vec4 mask = texture(tex_roughness, tcoord);
    float wet = mask.b;
//    float wet = 1.0f - texture(tex_roughness, tcoord).x; // * 2.5e-2 + vec2(0.005)).x;

    vec3 N = normalize(mix(normal, get_normal(wet), nm_influence));
    vec3 V = normalize(view_vec);

//    vec2 offset = normalize((der_tangent_space(normal, tangent) * V)).xz;
    vec3 albedo = get_albedo(mask); //wet * offset * 0.0); //min(d / 60.0, 0.8));

    float r = 1.0 - wet;

    float waterlevel = 0.2;
//    float abso = 1.0 - (waterlevel - clamp(position.y + waterlevel, -1.0, 0.25));
    float abso = 1.2 - (waterlevel - clamp(position.y + waterlevel, -1.0, waterlevel));
    vec3 absorbtion = vec3(0.95, 0.97, 1.0) * abso * abso * abso;

    albedo *= absorbtion;

    vec3 c_diff = albedo; //mix(albedo, albedo * vec3(0.95), wet);
//    vec3 c_diff = mix(albedo, albedo * vec3(0.91), wet);
    vec3 c_spec = mix(vec3(0.04), vec3(0.04037), wet);

    vec3 color = lighting(c_diff, c_spec, N, V, r);

    out_color = gamma_correct(color);
    out_normal = vec4(normalize(view_normal), length(view_vec));
}


