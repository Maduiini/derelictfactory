
#version 330

uniform sampler2D tex_color;
in vec2 tex_coord;

out vec4 out_color;

void main()
{
    vec4 color = texture2D(tex_color, tex_coord);
    out_color = color;
}
