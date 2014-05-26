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
    float sm_enabled; // Shadow map enabled
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
    return der_get_normal(tex_normal, tcoord, normal, tangent);
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

void main()
{
    vec3 albedo = get_albedo();
//    out_color = vec4(albedo, 1.0);
//    return;

//    vec3 N = normal;
    vec3 N = normalize(mix(normalize(normal), get_normal(), nm_influence));
    vec3 V = normalize(view_vec);

    float m = texture(tex_metallic, tcoord).x;
    float r = texture(tex_roughness, tcoord).x;

    vec3 c_diff = mix(albedo, vec3(0.04), m);
    vec3 c_spec = mix(vec3(0.04), albedo, m);

    vec3 color = lighting(c_diff, c_spec, N, V, r);

    out_color = gamma_correct(color);
    out_normal = vec4(normalize(view_normal), length(view_vec));
    out_extra = vec4(0.0, 0.0, 0.0, 1.0);
//    out_normal = vec4(mat3(mat_view) * N, length(view_vec));
}

