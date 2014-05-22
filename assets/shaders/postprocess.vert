#version 330

in vec2 in_position;
in vec2 in_tex_coord;

out vec2 tcoord;

void main()
{
    tcoord = in_tex_coord;

    gl_Position = vec4(in_position, 0.0, 1.0);
}
