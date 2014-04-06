
#ifndef H_DER_OBJ_MESH_LOADER_H
#define H_DER_OBJ_MESH_LOADER_H

namespace der
{

    class Mesh;

    class ObjMeshLoader
    {
    public:
        bool load(const char * const filepath, Mesh &mesh);

    };

} // der

#endif // H_DER_OBJ_MESH_LOADER_H

