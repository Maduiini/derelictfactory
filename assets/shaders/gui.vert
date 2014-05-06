#version 330

uniform vec2 gui_scale;
uniform vec2 gui_position;

in vec2 in_position;

out vec2 tcoord;

void main()
{
    tcoord = (in_position + 1.0) * 0.5;

    // Moves the origin of the quad from the center to the top-left corner of the quad
    vec2 origin_fix = vec2(in_position.x + 1.0, in_position.y - 1.0) * gui_scale;

    gl_Position = vec4(vec2(-1.0, 1.0) + origin_fix + vec2(gui_position.x * 2.0f, -gui_position.y * 2.0f), 0.0, 1.0);
}
