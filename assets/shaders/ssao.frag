#version 330

uniform sampler2D tex_color;
uniform sampler2D tex_normal;
uniform sampler2D tex_extra;

in vec2 tcoord;

layout (location = 0) out vec4 out_color;
layout (location = 1) out vec4 out_normal;
layout (location = 2) out vec4 out_extra;

vec3 random_points[16] = vec3[](vec3(0.53812504, 0.18565957, -0.43192), vec3(0.13790712, 0.24864247, 0.44301823),
                                vec3(0.33715037, 0.56794053, -0.005789503), vec3(-0.6999805, -0.04511441, -0.0019965635),
                                vec3(0.06896307, -0.15983082, -0.85477847), vec3(0.056099437, 0.006954967, -0.1843352),
                                vec3(-0.014653638, 0.14027752, 0.0762037), vec3(0.010019933, -0.1924225, -0.034443386),
                                vec3(-0.35775623, -0.5301969, -0.43581226), vec3(-0.3169221, 0.106360726, 0.015860917),
                                vec3(0.010350345, -0.58698344, 0.0046293875), vec3(-0.08972908, -0.49408212, 0.3287904),
                                vec3(0.7119986, -0.0154690035, -0.09183723), vec3(-0.053382345, 0.059675813, -0.5411899),
                                vec3(0.035267662, -0.063188605, 0.54602677), vec3(-0.47761092, 0.2847911, -0.0271716));

float random(vec2 p)
{
    const vec2 r = vec2(23.1406926327792690, 2.6651441426902251);
    return fract(cos(mod(123456789., 1e-7 + 256. * dot(p,r))));
}

float ssao(vec2 base_seed)
{
    const float samples = 8.0;
    const float inv_samples = 1.0 / samples;
    float sum = 0.0;

    vec3 random_vec3 = vec3( random(base_seed), random(base_seed + vec2(0.1, 0.2)), random(base_seed + vec2(0.3, 0.4)) );
    vec3 fres = normalize(random_vec3 * 2.0 - vec3(1.0));

    vec4 this_fragment = texture(tex_normal, tcoord);

    float current_depth = this_fragment.a;
    vec3 current_pos = vec3(tcoord.xy, current_depth);
    vec3 current_normal = this_fragment.xyz;

    const float strength = 0.32;
    const float falloff = 0.000002;
    const float radius = 0.29;

    float radius_d = radius / current_depth;

    vec3 ray, se, occ_normal;
    float occ_depth, depth_difference, normal_difference;

    for (int i=0; i<samples; i++)
    {
//        ray = radius_d * reflect(random_points[i % 16], fres);
        ray = radius_d * reflect(random_points[i & 15], fres);
        se = current_pos + sign(dot(ray, current_normal)) * ray;

        vec4 occluder_fragment = texture(tex_normal, se.xy);
        occ_normal = occluder_fragment.xyz;

        depth_difference = current_depth - occluder_fragment.a;

        normal_difference = 1.0 - dot(occ_normal, current_normal);

        sum += step(falloff, depth_difference) * normal_difference * (1.0 - smoothstep(falloff, strength, depth_difference));
    }

    return 1.0 - 1.3 * sum * inv_samples;
}

void main()
{
    float ao = ssao(gl_FragCoord.xy * 0.000999);
    //out_color = vec4(texture2D(tex_color, tcoord).rgb * ao, 1.0);

    out_color = texture(tex_color, tcoord);
    out_extra = vec4(ao, ao, ao, 1.0);
}
