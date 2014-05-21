
#ifndef H_DER_DEPTH_ONLY_FRAME_BUFFER_H
#define H_DER_DEPTH_ONLY_FRAME_BUFFER_H

#include "FrameBuffer.h"
#include "Texture.h"

namespace der
{

    class DepthOnlyFrameBuffer : public FrameBuffer
    {
    public:
        DepthOnlyFrameBuffer(int size);

        DepthTexture* get_depth_texture()
        { return &m_depth_texture; }

    private:
        DepthTexture m_depth_texture;
        int m_size;

    };

} // der

#endif // H_DER_DEPTH_ONLY_FRAME_BUFFER_H
