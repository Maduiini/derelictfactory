
uniform sampler2D tex_env_brdf;

#include "light_uniforms.glsl"


vec3 get_env(const vec3 v, const float lod)
{
    return der_get_env(tex_env, v, lod);
}

// Physically based shading
//---------------------------

// Lambertian diffuse term
vec3 diffuse_BRDF(const vec3 c_diff)
{
    return c_diff * ONE_OVER_PI;
}

// The Cook-Torrance micro-facet specular shading model:

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

float G1_Schlick(const float XoN, const float k)
{
    return XoN / (XoN * (1.0 - k) + k);
}

// Schlick's approximation of Smith's geometric attenuation
float G_Schlick(const float NoL, const float NoV, const float roughness)
{
    float r = roughness + 1.0;
    float k = r * r / 8.0;
    return G1_Schlick(NoL, k) * G1_Schlick(NoV, k);
}

vec3 specular_BRDF(const vec3 c_spec, const float NoL, const float NoH, const float NoV, const float VoH, const float roughness)
{
    float alpha = roughness * roughness;
    float D = D_GGX_Trowbridge_Reitz(alpha, NoH);
    vec3 F = F_Schlick_Epic(c_spec, VoH);
    float G = G_Schlick(NoL, NoV, roughness);
    return (D * F * G) / (4.0 * NoV); // NoL divider has been rearrangd out
}

vec3 BRDF(const vec3 c_diff, const vec3 c_spec, const vec3 N, const vec3 L, const vec3 V, const float roughness)
{
    vec3 H = normalize(L + V);

    float NoL = dot(N, L);
    float NoH = clamp(dot(N, H), 0.0, 1.0);
    float NoV = clamp(dot(N, V), 0.001, 1.0);
    float VoH = clamp(dot(V, H), 0.0, 1.0);

    vec3 color = diffuse_BRDF(c_diff) * NoL;
    color += specular_BRDF(c_spec, NoL, NoH, NoV, VoH, roughness);
    return color;
}


// Image based lighting (environment mapping)

// Schlick's approximation of Smith's geometric attenuation
float G_Schlick_IBL(const float NoL, const float NoV, const float roughness)
{
    float r = roughness;
    float k = r * r / 4.0;
    return G1_Schlick(NoL, k) * G1_Schlick(NoV, k);
}

vec3 specular_IBL_BRDF(const vec3 c_spec, const float NoL, const float NoH, const float NoV, const float VoH, const float roughness)
{
    float alpha = roughness * roughness;
    float D = D_GGX_Trowbridge_Reitz(alpha, NoH);
    vec3 F = F_Schlick_Epic(c_spec, VoH);
    float G = G_Schlick_IBL(NoL, NoV, roughness);
    return (D * F * G) / (4.0 * NoV); // NoL divider has been rearrangd out
}

vec3 IBL_BRDF(const vec3 c_diff, const vec3 c_spec, const vec3 N, const vec3 L, const vec3 V, const float roughness)
{
    vec3 H = normalize(L + V);

    float NoL = dot(N, L);
    float NoH = clamp(dot(N, H), 0.0, 1.0);
    float NoV = clamp(dot(N, V), 0.001, 1.0);
    float VoH = clamp(dot(V, H), 0.0, 1.0);

    vec3 color = diffuse_BRDF(c_diff) * NoL;
    color += specular_IBL_BRDF(c_spec, NoL, NoH, NoV, VoH, roughness);
    return color;
}


vec3 IBL_diffuse(const vec3 c_diff, const vec3 N, const vec3 V, const float roughness)
{
    vec3 L = -N;

    float lod = 5.0 + roughness * 5.0;
    vec3 color = get_env(L, lod);

//    float NoL = 0.225; //max(dot(N, L), 0.0);
//    float NoL = 1.0;
    return color * c_diff; // * NoL;// * 0.25;
}

vec3 IBL_specular(const vec3 c_spec, const vec3 N, const vec3 V, const float roughness)
{
    vec3 L = -reflect(V, N) * vec3(1.0, -1.0, 1.0);

//    float lod = roughness * 5.0;
//    float lod = roughness * 20.0;
    float lod = roughness * 10.0;
    vec3 color = get_env(L, lod);

//    float NoL = max(dot(N, L), 0.0);
    return color * c_spec; // * NoL; // * 0.25;
//    return color * c_spec * mix(NoL, 1.0, 0.5) * 0.25;
//    return color * c_spec * 0.25 * (NoL * 0.5 + 0.5);
}

vec3 adhoc_IBL(const vec3 c_diff, const vec3 c_spec, const vec3 N, const vec3 V, const float roughness)
{
    vec3 cd = IBL_diffuse(c_diff, N, V, roughness) * 1.0;
    vec3 cs = IBL_specular(c_spec, N, V, roughness) * 1.0;
    return IBL_BRDF(cd, cs, N, N, N, roughness);

//    return IBL_BRDF(cd, cs, N, N, N, roughness) * ONE_OVER_PI;

//    return IBL_diffuse(c_diff, N, V, roughness) +
//        IBL_specular(c_spec, N, V, roughness);
}


