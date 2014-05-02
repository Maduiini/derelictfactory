
#version 330

layout(row_major) uniform Globals
{
    mat4 mat_proj;
    mat4 mat_view;
    float time;
};

layout(row_major) uniform Instance
{
    mat4 mat_model;
};

in vec3 in_position;
in vec3 in_color;

out vec3 color;

void main()
{
    color = in_color;

    mat4 mvp = mat_proj * mat_view * mat_model;
    gl_Position = mvp * vec4(in_position, 1.0);
}
