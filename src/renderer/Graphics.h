
#ifndef H_DER_GRAPHICS_H
#define H_DER_GRAPHICS_H

#include "../Types.h"
#include "GraphicsTypes.h"

namespace der
{

    class IndexBuffer;

    class Graphics
    {
    public:
        static const int MAX_TEXTURE_UNITS = 16;

    public:
        Graphics();

        bool init();

        void clear();
        void clear_color();
        void clear_depth();
        void set_viewport(int x, int y, int width, int height);

        void reset_state();
        void update_state();

        void set_depth_enabled(bool enabled);
        bool is_depth_enabled() const;

        void set_color_enabled(bool enabled);
        bool is_color_enabled() const;

        void set_blend_enabled(bool enabled);
        bool is_blend_enabled() const;

        void set_cull_mode(CullMode mode);
        CullMode get_cull_mode() const;

        void set_texture(int unit, Texture *texture);

//        void use_program(Program *program);

        void draw_primitives(PrimitiveType prim_type, IndexBuffer *ib, size_t start_index, size_t index_count);
        void draw_primitives(PrimitiveType prim_type, size_t start_index, size_t index_count);

        void draw_triangles(IndexBuffer *ib, size_t start_index, size_t index_count);
        void draw_triangle_strip(IndexBuffer *ib, size_t start_index, size_t index_count);
        void draw_triangle_fan(IndexBuffer *ib, size_t start_index, size_t index_count);
        void draw_lines(IndexBuffer *ib, size_t start_index, size_t index_count);
        void draw_line_strip(IndexBuffer *ib, size_t start_index, size_t index_count);
        void draw_line_loop(IndexBuffer *ib, size_t start_index, size_t index_count);

        void draw_triangles(size_t start_index, size_t index_count);
        void draw_triangle_strip(size_t start_index, size_t index_count);
        void draw_triangle_fan(size_t start_index, size_t index_count);
        void draw_lines(size_t start_index, size_t index_count);
        void draw_line_strip(size_t start_index, size_t index_count);
        void draw_line_loop(size_t start_index, size_t index_count);

        void reset_state_changes();
        size_t get_state_changes() const;

    private:
        struct TextureUnit
        {
            Texture *m_texture;
        };

        struct State
        {
            bool m_depth_enabled;
            bool m_color_enabled;
            bool m_blend_enabled;
            CullMode m_cull_mode;
            TextureUnit m_texture_units[MAX_TEXTURE_UNITS];
        } m_current, m_prev;

        size_t m_state_changes;
    };

} // der

#endif // H_DER_GRAPHICS_H