vec3 approx_diff_IBL(const vec3 c_diff, const vec3 N, const vec3 V, const float roughness)
{
    return IBL_diffuse(c_diff, N, V, roughness) * ONE_OVER_PI;
//    return vec3(0.0);
}

vec3 approx_spec_IBL(const vec3 c_spec, const vec3 N, const vec3 V, const float roughness)
{
    float NoV = max(dot(N, V), 0.1);
    vec3 L = -reflect(V, N) * vec3(1.0, -1.0, 1.0);

//    float lod = roughness * 20.0;
//    float lod = roughness * 15.0;
//    float lod = roughness * 15.0;
//    float lod = roughness * 10.0;
    float lod = sqrt(roughness) * 10;
//    float lod = roughness * 5.0;
//    float lod = roughness * 1.5;
    vec3 color = get_env(L, lod);
    vec2 env_brdf = textureLod(tex_env_brdf, vec2(NoV, roughness) * 0.88 + vec2(0.1), 0.0).rg;
//    vec2 env_brdf = texture(tex_env_brdf, vec2(NoV, roughness)).rg;

    return color * (c_spec * env_brdf.x + env_brdf.yyy);
}

vec3 approx_IBL(const vec3 c_diff, const vec3 c_spec, const vec3 N, const vec3 V, const float roughness)
{
    vec3 cd = approx_diff_IBL(c_diff, N, V, roughness);
    vec3 cs = approx_spec_IBL(c_spec, N, V, roughness);
    return cd + cs;
}

vec3 IBL(const vec3 c_diff, const vec3 c_spec, const vec3 N, const vec3 V, const float roughness)
{
    return approx_IBL(c_diff, c_spec, N, V, roughness);
//    return adhoc_IBL(c_diff, c_spec, N, V, roughness);
}

// Lighting

vec3 light(const int i, const vec3 c_diff, const vec3 c_spec, const vec3 N, const vec3 V, const float roughness)
{
    vec4 pos = lights[i].position;
    vec4 dir = lights[i].direction;
    vec3 L = mix(dir.xyz, normalize(pos.xyz - position), pos.w);

    float r = lights[i].radius;
    float cos_spot = dir.w;

    float NoL = dot(N, L);
    float spot_f = dot(dir.xyz, L);
    spot_f = smoothstep(cos_spot-0.02, cos_spot+0.002, spot_f);
    spot_f = mix(1.0, spot_f, cos_spot > 0.0);

    if (NoL * r * spot_f <= 0.0) return vec3(0.0);

    float dist = distance(pos.xyz, position);

//    const float df = 0.05;
    const float df = 0.5;
//    const float df = 1.0;
    float dist2 = dist * dist * df;
    float x = dist2 / (r * r); // * df);
    float v = max(1.0 - x * x, 0.0);
    float attenuation = mix(1.0, (v * v) / (dist2 + 0.5), pos.w);
    attenuation *= spot_f;

    vec3 color = BRDF(c_diff, c_spec, N, L, V, roughness);
    vec4 lcolor = lights[i].color_energy;
    return color * lcolor.rgb * lcolor.w * attenuation;
}

vec3 back_light(const int i, const vec3 c_diff, const vec3 N, const vec3 V, const float roughness)
{
    vec4 pos = lights[i].position;
    vec4 dir = lights[i].direction;
    vec3 L = mix(dir.xyz, normalize(pos.xyz - position), pos.w);

    float r = lights[i].radius;
    float cos_spot = dir.w;

    float NoL = dot(N, L);
    float spot_f = dot(dir.xyz, L);
    spot_f = smoothstep(cos_spot-0.02, cos_spot+0.002, spot_f);
    spot_f = mix(1.0, spot_f, cos_spot > 0.0);

    if (NoL * r * spot_f <= 0.0) return vec3(0.0);

    float dist = distance(pos.xyz, position);

//    const float df = 0.05;
    const float df = 0.5;
//    const float df = 1.0;
    float dist2 = dist * dist * df;
    float x = dist2 / (r * r); // * df);
    float v = max(1.0 - x * x, 0.0);
    float attenuation = mix(1.0, (v * v) / (dist2 + 0.5), pos.w);
    attenuation *= spot_f;

    vec3 color = diffuse_BRDF(c_diff) * NoL;
    vec4 lcolor = lights[i].color_energy;
    return color * lcolor.rgb * lcolor.w * attenuation;
}

float sample_shadow(vec4 coord, vec2 offset)
{
    coord.xy += offset * coord.w;
    return textureProj(tex_shadowmap, coord);
}

float sample_shadow4(vec4 coord, vec2 offset)
{
    offset *= coord.w;
    vec4 offs1 = vec4(offset.xy, 0.0, 0.0);
    vec4 offs2 = vec4(offset.x, -offset.y, 0.0, 0.0);
    float sum = textureProj(tex_shadowmap, coord - offs1);
    sum += textureProj(tex_shadowmap, coord + offs2);
    sum += textureProj(tex_shadowmap, coord - offs2);
    sum += textureProj(tex_shadowmap, coord + offs1);
    return sum;
}

