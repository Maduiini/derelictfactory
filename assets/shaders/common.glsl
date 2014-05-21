
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
    // linearize gamma
    return linearize(color);
}

vec4 der_get_albedo4(sampler2D tex_albedo, vec2 tcoord)
{
    vec4 color = texture(tex_albedo, tcoord);
    // linearize gamma
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

//    vec3 up = (abs(v.y) < 0.999) ? vec3(0, 1, 0) : vec3(1, 0, 0);
//    vec3 right = normalize(cross(up, v));
//    vec3 fwd = cross(v, right);
//
//    vec3 color_up = textureLod(tex_env, up, lod).rgb;
//    vec3 color_rt = textureLod(tex_env, right, lod).rgb;
//    vec3 color_fw = textureLod(tex_env, fwd, lod).rgb;
//    vec3 color_dw = textureLod(tex_env, -up, lod).rgb;
//    vec3 color_lt = textureLod(tex_env, -right, lod).rgb;
//    vec3 color_bk = textureLod(tex_env, -fwd, lod).rgb;
//
//    v = v * 0.5 + vec3(1.0);
//    vec3 c_x = mix(color_lt, color_rt, v.x);
//    vec3 c_y = mix(color_dw, color_up, v.y);
//    vec3 c_z = mix(color_bk, color_fw, v.z);
//
//    vec3 color = linearize(c_x) + linearize(c_y) + linearize(c_z);
//    color *= 3;
//    vec3 color = (c_x) + (c_y) + (c_z);

//    vec3 color = max(color_up * dot(v, up), 0.0) + max(color_rt * dot(v, right), 0.0)
//        + max(color_fw * dot(v, fwd), 0.0) + max(color_dw * dot(v, -up), 0.0)
//        + max(color_lt * dot(v, -right), 0.0) + max(color_bk * dot(v, -fwd), 0.0);
//    vec3 color = max(color_up, 0.0) + max(color_rt, 0.0)
//        + max(color_fw, 0.0) + max(color_dw, 0.0)
//        + max(color_lt, 0.0) + max(color_bk, 0.0);

//    vec3 offs0 = vec3(-1.0, 0.0, -1.0) * 0.005;
//    vec3 offs1 = vec3(1.0, 0.0, 1.0) * 0.005;
//    vec3 offs2 = vec3(-1.0, 0.0, 1.0) * 0.005;
//    vec3 offs3 = vec3(1.0, 0.0, -1.0) * 0.005;
//
//    vec3 color0 = textureLod(tex_env, normalize(v + offs0), lod).rgb;
//    vec3 color1 = textureLod(tex_env, normalize(v + offs1), lod).rgb;
//    vec3 color2 = textureLod(tex_env, normalize(v + offs2), lod).rgb;
//    vec3 color3 = textureLod(tex_env, normalize(v + offs3), lod).rgb;
//
//    color = mix(color + color0 + color1 + color2 + color3, color * 5, lod);
//    color *= 0.2;

    // linearize gamma
    return linearize(color);
//    return color;
}

