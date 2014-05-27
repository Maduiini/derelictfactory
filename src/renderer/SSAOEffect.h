
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

        void set_strength(float str);

    private:
        float m_strength;

    };

    class SSAOBlurX : public PostProcessingEffect
    {
    public:
        SSAOBlurX();

        //virtual void setup_rendering(Graphics *graphics, ResourceCache &cache, ColorFrameBuffer *secondary_buffer) override;
    };

    class SSAOBlurY : public PostProcessingEffect
    {
    public:
        SSAOBlurY();

        //virtual void setup_rendering(Graphics *graphics, ResourceCache &cache, ColorFrameBuffer *secondary_buffer) override;
    };

} // der

#endif // H_DER_SSAO_EFFECT_H
