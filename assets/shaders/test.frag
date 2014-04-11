
#version 330

uniform sampler2D tex_color;
in vec2 tex_coord;

void main()
{
    vec4 color = texture2D(tex_color, tex_coord);
    gl_FragData[0] = color;
//    gl_FragData[0] = vec4(1.0, 0.0, 0.0, 1.0);
}
