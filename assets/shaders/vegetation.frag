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

uniform Params
{
    float nm_influence; // Normalmap influence
};


in vec3 position;
in vec3 normal;
in vec4 tangent;
in vec2 tcoord;
in vec3 view_vec;
in vec3 view_normal;

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_normal;


vec3 get_normal()
{
    return der_get_normal(tex_normal, tcoord, normal, tangent);
}

vec4 get_albedo()
{
    return der_get_albedo4(tex_albedo, tcoord);
}

vec3 get_env(const vec3 v, const float lod)
{
    return der_get_env(tex_env, v, lod);
}


#include "pbs.glsl"

vec3 back_lighting(vec3 c_diff, const vec3 N, const vec3 V, const float roughness)
{
    vec3 color = vec3(0.0);
    for (int i = 0; i < light_count; i++)
    {
        color += light(i, c_diff, vec3(0.0), N, V, roughness);
    }
//    color += light(0, c_diff, c_spec, N, V, roughness);
//    color += light(1, c_diff, c_spec, N, V, roughness);
//    color += light(2, c_diff, c_spec, N, V, roughness);
//    color += light(3, c_diff, c_spec, N, V, roughness);
//    color += light(4, c_diff, c_spec, N, V, roughness);
//    color += light(5, c_diff, c_spec, N, V, roughness);
//    color += light(6, c_diff, c_spec, N, V, roughness);
//    color += light(7, c_diff, c_spec, N, V, roughness);
//    color += light(8, c_diff, c_spec, N, V, roughness);
//    color += light(9, c_diff, c_spec, N, V, roughness);
//    color += light(10, c_diff, c_spec, N, V, roughness);
//    color += light(11, c_diff, c_spec, N, V, roughness);
//    color += light(12, c_diff, c_spec, N, V, roughness);
//    color += light(13, c_diff, c_spec, N, V, roughness);
//    color += light(14, c_diff, c_spec, N, V, roughness);
//    color += light(15, c_diff, c_spec, N, V, roughness);

    color += IBL_diffuse(c_diff, N, V, roughness);
    return color;
}

void main()
{
    vec4 albedo = get_albedo();
    if (albedo.a < 0.25) discard;

//    vec3 N = normal; //get_normal();
//    vec3 N = normalize(position);
//    N = normalize(mix(N, get_normal(), 0.5));
    vec3 N = normalize(mix(normal, get_normal(), nm_influence));
    vec3 V = normalize(view_vec);

    N = mix(-N, N, gl_FrontFacing);
//    out_color = vec4(N, 1.0);
//    return;

    float m = texture(tex_metallic, tcoord).x;
    float r = texture(tex_roughness, tcoord).x;

    vec3 c_diff = mix(albedo.rgb, vec3(0.04), m);
    vec3 c_spec = mix(vec3(0.04), albedo.rgb, m);

    r = 1.0;
    vec3 color = lighting(c_diff, c_spec * 0.0, N, V, r);
    vec3 back_color = back_lighting(c_diff, -N, V, r);
    color += back_color * albedo.a;

    out_color = gamma_correct(color, albedo.a);
    out_normal = vec4(normalize(view_normal), length(view_vec));
}

