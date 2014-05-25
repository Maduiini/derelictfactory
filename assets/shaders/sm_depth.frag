#version 330

uniform sampler2D tex_albedo;

in vec2 tcoord;

out float depth;

void main()
{
    vec4 albedo = texture(tex_albedo, tcoord);
    if (albedo.a < 0.2) discard;

    depth = gl_FragCoord.z;
}
