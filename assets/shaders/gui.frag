#version 330

uniform sampler2D gui_texture;

in vec2 tcoord;

out vec4 out_color;

void main()
{
    vec4 tex_color = texture2D(gui_texture, tcoord);

    // TODO: add proper transparency
    float alpha = float(tex_color.r + tex_color.g + tex_color.b > 0.0);
    out_color = vec4(tex_color.rgb, alpha);
//    out_color = tex_color;
}
