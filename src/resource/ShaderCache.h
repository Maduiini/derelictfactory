
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

    class ProgramCache // : public BaseResourceCache<Program>
    {
    public:
        ProgramCache(ResourceCache &res_cache);

        Program *get(ResourceID v, ResourceID f);

        void reload_all();
        void refresh_all();

    protected:
//        virtual Program* load(const char * const filepath);

    private:
        struct ProgramResource
        {
            ResourceID vertex_shader;
            ResourceID fragment_shader;
            Program *program;
        };

        void do_load(ProgramResource &res);

    private:
        typedef std::unordered_map<ResourceID, ProgramResource> ResourceMap;

        ResourceMap m_resources;

        ResourceCache &m_resource_cache;

        ResourceID m_default_vert, m_default_frag;
    };

} // der

#endif // H_DER_SHADER_CACHE_H