vec4 shadow_coord()
{
//    float NoL = 0.0;
    float NoL = dot(normal, lights[0].direction.xyz);
    float normal_factor = smoothstep(0.0, 1.0, NoL) * 0.005;
    vec3 pos = position + normal * normal_factor;
    vec4 coord = mat_light * vec4(pos, 1.0);
    coord.z -= 0.001;
    return coord;
}

float shadowmap5()
{
    vec4 coord = shadow_coord();
//    return textureProj(tex_shadowmap, coord);
    float sum = sample_shadow(coord, vec2(0.0));
//    return sum;
    sum += sample_shadow4(coord, vec2(0.5) / 4096.0);
    return sum / 5.0;
}

float shadowmap8()
{
    vec4 coord = shadow_coord();
    float sum = 0.0;
    sum += sample_shadow4(coord, vec2(0.5) / 4096.0);
    sum += sample_shadow4(coord, vec2(1.5) / 4096.0);
    return sum / 8.0;
}

float shadowmap16()
{
    vec4 coord = shadow_coord();
    float sum = 0.0;
    float x, y;

    float offs = 1.0 / 4096.0;
    for (y = -1.5; y <= 1.5; y += 1.0)
      for (x = -1.5; x <= 1.5; x += 1.0)
        sum += sample_shadow(coord, vec2(x, y) * offs);

    return sum / 16.0;
}

vec3 lighting(const vec3 c_diff, const vec3 c_spec, const vec3 N, const vec3 V, const float roughness)
{
    float shadow = (sm_enabled > 0.0) ? shadowmap8() : 1.0;
    vec3 color = vec3(0.0);

//    color = light(0, c_diff, c_spec, N, V, roughness) * shadow;
//    for (int i = 1; i < light_count; i++)
//    {
//        color += light(i, c_diff, c_spec, N, V, roughness);
//    }

    color += light(0, c_diff, c_spec, N, V, roughness) * shadow;
    color += light(1, c_diff, c_spec, N, V, roughness);
    color += light(2, c_diff, c_spec, N, V, roughness);
    color += light(3, c_diff, c_spec, N, V, roughness);
    color += light(4, c_diff, c_spec, N, V, roughness);
    color += light(5, c_diff, c_spec, N, V, roughness);
    color += light(6, c_diff, c_spec, N, V, roughness);
    color += light(7, c_diff, c_spec, N, V, roughness);

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

vec3 back_lighting(const vec3 c_diff, const vec3 N, const vec3 V, const float roughness, const float shadow)
{
    vec3 color = vec3(0.0);

    color += back_light(0, c_diff, N, V, roughness) * shadow;
    color += back_light(1, c_diff, N, V, roughness);
    color += back_light(2, c_diff, N, V, roughness);
    color += back_light(3, c_diff, N, V, roughness);

    color += IBL_diffuse(c_diff, N, V, roughness);
    // This multiplication simulates absorption in the translucent material. (or at least tries to)
    return color * clamp(vec3(0.1) + c_diff * 0.5, vec3(0.1), vec3(1.0));
//    return color;
}

// Special lighting for vegetation. Takes translucency into account.
vec3 vege_lighting(const vec3 c_diff, const vec3 c_spec, const vec3 N, const vec3 V, const float roughness)
{
    float shadow = (sm_enabled > 0.0) ? shadowmap8() : 1.0;
    vec3 color = vec3(0.0);

//    color = light(0, c_diff, c_spec, N, V, roughness) * shadow;
//    for (int i = 1; i < light_count; i++)
//    {
//        color += light(i, c_diff, c_spec, N, V, roughness);
//    }

    color += light(0, c_diff, c_spec, N, V, roughness) * shadow;
    color += light(1, c_diff, c_spec, N, V, roughness);
    color += light(2, c_diff, c_spec, N, V, roughness);
    color += light(3, c_diff, c_spec, N, V, roughness);
    color += light(4, c_diff, c_spec, N, V, roughness);
    color += light(5, c_diff, c_spec, N, V, roughness);
    color += light(6, c_diff, c_spec, N, V, roughness);
    color += light(7, c_diff, c_spec, N, V, roughness);
//    color += light(8, c_diff, c_spec, N, V, roughness);
//    color += light(9, c_diff, c_spec, N, V, roughness);
//    color += light(10, c_diff, c_spec, N, V, roughness);
//    color += light(11, c_diff, c_spec, N, V, roughness);
//    color += light(12, c_diff, c_spec, N, V, roughness);
//    color += light(13, c_diff, c_spec, N, V, roughness);
//    color += light(14, c_diff, c_spec, N, V, roughness);
//    color += light(15, c_diff, c_spec, N, V, roughness);

    color += IBL(c_diff, c_spec, N, V, roughness);
    return color + back_lighting(c_diff, -N, V, roughness, shadow);
}
