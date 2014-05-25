#version 330

uniform sampler2D tex_albedo;
uniform sampler2D tex_normal;
uniform sampler2D tex_roughness;
uniform sampler2D tex_metallic;
uniform samplerCube tex_env;


in vec3 view_vec;

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_normal;
layout (location = 2) out vec4 out_extra;

vec3 get_color()
{
    vec3 color = texture(tex_env, view_vec * vec3(-1.0, 1.0, -1.0)).rgb;
    return color;
}

void main()
{
    vec3 color = get_color();
    // gamma corrected output
    out_color = vec4(color, 1.0);
    out_extra = vec4(0.0, 0.0, 0.0, 1.0);
}


