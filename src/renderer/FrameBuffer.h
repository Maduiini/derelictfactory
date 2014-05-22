
#ifndef H_DER_FRAME_BUFFER_H
#define H_DER_FRAME_BUFFER_H

#include "GLTypes.h"

namespace der
{

    class FrameBuffer
    {
    public:
        FrameBuffer();
        virtual ~FrameBuffer();

        bool is_complete();

        void bind();

        virtual void set_draw_buffers() = 0;

        static void bind_default_buffer();

    protected:
        GLuint m_framebuffer;

    };


} // der

#endif // H_DER_FRAME_BUFFER_H
