#version 330

uniform sampler2D tex_albedo;

in vec2 tcoord;
//in vec3 normal;
//in vec3 view_vec;

out float depth;

void main()
{
    vec4 albedo = texture(tex_albedo, tcoord);
    if (albedo.a < 0.2) discard;

//    vec3 V = normalize(view_vec);
//    float bias = abs(dot(normal, view_vec));
//    float bias = abs(dot(normal, view_vec) * 0.01);
//    float bias = 0.0;
////    gl_FragDepth = gl_FragCoord.z + bias;

    depth = gl_FragCoord.z;
//    gl_FragDepth = gl_FragCoord.z;
//    gl_FragColor = vec4(1.0);
}
