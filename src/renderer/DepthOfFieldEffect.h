
#ifndef H_DER_DEPTH_OF_FIELD_H
#define H_DER_DEPTH_OF_FIELD_H

#include "PostProcessingEffect.h"

namespace der
{

    class DepthOfFieldEffect : public PostProcessingEffect
    {
    public:
        DepthOfFieldEffect();

        virtual void setup_rendering(Graphics *graphics, ResourceCache &cache, ColorFrameBuffer *secondary_buffer) override;

        void enable_debugging(bool enable);

    private:
        bool m_debug;

    };


} // der

#endif // H_DER_DEPTH_OF_FIELD_H
