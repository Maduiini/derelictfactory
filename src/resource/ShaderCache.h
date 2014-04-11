
#ifndef H_DER_SHADER_CACHE_H
#define H_DER_SHADER_CACHE_H

#include "BaseResourceCache.h"

namespace der
{

    class Shader;
    class Program;


    class ShaderCache : public BaseResourceCache<Shader>
    {
    public:
        ShaderCache();

    protected:
        virtual Shader* load(const char * const filepath);
    };

    class ResourceCache;

    class ProgramCache : public BaseResourceCache<Program>
    {
    public:
        ProgramCache(ResourceCache &res_cache);

        Program *get(ResourceID v, ResourceID f);

    protected:
        virtual Program* load(const char * const filepath);

        ResourceCache &m_resource_cache;
    };

} // der

#endif // H_DER_SHADER_CACHE_H

