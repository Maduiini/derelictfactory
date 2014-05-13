
#ifndef H_DER_QUAD_TREE_RENDERER_H
#define H_DER_QUAD_TREE_RENDERER_H

#include "../resource/Resource.h"

namespace der
{

    class Renderer;
    class VertexArrayObject;
    class VertexBuffer;
    class IndexBuffer;
    class QuadTree;
    class QuadTreeNode;

    class QuadTreeRenderer
    {
    public:
        QuadTreeRenderer();

        void render_immediate(Renderer *renderer, const QuadTree *qt);
        void render(Renderer *renderer, const QuadTree *qt);

    private:
        void render_node_immediate(Renderer *renderer, const QuadTreeNode *node, size_t level);
        void render_node(Renderer *renderer, const QuadTreeNode *node, size_t level);

        void build();

    private:
        ResourceID m_vert_shader;
        ResourceID m_frag_shader;
        ResourceID m_material;
        VertexArrayObject *m_vao;
        VertexBuffer *m_vbuffer;
        IndexBuffer *m_ibuffer;
    };

} // der

#endif // H_DER_QUAD_TREE_RENDERER_H

