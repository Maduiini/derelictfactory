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

out vec3 position;
out vec3 normal;
out vec4 tangent;
out vec2 tcoord;
out vec3 view_vec;
out vec3 view_normal;

mat3 adjoint_tr(mat3 m)
{
    mat3 result;
	result[0][0] = m[2][2] * m[1][1] - m[1][2] * m[2][1];
	result[0][1] = m[1][2] * m[2][0] - m[1][0] * m[2][2];
	result[0][2] = m[1][0] * m[2][1] - m[2][0] * m[1][1];

	result[1][0] = m[0][2] * m[2][1] - m[2][2] * m[0][1];
	result[1][1] = m[2][2] * m[0][0] - m[0][2] * m[2][0];
	result[1][2] = m[2][0] * m[0][1] - m[0][0] * m[2][1];

	result[2][0] = m[1][2] * m[0][1] - m[0][2] * m[1][1];
	result[2][1] = m[1][0] * m[0][2] - m[1][2] * m[0][0];
	result[2][2] = m[0][0] * m[1][1] - m[1][0] * m[0][1];
	return result;
}

void main()
{
    mat3 nmat = adjoint_tr(mat3(mat_model));

    vec4 pos = mat_model * vec4(in_position, 1.0);

    position = pos.xyz;
    normal = nmat * in_normal;
    tangent = vec4(nmat * in_tangent.xyz, in_tangent.w);
    tcoord = in_tex_coord;
    view_vec = camera_pos - position;
    view_normal = mat3(mat_view) * normal;

    gl_Position = mat_proj * mat_view * pos;
}

