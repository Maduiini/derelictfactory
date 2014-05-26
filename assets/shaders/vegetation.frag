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


vec3 get_normal()
{
    return der_get_normal(tex_normal, tcoord, normal, tangent);
}

vec4 get_albedo()
{
    return der_get_albedo4(tex_albedo, tcoord);
}


#include "pbs.glsl"

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

//    r = 1.0;
    vec3 color = vege_lighting(c_diff, c_spec, N, V, r);
//    vec3 color = lighting(c_diff, c_spec, N, V, r);
//    vec3 back_color = back_lighting(c_diff, -N, V, r, shadowmap5());
//    color += back_color * albedo.a * clamp(vec3(0.1) + albedo.rgb * 0.5, vec3(0.1), vec3(1.0));

    out_color = gamma_correct(color, albedo.a);
    out_normal = vec4(normalize(view_normal), length(view_vec));
}

