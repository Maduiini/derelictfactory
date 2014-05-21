
#ifndef H_DER_FRAME_BUFFER_H
#define H_DER_FRAME_BUFFER_H

#include "GLTypes.h"

namespace der
{

    class FrameBuffer
    {
    public:
        FrameBuffer();
        ~FrameBuffer();

        bool is_complete();

        void bind();

        static void bind_default_buffer();

    private:
        GLuint m_framebuffer;

    };


} // der

#endif // H_DER_FRAME_BUFFER_H
