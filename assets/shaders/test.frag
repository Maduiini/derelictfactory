
#version 330

uniform sampler2D tex_color;
uniform sampler2D tex_normal;

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

void main()
{
    vec3 color = texture2D(tex_color, tcoord).rgb;

    vec3 n = tangent_space() * texture2D(tex_normal, tcoord).xyz;
//    vec3 n = normalize(normal);

    vec3 l = normalize(vec3(1.0, 1.0, 0.0));
    float NoL = max(dot(n, l), 0.0) + 0.2;

    vec3 l2 = normalize(vec3(1.5, -1.0, -0.5));
    float NoL2 = max(dot(n, l2), 0.0);

    vec3 c_l1 = vec3(1.0, 1.0, 0.75);
    vec3 c_l2 = vec3(0.5, 0.75, 1.0);

    out_color = vec4(color * (NoL * c_l1 + NoL2 * c_l2), 1.0);
}
