
#version 330

uniform sampler2D tex_albedo;
uniform sampler2D tex_normal;
uniform sampler2D tex_roughness;
uniform sampler2D tex_metallic;
uniform samplerCube tex_env;

layout(row_major) uniform Globals
{
    mat4 mat_proj;
    mat4 mat_view;
    float time;
};

layout(row_major) uniform Instance
{
    mat4 mat_model;
    int light_count;
};

struct Light
{
    vec4 position;      // Position(w=1) or direction(w=0)
    vec4 color_energy;  // rgb = color, w = energy
    float radius;
};

#define MAX_LIGHTS 16
layout(row_major) uniform Lights
{
    Light lights[MAX_LIGHTS];
};


in vec3 position;
in vec3 normal;
in vec4 tangent;
in vec2 tcoord;

out vec4 out_color;

mat3 tangent_space()
{
    vec3 n = normalize(normal);
    vec3 t = normalize(tangent.xyz);
    vec3 bt = cross(n, t) * tangent.w;
    return mat3(t, bt, n);
}

vec3 get_normal()
{
    vec3 n = texture2D(tex_normal, tcoord).xyz * vec3(2.0, 2.0, 1.0) - vec3(1.0, 1.0, 0.0);
    return tangent_space() * n;
}

vec3 light(int i, vec3 N)
{
    vec4 pos = lights[i].position;
    vec3 L = pos.xyz - (position * pos.w);

    L = normalize(L);
    float NoL = max(0.0, dot(N, L));

    float r = lights[i].radius;
    float dist = distance(pos.xyz, position);

    float dist2 = dist * dist;
    float v = max(1.0 - pow(dist2 / (r * r), 2), 0.0);
    float attenuation = mix(1.0, (v * v) / (dist2 + 1.0), pos.w);

    vec4 color = lights[i].color_energy;
    return NoL * color.rgb * color.w * attenuation;
}

vec3 lighting(vec3 N)
{
    vec3 color = vec3(0.0);
    for (int i = 0; i < light_count; i++)
    {
        color += light(i, N);
    }
    return color;
}

void main()
{
    vec3 color = texture2D(tex_albedo, tcoord).rgb;
    vec3 N = get_normal();

    color = color * lighting(N);

    out_color = vec4(color, 1.0);
}

