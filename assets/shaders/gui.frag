#version 330

uniform sampler2D gui_texture;

in vec2 tcoord;

out vec4 out_color;

void main()
{
    out_color = texture2D(gui_texture, tcoord);
}
