
#include "SSAOEffect.h"

#include "Graphics.h"
#include "ColorFrameBuffer.h"

#include "../resource/ResourceCache.h"

namespace der
{

    SSAOEffect::SSAOEffect()
        : PostProcessingEffect(make_resource("ssao.frag"))
    { }


    SSAOBlurX::SSAOBlurX()
        : PostProcessingEffect(make_resource("ssao_blur_x.frag"))
    { }


    SSAOBlurY::SSAOBlurY()
        : PostProcessingEffect(make_resource("ssao_blur_y.frag"))
    { }


} // der
