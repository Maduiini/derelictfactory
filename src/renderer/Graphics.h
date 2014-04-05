
#ifndef H_DER_GRAPHICS_H
#define H_DER_GRAPHICS_H

namespace der
{

    class Texture;

    enum CullMode
    {
        CULL_BackFace,
        CULL_FrontFace,
        CULL_None
    };

    class Graphics
    {
    public:
        Graphics();

        bool init();

        void reset_state();
        void update_state();

        void set_cull_mode(CullMode mode);
        CullMode get_cull_mode() const;

        void set_texture(int unit, Texture *texture);

        static const int MAX_TEXTURE_UNITS = 16;

    private:
        struct TextureUnit
        {
            Texture *m_texture;
        };

        struct State
        {
            CullMode m_cull_mode;
            TextureUnit m_texture_units[MAX_TEXTURE_UNITS];
        } m_current, m_prev;
    };

} // der

#endif // H_DER_GRAPHICS_H

