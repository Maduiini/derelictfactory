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
    vec3 col = texture2D(tex_extra, tcoord).rgb * weight[0];

    for (int i=1; i<3; i++)
    {
        col += texture2D(tex_extra, tcoord + vec2(0.0, offset[i])/height).rgb * weight[i];
        col += texture2D(tex_extra, tcoord - vec2(0.0, offset[i])/height).rgb * weight[i];
    }

    out_color = vec4(texture2D(tex_color, tcoord).rgb * col.r, 1.0);
}
