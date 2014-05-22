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
out vec3 normal;
out vec4 tangent;
out vec2 tcoord;
out vec3 view_vec;
out vec3 view_normal;


void main()
{
    mat3 nmat = mat3(mat_model);

    vec4 pos = mat_model * vec4(in_position, 1.0);

    position = pos.xyz;
    normal = nmat * in_normal;
    tangent = vec4(nmat * in_tangent.xyz, in_tangent.w);
    tcoord = in_tex_coord;
    view_vec = position - camera_pos;
    view_normal = mat3(mat_view) * normal;

    gl_Position = mat_proj * mat_view * pos;
}

