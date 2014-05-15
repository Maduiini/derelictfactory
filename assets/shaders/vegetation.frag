#version 330

uniform sampler2D tex_albedo;
uniform sampler2D tex_normal;
uniform sampler2D tex_roughness;
uniform sampler2D tex_metallic;
uniform samplerCube tex_env;

layout(row_major) uniform Globals
{
    mat4 mat_proj;
    mat4 mat_view;
    vec3 camera_pos;
    float time;
};

uniform Params
{
    float nm_influence; // Normalmap influence
};

struct Light
{
    vec4 position;      // Position(w=1) or direction(w=0)
    vec4 direction;     // xyz = direction, w = cos(spot_angle)
    vec4 color_energy;  // rgb = color, w = energy
    float radius;
};

#define MAX_LIGHTS 16
layout(row_major) uniform Lights
{
    int light_count;
    Light lights[MAX_LIGHTS];
};


in vec3 position;
in vec3 normal;
in vec4 tangent;
in vec2 tcoord;
in vec3 view_vec;

out vec4 out_color;

mat3 tangent_space()
{
    vec3 n = normalize(normal);
    vec3 t = normalize(tangent.xyz);
    vec3 bt = cross(n, t) * tangent.w;
    return mat3(t, bt, n);
}

vec3 get_normal()
{
    vec3 n = texture(tex_normal, tcoord).xyz * vec3(2.0, 2.0, 1.0) - vec3(1.0, 1.0, 0.0);
    return tangent_space() * n;
}

vec4 gamma_correct(vec3 color)
{
    vec3 s1 = sqrt(color);
    vec3 s2 = sqrt(s1);
    vec3 s3 = sqrt(s2);
    vec3 srgb = 0.662002687 * s1 + 0.684122060 * s2 - 0.3235601 * s3 - 0.225411470 * color;
    return vec4(srgb, 1.0);

    // An expensive approximation
//    return vec4(pow(color, vec3(1.0/2.23333)), 1.0);
}

vec3 linearize(vec3 color)
{
    // The official sRGB linearization. Very inefficient.
//    vec3 c1 = pow((color + vec3(0.055) / 1.055), vec3(2.4));
//    vec3 c2 = color / 12.92;
//    return mix(c1, c2, lessThanEqual(color, vec3(0.04045)));

    // A fair approximation
//    return pow(color, vec3(2.23333));

    // A very crude approximation
//    return color * color;

    // A good and fast approximation
    return color * (color * (color * 0.305306011 + 0.682171111) + 0.012522878);
}

vec4 get_albedo()
{
    vec4 color = texture(tex_albedo, tcoord);
    // linearize gamma
    return vec4(linearize(color.rgb), color.a);
}

vec3 get_env(vec3 v, float lod)
{
    vec3 color = textureLod(tex_env, v, lod).rgb;
    // linearize gamma
    return linearize(color);
}


const float PI = 3.14;
const float ONE_OVER_PI = 1.0 / PI;

// --- Physically based shading

// Diffuse term
vec3 diffuse_BRDF(const vec3 c_diff)
{
    return c_diff * ONE_OVER_PI;
}

// The Cook-Torrance microfacet specular shading model:

//
//           D(h) F(v, h) G(l, v, h)
// f(l, v) = ----------------------
//                4 (n|l) (n|v)
//
// D = normal distribution function (NDF)
// F = Fresnel term
// F = Geometric attenuation
//
// (x|y) means dot product
// n = surface normal
// l = light direction
// v = view direction
// h = half vector = normalized(l + v)

// Normal distribution function
float D_GGX_Trowbridge_Reitz(const float alpha, const float NoH)
{
    float a2 = alpha * alpha + 0.05;
    float x = NoH * NoH * (a2 - 1.0) + 1.0;
    return a2 / (PI * x * x);
}

// Epic games' approximation of the usual Schlick's Fresnel term.
vec3 F_Schlick_Epic(const vec3 c_spec, const float VoH)
{
    return c_spec + (vec3(1.0) - c_spec) * exp2((-5.55473 * VoH - 6.98316) * VoH);
}

float G1_Smith(const float XoN, const float k)
{
    return XoN / (XoN * (1.0 - k) + k);
}

