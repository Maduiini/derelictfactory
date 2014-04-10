
#ifndef H_DER_GRAPHICS_H
#define H_DER_GRAPHICS_H

namespace der
{

    class Texture;

    enum class CullMode
    {
        BackFace,
        FrontFace,
        None
    };

    class Graphics
    {
    public:
        static const int MAX_TEXTURE_UNITS = 16;

    public:
        Graphics();

        bool init();

        void clear();
        void set_viewport(int x, int y, int width, int height);

        void reset_state();
        void update_state();

        void set_depth_enabled(bool enabled);
        bool is_depth_enabled() const;

        void set_cull_mode(CullMode mode);
        CullMode get_cull_mode() const;

        void set_texture(int unit, Texture *texture);

//        void bind_buffer_program(VertexBuffer *vb, Program *program);
//        void draw_triangles(IndexBuffer *ib, size_t start_index, size_t index_count);

    private:
        struct TextureUnit
        {
            Texture *m_texture;
        };

        struct State
        {
            bool m_depth_enabled;
            CullMode m_cull_mode;
            TextureUnit m_texture_units[MAX_TEXTURE_UNITS];
        } m_current, m_prev;
    };

} // der

#endif // H_DER_GRAPHICS_H

