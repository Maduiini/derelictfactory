#version 330

in vec3 color;

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_normal;
layout (location = 2) out vec4 out_extra;

void main()
{
    out_color = vec4(color, 1.0);
    out_extra = vec4(0.0, 0.0, 0.0, 1.0);
}
