
#ifndef H_DER_MESH_RENDERER_H
#define H_DER_MESH_RENDERER_H

namespace der
{

    class Mesh;

    class MeshRenderer
    {
    public:
        MeshRenderer();

        void render();

        void set_mesh(Mesh *mesh);
        Mesh* get_mesh();

    private:
        Mesh *m_mesh;
    };

} // der

#endif // H_DER_MESH_RENDERER_H

