#version 330

layout(row_major) uniform Globals
{
    mat4 mat_proj;
    mat4 mat_view;
    vec3 camera_pos;
    float time;
    float window_width;
    float window_height;
};

layout(row_major) uniform Instance
{
    mat4 mat_model;
};


in vec3 in_position;
in vec3 in_normal;
in vec4 in_tangent;
in vec2 in_tex_coord;

out vec3 view_vec;

void main()
{
    vec4 pos = mat_model * vec4(in_position, 1.0);

    view_vec = camera_pos - pos.xyz;
    view_vec = mat3(mat_model) * view_vec;

    gl_Position = mat_proj * mat_view * pos;
}


