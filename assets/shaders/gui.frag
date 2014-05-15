#version 330

uniform sampler2D gui_texture;

in vec2 tcoord;

out vec4 out_color;

void main()
{
    vec4 tex_color = texture(gui_texture, tcoord);
    out_color = tex_color;
}
