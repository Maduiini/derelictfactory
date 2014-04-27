
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
        : m_program_cache(*this)
    { }

    ResourceCache::~ResourceCache()
    { }

    Program* ResourceCache::get_program(ResourceID v, ResourceID f)
    {
        return m_program_cache.get(v, f);
    }

} // der
