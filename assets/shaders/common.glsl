
// A set utility functions


// Some constants
// ================

const float PI = 3.1415926;
const float ONE_OVER_PI = 1.0 / PI;


// Gamma correction
// ================

vec4 gamma_correct(vec3 color)
{
    vec3 s1 = sqrt(color);
    color = min(color, vec3(4.0));
    vec3 s2 = sqrt(s1);
    vec3 s3 = sqrt(s2);
    vec3 s4 = sqrt(s3);
    vec3 srgb = 0.662002687 * s1 + 0.684122060 * s2 - 0.3235601 * s3 - 0.225411470 * color;
    return vec4(srgb, 1.0);

    // An expensive approximation
//    return vec4(pow(color, vec3(1.0/2.23333)), 1.0);
}

vec4 gamma_correct(vec3 color, float a)
{
    vec3 s1 = sqrt(color);
    color = min(color, vec3(4.0));
    vec3 s2 = sqrt(s1);
    vec3 s3 = sqrt(s2);
    vec3 s4 = sqrt(s3);
    vec3 srgb = 0.662002687 * s1 + 0.684122060 * s2 - 0.3235601 * s3 - 0.225411470 * color;
    return vec4(srgb, a);

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


// Tangent space
// ================

mat3 der_tangent_space(vec3 normal, vec4 tangent)
{
    vec3 n = normalize(normal);
    vec3 t = normalize(tangent.xyz);
    vec3 bt = cross(n, t) * tangent.w;
    return mat3(t, bt, n);
}


// Texture look-up
// ================

vec3 der_get_albedo(sampler2D tex_albedo, vec2 tcoord)
{
    vec3 color = texture(tex_albedo, tcoord).rgb;
    return linearize(color);
}

vec4 der_get_albedo4(sampler2D tex_albedo, vec2 tcoord)
{
    vec4 color = texture(tex_albedo, tcoord);
    return vec4(linearize(color.rgb), color.a);
}

vec3 der_get_normal(sampler2D tex_normal, vec2 tcoord, vec3 normal, vec4 tangent)
{
    vec3 n = texture(tex_normal, tcoord).xyz * vec3(2.0, 2.0, 1.0) - vec3(1.0, 1.0, 0.0);
    return der_tangent_space(normal, tangent) * n;
}

vec3 der_get_env(samplerCube tex_env, vec3 v, float lod)
{
    vec3 color = textureLod(tex_env, v, lod).rgb;
    return linearize(color);
}
