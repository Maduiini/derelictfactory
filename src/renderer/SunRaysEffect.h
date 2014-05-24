
#ifndef H_DER_SUN_RAYS_EFFECT_H
#define H_DER_SUN_RAYS_EFFECT_H

namespace der
{

    class SunRaysEffect : public PostProcessingEffect
    {
    public:
        SunRaysEffect()
            : PostProcessingEffect(make_resource("sunrays.frag"))
        { }

        //virtual void setup_rendering(Graphics *graphics, ResourceCache &cache, ColorFrameBuffer *secondary_buffer) override;
    };

} // der

#endif // H_DER_SUN_RAYS_EFFECT_H
