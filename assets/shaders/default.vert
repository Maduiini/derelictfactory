
#version 330

layout(row_major) uniform Globals
{
    mat4 mat_proj;
    mat4 mat_view;
    mat4 mat_model;
};

in vec3 in_position;
in vec2 in_tex_coord;

out vec2 tcoord;

void main()
{
    vec4 pos = vec4(in_position, 1.0);

    tcoord = in_tex_coord;

    mat4 mvp = mat_proj * mat_view * mat_model;
    gl_Position = mvp * pos;
}

