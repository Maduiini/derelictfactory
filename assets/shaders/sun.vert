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

#include "light_uniforms.glsl"


in vec3 in_position;
in vec2 in_tex_coord;

out vec2 tcoord;


void main()
{
    tcoord = in_tex_coord;

    vec3 pos = in_position + lights[0].position.xyz;

    mat4 mvp = mat_proj * mat_view; // * mat_model;
    gl_Position = mvp * vec4(pos, 1.0);
}


