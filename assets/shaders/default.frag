
#version 330

uniform sampler2D tex_albedo;

in vec2 tcoord;
out vec4 out_color;

void main()
{
    vec3 color = texture2D(tex_albedo, tcoord).rgb;
    vec3 magenta = vec3(1.0, 0.0, 1.0);
    out_color = vec4(color * magenta, 1.0);
}

