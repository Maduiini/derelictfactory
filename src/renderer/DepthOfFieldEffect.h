
#ifndef H_DER_DEPTH_OF_FIELD_H
#define H_DER_DEPTH_OF_FIELD_H

#include "PostProcessingEffect.h"

namespace der
{

    class DepthOfFieldEffect : public PostProcessingEffect
    {
    public:
        DepthOfFieldEffect()
            : PostProcessingEffect(make_resource("depth_of_field.frag"))
        { }

        //virtual void setup_rendering(Graphics *graphics, ResourceCache &cache, ColorFrameBuffer *secondary_buffer) override;
    };


} // der

#endif // H_DER_DEPTH_OF_FIELD_H
