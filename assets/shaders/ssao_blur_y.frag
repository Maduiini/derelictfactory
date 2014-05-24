#version 330

uniform sampler2D tex_color;
uniform sampler2D tex_normal;
uniform sampler2D tex_extra;

uniform float offset[3] = float[](0.0, 1.3846153846, 3.2307692308);
uniform float weight[3] = float[](0.2270270270, 0.3162162162, 0.0702702703);

// todo: send window size from application
uniform float width = 1024.0;
uniform float height = 768.0;

in vec2 tcoord;

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_normal;
layout (location = 2) out vec4 out_extra;

void main()
{
    float col = texture(tex_extra, tcoord).r * weight[0];

    float inv_h = 1.0 / height;
    for (int i=1; i<3; i++)
    {
        vec2 offs = vec2(0.0, offset[i] * inv_h);

//        col += texture(tex_extra, tcoord + offs).rgb * weight[i];
//        col += texture(tex_extra, tcoord - offs).rgb * weight[i];

        float sample = texture(tex_extra, tcoord + offs).r;
        sample += texture(tex_extra, tcoord - offs).r;

        col += sample * weight[i];
    }

    out_color = texture(tex_color, tcoord);
    out_extra = texture(tex_extra, tcoord);
    out_extra.r = col;
}
