
#include "ResourceCache.h"
#include "DirectoryFiles.h"
#include "../Log.h"

#include <dirent.h>
#include <string.h>
#include <vector>
#include <sstream>

namespace der
{

    ResourceCache::ResourceCache()
        : m_mesh_cache()
        , m_texture_cache()
        , m_shader_cache()
        , m_program_cache(*this)
        , m_material_cache()
    { }

    ResourceCache::~ResourceCache()
    { }

    void ResourceCache::reload_all()
    {
        m_mesh_cache.reload_all();
        m_texture_cache.reload_all();
        m_shader_cache.reload_all();
        m_program_cache.reload_all();
        m_material_cache.reload_all();
    }

    Program* ResourceCache::get_program(ResourceID v, ResourceID f)
    {
        return m_program_cache.get(v, f);
    }

} // der
