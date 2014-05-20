
// Physically based shading


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
//    vec3 color = linearize(textureLod(tex_env, L, lod).rgb);
    vec3 color = get_env(L, lod);

//    float NoL = 0.225; //max(dot(N, L), 0.0);
//    float NoL = 1.0;
    return color * c_diff; // * NoL;// * 0.25;
}

vec3 IBL_specular(const vec3 c_spec, const vec3 N, const vec3 V, const float roughness)
{
    vec3 L = -reflect(V, N) * vec3(1.0, -1.0, 1.0);

//    float lod = roughness * 5.0;
    float lod = roughness * 20.0;
    vec3 color = get_env(L, lod);

//    float NoL = max(dot(N, L), 0.0);
    return color * c_spec; // * NoL; // * 0.25;
//    return color * c_spec * mix(NoL, 1.0, 0.5) * 0.25;
//    return color * c_spec * 0.25 * (NoL * 0.5 + 0.5);
}

vec3 IBL(const vec3 c_diff, const vec3 c_spec, const vec3 N, const vec3 V, const float roughness)
{
    vec3 cd = IBL_diffuse(c_diff, N, V, roughness) * 1.0;
    vec3 cs = IBL_specular(c_spec, N, V, roughness) * 1.0;
    return IBL_BRDF(cd, cs, N, N, N, roughness) * ONE_OVER_PI;
//    return IBL_diffuse(c_diff, N, V, roughness) +
//        IBL_specular(c_spec, N, V, roughness);
}
