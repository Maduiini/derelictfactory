#version 330

uniform sampler2D tex_albedo;

layout(row_major) uniform Globals
{
    mat4 mat_proj;
    mat4 mat_view;
    vec3 camera_pos;
    float time;
};

in vec2 tcoord;

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_normal;
layout (location = 2) out vec4 out_extra;

void main()
{
    out_color = vec4(1.0, 0.9, 0.8, 1.0);
    out_extra = vec4(0.0, 1.0, 0.0, 1.0);
}
