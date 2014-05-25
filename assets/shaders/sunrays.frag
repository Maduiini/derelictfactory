#version 330

uniform sampler2D tex_color;
uniform sampler2D tex_normal;
uniform sampler2D tex_extra;

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

float random(vec2 p)
{
    const vec2 r = vec2(23.1406926327792690, 2.6651441426902251);
    return fract(cos(mod(123456789.0, 1e-7 + 256.0 * dot(p, r))));
}

#include "light_uniforms.glsl"

vec3 glow()
{
    // todo: send sun_position from application
//    const vec4 sun_position = vec4(-82.2, 69.4, -34.4, 1.0);
    vec3 sun_pos = lights[0].position.xyz;
    vec4 sun_position = vec4(sun_pos, 1.0);

    vec4 sun_CS = mat_proj * mat_view * sun_position;
    vec3 sun_NDC = sun_CS.xyz / sun_CS.w;
    vec2 sun_SS = (sun_NDC.xy + vec2(1.0)) * 0.5;

    const int samples = 64;
    float decay = 0.94975;
    float intensity = 0.525;
    vec2 temp_uv = tcoord;

    // ---------------------
    // with random offset
    vec2 random_offset = 0.002 * vec2(random(tcoord), random(tcoord + vec2(0.333, 0.444)));
    vec2 direction = sun_SS - temp_uv + random_offset;

    // without random offset
    //vec2 direction = sun_SS - temp_uv;
    // ---------------------

    direction /= float(samples);
    vec3 color = texture(tex_color, temp_uv).rgb;

    for (int i=0; i<samples; i++) {
        color += texture(tex_color, temp_uv).rgb * texture(tex_extra, temp_uv).g * intensity;
        intensity *= decay;
        temp_uv += direction;
    }

    return color;
}

void main()
{
//    float ao = 1.0
    float ao = texture(tex_extra, tcoord).r;
    out_color = vec4(glow() * ao, 1.0);
    out_normal = texture(tex_normal, tcoord);
}
