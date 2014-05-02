
#ifndef H_DER_TRANSFORM_RENDERER_H
#define H_DER_TRANSFORM_RENDERER_H

#include "../resource/Resource.h"

namespace der
{

    class Graphics;
    class ResourceCache;
    class VertexArrayObject;
    class VertexBuffer;

    class TransformRenderer
    {
    public:
        TransformRenderer();

        void render(Graphics *graphics, ResourceCache &cache);

    private:
        static void build();
    private:
        static ResourceID m_vert_shader;
        static ResourceID m_frag_shader;
        static VertexArrayObject *m_vao;
        static VertexBuffer *m_vbuffer;
    };

} // der

#endif // H_DER_TRANSFORM_RENDERER_H