// Smith's geometric attenuation
float G_Smith(const float NoL, const float NoV, const float roughness)
{
    float r = roughness + 1.0;
    float k = r * r / 8.0;
    return G1_Smith(NoL, k) * G1_Smith(NoV, k);
}

vec3 specular_BRDF(const vec3 c_spec, const float NoL, const float NoH, const float NoV, const float VoH, const float roughness)
{
    float alpha = roughness * roughness;
    float D = D_GGX_Trowbridge_Reitz(alpha, NoH);
    vec3 F = F_Schlick_Epic(c_spec, VoH);
    float G = G_Smith(NoL, NoV, roughness);
    return (D * F * G) / (4.0 * NoV); // NoL divider has been rearrangd out
}

vec3 BRDF(vec3 c_diff, vec3 c_spec, const vec3 N, const vec3 L, const vec3 V, const float roughness)
{
    vec3 H = normalize(L + V);

    float NoL = dot(N, L);
//    if (NoL < 0.0) return vec3(0.0);

    float NoH = clamp(dot(N, H), 0.0, 1.0);
    float NoV = clamp(dot(N, V), 0.001, 1.0);
    float VoH = max(dot(V, H), 0.0);

    vec3 color = diffuse_BRDF(c_diff) * NoL;
    color += specular_BRDF(c_spec, NoL, NoH, NoV, VoH, roughness);
    return color;
}


vec3 light(const int i, vec3 c_diff, vec3 c_spec, const vec3 N, const vec3 V, const float roughness)
{
    vec4 pos = lights[i].position;
    vec4 dir = lights[i].direction;
    vec3 L = mix(dir.xyz, normalize(pos.xyz - position), pos.w);

    float r = lights[i].radius;
    float cos_spot = dir.w;
//    cos_spot = 0.8;

    float NoL = dot(N, L);
//    float spot_f = smoothstep(0.0, cos_spot, abs(dot(dir.xyz, L)));
//    float spot_f = float(cos_spot < dot(dir.xyz, L));
//    float spot_f = smoothstep(0.0, abs(cos_spot), abs(dot(dir.xyz, L)));
    float spot_f = dot(dir.xyz, L);
//    spot_f *= float(spot_f > cos_spot);
//    if (spot_f < cos_spot*2)
        spot_f = smoothstep(cos_spot-0.02, cos_spot+0.002, spot_f);
//    else
//        spot_f = 1.0;
    spot_f = mix(1.0, spot_f, cos_spot > 0.0);
    if (NoL * r * spot_f <= 0.0) return vec3(0.0);
//    if (pos.w <= 0.0) return vec3(0.0);

    float dist = distance(pos.xyz, position);

//    const float df = 0.05;
    const float df = 0.5;
//    const float df = 1.0;
//    float v = max(1.0 - pow(dist2 / (r * r), 2), 0.0);
    float dist2 = dist * dist * df;
    float x = dist2 / (r * r); // * df);
    float v = max(1.0 - x * x, 0.0);
    float attenuation = mix(1.0, (v * v) / (dist2 + 0.5), pos.w);
    attenuation *= spot_f;

    vec3 color = BRDF(c_diff, c_spec, N, L, V, roughness);
    vec4 lcolor = lights[i].color_energy;
    return color * lcolor.rgb * lcolor.w * attenuation;
}

vec3 IBL_diffuse(const vec3 c_diff, const vec3 N, const vec3 V, const float roughness)
{
    vec3 L = -N;

    float lod = 5.0 + roughness * 5.0;
//    vec3 color = linearize(textureLod(tex_env, L, lod).rgb);
    vec3 color = get_env(L, lod);

    float NoL = 0.225; //max(dot(N, L), 0.0);
    return color * c_diff * NoL;// * 0.25;
}

vec3 IBL_specular(const vec3 c_spec, const vec3 N, const vec3 V, const float roughness)
{
    vec3 L = -reflect(V, N) * vec3(1.0, -1.0, 1.0);

    float lod = roughness * 5.0;
//    vec3 color = linearize(textureLod(tex_env, L, lod).rgb);
    vec3 color = get_env(L, lod);

    float NoL = max(dot(N, L), 0.0);
    return color * c_spec * NoL; // * 0.25;
//    return color * c_spec * mix(NoL, 1.0, 0.5) * 0.25;
//    return color * c_spec * 0.25 * (NoL * 0.5 + 0.5);
}

