
#ifndef H_DER_POST_PROCESSING_EFFECT_H
#define H_DER_POST_PROCESSING_EFFECT_H

#include "../resource/Resource.h"

namespace der
{

    class Graphics;
    class ResourceCache;
    class ColorFrameBuffer;

    class PostProcessingEffect
    {
    public:
        PostProcessingEffect(ResourceID fragment_shader);
        virtual ~PostProcessingEffect();

        virtual void setup_rendering(Graphics *graphics, ResourceCache &cache, ColorFrameBuffer *secondary_buffer);

        void set_enabled(const bool enabled)
        { m_enabled = enabled; }

        bool is_enabled() const
        { return m_enabled; }

    protected:
        const ResourceID m_fragment_shader;
        bool m_enabled;

        static const ResourceID m_vertex_shader = make_resource("postprocess.vert");

    };

}

#endif // H_DER_POST_PROCESSING_EFFECT_H
