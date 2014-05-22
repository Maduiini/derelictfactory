#version 330

uniform sampler2D tex_color;
uniform sampler2D tex_normal;

in vec2 tcoord;

out vec4 out_color;

void main()
{
    out_color = vec4(texture2D(tex_normal, tcoord).rgb, 1.0);
}
