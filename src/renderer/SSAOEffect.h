
#ifndef H_DER_SSAO_EFFECT_H
#define H_DER_SSAO_EFFECT_H

#include "PostProcessingEffect.h"

namespace der
{

    class SSAOEffect : public PostProcessingEffect
    {
    public:
        SSAOEffect();

        virtual void setup_rendering(Graphics *graphics, ResourceCache &cache, ColorFrameBuffer *secondary_buffer) override;

    private:
        // add ssao parameters here

    };

} // der

#endif // H_DER_SSAO_EFFECT_H
