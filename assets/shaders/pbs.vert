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
in vec3 in_normal;
in vec4 in_tangent;
in vec2 in_tex_coord;

out vec3 normal;
out vec4 tangent;
out vec2 tcoord;

void main()
{
    vec4 pos = vec4(in_position, 1.0);

    mat3 nmat = mat3(mat_model);
    normal = nmat * in_normal;
    tangent = vec4(nmat * in_tangent.xyz, in_tangent.w);
    tcoord = in_tex_coord;

    mat4 mvp = mat_proj * mat_view * mat_model;
    gl_Position = mvp * pos;
}

