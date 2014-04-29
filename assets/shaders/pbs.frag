
#version 330

#define MAX_LIGHTS 16

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
    int light_count;
};

layout(row_major) uniform Lights
{
    vec4 position;      // Position(w=1) or direction(w=0)
    vec4 color_energy;  // rgb = color, w = energy
    float radius;
} lights[MAX_LIGHTS];


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
    return vec3(1.0);
}

vec3 lighting(vec3 N)
{
    vec3 L = vec3(0.0);
    for (int i = 0; i < light_count; i++)
    {
        L += light(i, N);
    }
    return L;
}

void main()
{
    vec3 color = texture2D(tex_albedo, tcoord).rgb;
    vec3 n = get_normal();


    vec3 l = normalize(vec3(1.0, 1.0, 0.0));
    float NoL = max(dot(n, l), 0.0);

    vec3 l2 = normalize(vec3(-1.5, -1.0, -0.5));
    float NoL2 = max(dot(n, l2), 0.0);

    vec3 c_l1 = vec3(1.0, 1.0, 0.85);
    vec3 c_l2 = vec3(0.25, 0.5, 0.65);
    color = color * (NoL * c_l1 + NoL2 * c_l2);


    out_color = vec4(color, 1.0);
}

