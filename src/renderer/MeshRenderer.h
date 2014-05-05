
#ifndef H_DER_MESH_RENDERER_H
#define H_DER_MESH_RENDERER_H

#include "../Types.h"

namespace der
{

    class Graphics;
    class ResourceCache;

    class Mesh;

    class MeshRenderer
    {
    public:
        MeshRenderer();

        void render(Graphics *graphics, ResourceCache &cache);

        void set_mesh(ResourceID mesh);
        ResourceID get_mesh();

    private:
        ResourceID m_mesh;
    };

} // der

#endif // H_DER_MESH_RENDERER_H

