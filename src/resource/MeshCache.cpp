
#include "MeshCache.h"
#include "../renderer/Mesh.h"
#include "ObjMeshLoader.h"

namespace der
{

    MeshCache::MeshCache()
        : BaseResourceCache("assets/models/")
    { }

    Mesh* MeshCache::load(const char * const filepath)
    {
        ObjMeshLoader loader;
        Mesh *mesh = new Mesh();
        return loader.load(filepath, *mesh) ? mesh : nullptr;
    }

} // der
