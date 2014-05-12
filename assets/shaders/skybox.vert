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

out vec3 view_vec;

void main()
{
    vec4 pos = mat_model * vec4(in_position, 1.0);

    view_vec = pos.xyz - camera_pos;

    gl_Position = mat_proj * mat_view * pos;
}


