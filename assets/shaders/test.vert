
#version 330

uniform mat4 mat_proj;
uniform mat4 mat_view;
uniform mat4 mat_model;

in vec3 in_position;
in vec3 in_normal;
in vec4 in_tangent;
in vec2 in_tex_coord;

out vec2 tex_coord;

void main()
{
    vec4 pos = vec4(in_position, 1.0);

    tex_coord = in_tex_coord;

    mat4 mvp = mat_proj * mat_view * mat_model;
    gl_Position = mvp * pos;
}
