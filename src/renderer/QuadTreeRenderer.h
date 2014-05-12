
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
    class InstanceUniformBlock;
    class QuadTree;
    class QuadTreeNode;

    class QuadTreeRenderer
    {
    public:
        QuadTreeRenderer();

        void render(Graphics *graphics, ResourceCache &cache, InstanceUniformBlock *uniforms, const QuadTree *qt);
        void render(Renderer *renderer, QuadTree *qt);

    private:
        static void render_node(Graphics *graphics, ResourceCache &cache, InstanceUniformBlock *uniforms,
                                const QuadTreeNode *node, size_t level);

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

