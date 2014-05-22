
#ifndef H_DER_POST_PROCESSOR_H
#define H_DER_POST_PROCESSOR_H

#include <vector>

namespace der
{

    class ColorFrameBuffer;
    class PostProcessingEffect;
    class VertexArrayObject;
    class VertexBuffer;
    class Graphics;
    class ResourceCache;

    class PostProcessor
    {
    public:
        PostProcessor();
        ~PostProcessor();

        void resize(const int width, const int height);

        void add_effect(PostProcessingEffect *effect);

        void begin_scene();

        void post_process(Graphics *graphics, ResourceCache &cache);

        void set_enabled(const bool enabled)
        { m_enabled = enabled; }

        bool is_enabled() const
        { return m_enabled; }

    private:
        void swap_buffers();

        static void build_buffers();

    private:
        ColorFrameBuffer *m_primary_buffer;
        ColorFrameBuffer *m_secondary_buffer;
        std::vector<PostProcessingEffect*> m_effects;
        bool m_enabled;

        static VertexArrayObject *m_vao;
        static VertexBuffer *m_vbuffer;

    };

} // der

#endif // H_DER_POST_PROCESSOR_H
