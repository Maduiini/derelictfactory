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
layout (location = 2) out vec4 out_extra;

vec3 get_normal()
{
    return der_get_normal(tex_normal, tcoord * 2.0e-2, normal, tangent);
}

vec3 get_albedo()
{
    return der_get_albedo(tex_albedo, tcoord);
}

#include "pbs.glsl"

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
//    out_normal = vec4(normalize(view_normal), d);
    out_extra = vec4(0.0, 0.0, 0.0, 1.0);
}



