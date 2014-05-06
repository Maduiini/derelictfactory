
#include "MeshCache.h"
#include "../renderer/Mesh.h"
#include "ObjMeshLoader.h"
#include "../Log.h"

namespace der
{

    MeshCache::MeshCache()
        : BaseResourceCache("assets/models/")
    {
        m_supported_extensions.push_back("obj");
    }

    Mesh* MeshCache::load(const char * const filepath)
    {
        ObjMeshLoader loader;
        Mesh *mesh = new Mesh();
        if (!loader.load(filepath, *mesh))
            log::error("Could not load mesh: %", filepath);
        return mesh;
    }

} // der