vec3 IBL(const vec3 c_diff, const vec3 c_spec, const vec3 N, const vec3 V, const float roughness)
{
    return IBL_diffuse(c_diff, N, V, roughness) +
        IBL_specular(c_spec, N, V, roughness);
}

vec3 lighting(vec3 c_diff, vec3 c_spec, const vec3 N, const vec3 V, const float roughness)
{
    vec3 color = vec3(0.0);
    for (int i = 0; i < light_count; i++)
    {
        color += light(i, c_diff, c_spec, N, V, roughness);
    }
//    color += light(0, c_diff, c_spec, N, V, roughness);
//    color += light(1, c_diff, c_spec, N, V, roughness);
//    color += light(2, c_diff, c_spec, N, V, roughness);
//    color += light(3, c_diff, c_spec, N, V, roughness);
//    color += light(4, c_diff, c_spec, N, V, roughness);
//    color += light(5, c_diff, c_spec, N, V, roughness);
//    color += light(6, c_diff, c_spec, N, V, roughness);
//    color += light(7, c_diff, c_spec, N, V, roughness);
//    color += light(8, c_diff, c_spec, N, V, roughness);
//    color += light(9, c_diff, c_spec, N, V, roughness);
//    color += light(10, c_diff, c_spec, N, V, roughness);
//    color += light(11, c_diff, c_spec, N, V, roughness);
//    color += light(12, c_diff, c_spec, N, V, roughness);
//    color += light(13, c_diff, c_spec, N, V, roughness);
//    color += light(14, c_diff, c_spec, N, V, roughness);
//    color += light(15, c_diff, c_spec, N, V, roughness);

    color += IBL(c_diff, c_spec, N, V, roughness);
    return color;
}

vec3 back_lighting(vec3 c_diff, const vec3 N, const vec3 V, const float roughness)
{
    vec3 color = vec3(0.0);
    for (int i = 0; i < light_count; i++)
    {
        color += light(i, c_diff, vec3(0.0), N, V, roughness);
    }
//    color += light(0, c_diff, c_spec, N, V, roughness);
//    color += light(1, c_diff, c_spec, N, V, roughness);
//    color += light(2, c_diff, c_spec, N, V, roughness);
//    color += light(3, c_diff, c_spec, N, V, roughness);
//    color += light(4, c_diff, c_spec, N, V, roughness);
//    color += light(5, c_diff, c_spec, N, V, roughness);
//    color += light(6, c_diff, c_spec, N, V, roughness);
//    color += light(7, c_diff, c_spec, N, V, roughness);
//    color += light(8, c_diff, c_spec, N, V, roughness);
//    color += light(9, c_diff, c_spec, N, V, roughness);
//    color += light(10, c_diff, c_spec, N, V, roughness);
//    color += light(11, c_diff, c_spec, N, V, roughness);
//    color += light(12, c_diff, c_spec, N, V, roughness);
//    color += light(13, c_diff, c_spec, N, V, roughness);
//    color += light(14, c_diff, c_spec, N, V, roughness);
//    color += light(15, c_diff, c_spec, N, V, roughness);

    color += IBL_diffuse(c_diff, N, V, roughness);
    return color;
}

void main()
{
    vec4 albedo = get_albedo();
    if (albedo.a < 0.55) discard;

    vec3 N = normal; //get_normal();
//    vec3 N = normalize(mix(normal, get_normal(), nm_influence));
    vec3 V = -normalize(view_vec);

    if (!gl_FrontFacing)
    {
        N *= -1.0;
    }
//    out_color = vec4(N, 1.0);
//    return;

    float m = texture(tex_metallic, tcoord).x;
    float r = texture(tex_roughness, tcoord).x;

    vec3 c_diff = mix(albedo.rgb, vec3(0.04), m);
    vec3 c_spec = mix(vec3(0.04), albedo.rgb, m);

    vec3 color = lighting(c_diff, c_spec, N, V, r);
    vec3 back_color = back_lighting(c_diff, -N, V, r);
    color += back_color * albedo.a;

    // gamma corrected output
    out_color = gamma_correct(color) * vec4(1.0, 1.0, 1.0, albedo.a);
}

