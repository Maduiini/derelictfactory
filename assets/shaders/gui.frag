#version 330

uniform sampler2D gui_texture;

in vec2 tcoord;

out vec4 out_color;

void main()
{
    vec4 tex_color = texture2D(gui_texture, tcoord);

    // TODO: add proper transparency
//    if (tex_color.r == 0.0 && tex_color.g == 0.0 && tex_color.b == 0.0)
//        out_color = vec4(0.0, 0.0, 0.0, 0.0);
//    else
//    {
        float alpha = min(tex_color.r + tex_color.g + tex_color.b, 1.0);
//        out_color = vec4(tex_color.rgb, 1.0) * alpha;
        out_color = vec4(tex_color.rgb, alpha);
//    }
//        out_color = tex_color;
}
