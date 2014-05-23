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
    vec4 col = texture(tex_extra, tcoord) * weight[0];

    float inv_w = 1.0 / width;
    for (int i=1; i<3; i++)
    {
        vec2 offs = vec2(offset[i] * inv_w, 0.0);

//        col += texture(tex_extra, tcoord + offs).rgb * weight[i];
//        col += texture(tex_extra, tcoord - offs).rgb * weight[i];

        vec4 sample = texture(tex_extra, tcoord + offs);
        sample += texture(tex_extra, tcoord - offs);

        col += sample * weight[i];
    }

    out_color = texture(tex_color, tcoord);
    out_extra = vec4(col.rgb, 1.0);
}
