
#include "PostProcessingEffect.h"

namespace der
{

    PostProcessingEffect::PostProcessingEffect(ResourceID fragment_shader)
        : m_fragment_shader(fragment_shader)
        , m_enabled(true)
    { }

    PostProcessingEffect::~PostProcessingEffect()
    { }

} // der
