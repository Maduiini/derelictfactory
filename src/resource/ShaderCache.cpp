
#include "ShaderCache.h"
#include "../renderer/Shader.h"

#include "ResourceCache.h"

#include <fstream>
#include <iostream>

namespace der
{

    ShaderCache::ShaderCache()
        : BaseResourceCache("assets/shaders/")
    { }

    Shader* ShaderCache::load(const char * const filepath)
    {
        const std::string fpath(filepath);
        size_t pos = fpath.find_last_of('.');
        if (pos != std::string::npos)
        {
            Shader::Type type;
            const std::string ext = fpath.substr(pos);
            if (ext == ".vert")
                type = Shader::TYPE_Vert;
            else if (ext == ".frag")
                type = Shader::TYPE_Frag;
            else
                return nullptr;

            std::ifstream in(filepath, std::ios::binary);
            if (!in.is_open()) return nullptr;

            in.seekg(0, std::ios::end);
            const size_t file_size = in.tellg();
            in.seekg(0, std::ios::beg);

            char *contents = new char[file_size + 1];
            for (size_t i = 0; i < file_size; i++)
                contents[i] = 0;
            in.read(contents, file_size);
            contents[file_size] = 0; // make null-terminated

            in.close();

            Shader *shader = new Shader(type);
            shader->compile(contents);

            delete[] contents;
            return shader;
        }
        return nullptr;
    }



    ProgramCache::ProgramCache(ResourceCache &res_cache)
        : BaseResourceCache("")
        , m_resource_cache(res_cache)
        , m_default_vert(InvalidResource)
        , m_default_frag(InvalidResource)
    {
        m_default_vert = make_resource("default.vert");
        m_default_frag = make_resource("default.frag");
    }

    Program* ProgramCache::get(ResourceID v, ResourceID f)
    {
        ResourceID id = v + f;
        auto it = m_resources.find(id);
        if (it == m_resources.end())
        {
            Shader *vs = m_resource_cache.get<Shader>(v);
            Shader *fs = m_resource_cache.get<Shader>(f);

            if (vs && fs)
            {
                Program *program = new Program();
                program->attach(vs);
                program->attach(fs);
                if (program->link())
                {
                    Resource res;
                    res.filepath = "";
                    res.resource = program;
                    m_resources[id] = res;
                    return program;
                }
                else
                {
                    delete program;
                    program = get(m_default_vert, m_default_frag);

                    Resource res;
                    res.filepath = "";
                    res.resource = program;
                    m_resources[id] = res;
                    return program;
                }
            }
            return nullptr;
        }

        Resource &res = it->second;
        return res.resource;
    }

    Program* ProgramCache::load(const char * const filepath)
    { return nullptr; }

} // der
