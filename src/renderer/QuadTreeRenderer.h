
#ifndef H_DER_QUAD_TREE_RENDERER_H
#define H_DER_QUAD_TREE_RENDERER_H

#include "../resource/Resource.h"

namespace der
{

    class Renderer;
    class Graphics;
    class ResourceCache;
    class VertexArrayObject;
    class VertexBuffer;
    class IndexBuffer;

    class QuadTreeRenderer
    {
    public:
        QuadTreeRenderer();

        void render(Graphics *graphics, ResourceCache &cache);
        void render(Renderer *renderer);

    private:
        static void build();

    private:
        static ResourceID m_vert_shader;
        static ResourceID m_frag_shader;
        static VertexArrayObject *m_vao;
        static VertexBuffer *m_vbuffer;
        static IndexBuffer *m_ibuffer;
    };

} // der

#endif // H_DER_QUAD_TREE_RENDERER_H

