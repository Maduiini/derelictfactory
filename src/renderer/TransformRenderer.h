
#ifndef H_DER_TRANSFORM_RENDERER_H
#define H_DER_TRANSFORM_RENDERER_H

#include "../resource/Resource.h"

namespace der
{

    class Renderer;
    class VertexArrayObject;
    class VertexBuffer;

    class TransformRenderer
    {
    public:
        TransformRenderer();

        void render_immediate(Renderer *renderer);
        void render(Renderer *renderer);

    private:
        static void build();
    private:
        static ResourceID m_material;
        static VertexArrayObject *m_vao;
        static VertexBuffer *m_vbuffer;
    };

} // der

#endif // H_DER_TRANSFORM_RENDERER_H

