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

vec3 light(const int i, vec3 c_diff, vec3 c_spec, const vec3 N, const vec3 V, const float roughness)
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
    float x = dist2 / (r * r);
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

    color += IBL(c_diff, c_spec, N, V, roughness);
    return color;
}

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
    if (albedo.a < 0.55) discard;

//    vec3 N = normal; //get_normal();
    vec3 N = normalize(mix(normal, get_normal(), nm_influence));
    vec3 V = -normalize(view_vec);

    N = mix(-N, N, gl_FrontFacing);
//    out_color = vec4(N, 1.0);
//    return;

    float m = texture(tex_metallic, tcoord).x;
    float r = texture(tex_roughness, tcoord).x;

    vec3 c_diff = mix(albedo.rgb, vec3(0.04), m);
    vec3 c_spec = mix(vec3(0.04), albedo.rgb, m);

    vec3 color = lighting(c_diff, c_spec, N, V, r);
    vec3 back_color = back_lighting(c_diff, -N, V, r);
    color += back_color * albedo.a;

    // gamma corrected output
    out_color = gamma_correct(color) * vec4(1.0, 1.0, 1.0, albedo.a);
}

