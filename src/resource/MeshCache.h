
#ifndef H_DER_MESH_CACHE_H
#define H_DER_MESH_CACHE_H

#include "BaseResourceCache.h"

#include "Resource.h"
#include <string>
#include <unordered_map>

namespace der
{

    class Mesh;

    class MeshCache : public BaseResourceCache<Mesh>
    {
    public:
        MeshCache();

    protected:
        virtual Mesh* load(const char * const filepath, InputFileList &dependencies);

    };

} // der

#endif // H_DER_MESH_CACHE_H

