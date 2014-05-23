#version 330

layout(row_major) uniform Globals
{
    mat4 mat_proj;
    mat4 mat_view;
    vec3 camera_pos;
    float time;
};

layout(row_major) uniform Instance
{
    mat4 mat_model;
};


in vec3 in_position;
in vec3 in_normal;
in vec4 in_tangent;
in vec2 in_tex_coord;

out vec3 position;
out vec3 view_vec;

void main()
{
    vec4 pos = mat_model * vec4(in_position, 1.0);

    position = pos.xyz;
    view_vec = camera_pos - pos.xyz;

    gl_Position = mat_proj * mat_view * pos;
}


