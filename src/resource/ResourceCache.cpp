
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
        , m_texture_cube_cache()
        , m_shader_cache()
        , m_program_cache(*this)
        , m_material_cache()
    {
        m_mesh_cache.scan();
        m_texture_cache.scan();
        m_texture_cube_cache.scan();
        m_shader_cache.scan();
        m_material_cache.scan();
    }

    ResourceCache::~ResourceCache()
    { }

    void ResourceCache::reload_all()
    {
        m_mesh_cache.reload_all();
        m_texture_cache.reload_all();
        m_texture_cube_cache.reload_all();
        m_shader_cache.reload_all();
        m_program_cache.reload_all();
        m_material_cache.reload_all();
    }

    void ResourceCache::refresh_all()
    {
        m_mesh_cache.refresh_all();
        m_texture_cache.refresh_all();
        m_texture_cube_cache.refresh_all();
        m_shader_cache.refresh_all();
        m_program_cache.refresh_all();
        m_material_cache.refresh_all();
    }

    Program* ResourceCache::get_program(ResourceID v, ResourceID f)
    {
        return m_program_cache.get(v, f);
    }

} // der
