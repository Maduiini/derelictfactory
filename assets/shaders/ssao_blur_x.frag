#version 330

uniform sampler2D tex_color;
uniform sampler2D tex_normal;
uniform sampler2D tex_extra;

uniform float offset[3] = float[](0.0, 1.3846153846, 3.2307692308);
uniform float weight[3] = float[](0.2270270270, 0.3162162162, 0.0702702703);

//uniform float width = 1024.0;
//uniform float height = 768.0;

layout(row_major) uniform Globals
{
    mat4 mat_proj;
    mat4 mat_view;
    vec3 camera_pos;
    float time;
    float window_width;
    float window_height;
};

in vec2 tcoord;

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_normal;
layout (location = 2) out vec4 out_extra;

void main()
{
    float col = texture(tex_extra, tcoord).r * weight[0];

    float inv_w = 1.0 / window_width;
    for (int i=1; i<3; i++)
    {
        vec2 offs = vec2(offset[i] * inv_w, 0.0);

//        col += texture(tex_extra, tcoord + offs).rgb * weight[i];
//        col += texture(tex_extra, tcoord - offs).rgb * weight[i];

        float sample = texture(tex_extra, tcoord + offs).r;
        sample += texture(tex_extra, tcoord - offs).r;

        col += sample * weight[i];
    }

    out_color = texture(tex_color, tcoord);
    out_normal = texture(tex_normal, tcoord);
    out_extra = texture(tex_extra, tcoord);
    out_extra.r = col;
}
