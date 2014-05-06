
#include "ShaderCache.h"
#include "../renderer/Shader.h"

#include "ResourceCache.h"

#include <fstream>
#include <iostream>

namespace der
{

    ShaderCache::ShaderCache()
        : BaseResourceCache("assets/shaders/")
    {
        m_supported_extensions.push_back("vert");
        m_supported_extensions.push_back("frag");
    }

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
        : m_resource_cache(res_cache)
        , m_default_vert(InvalidResource)
        , m_default_frag(InvalidResource)
    {
        m_default_vert = make_resource("default.vert");
        m_default_frag = make_resource("default.frag");
    }

    void ProgramCache::do_load(ProgramResource &res)
    {
        Shader *vs = m_resource_cache.get<Shader>(res.vertex_shader);
        Shader *fs = m_resource_cache.get<Shader>(res.fragment_shader);
        if (vs && fs)
        {
            Program::use_none();

            delete res.program;
            res.program = new Program();

            res.program->attach(vs);
            res.program->attach(fs);
            if (!res.program->link())
            {
                res.program->detach_shaders();
                vs = m_resource_cache.get<Shader>(m_default_vert);
                fs = m_resource_cache.get<Shader>(m_default_frag);
                res.program->attach(vs);
                res.program->attach(fs);
                if (!res.program->link())
                {
                    delete res.program;
                    res.program = nullptr;
                }
            }
        }
    }

    Program* ProgramCache::get(ResourceID v, ResourceID f)
    {
        ResourceID id = v + f;
        auto it = m_resources.find(id);
        if (it == m_resources.end())
        {
            ProgramResource res;
            res.vertex_shader = v;
            res.fragment_shader = f;
            res.program = nullptr;

            do_load(res);
            m_resources[id] = res;
            return res.program;
        }

        ProgramResource &res = it->second;
        return res.program;
    }

    void ProgramCache::reload_all()
    {
        auto it = m_resources.begin();
        for (; it != m_resources.end(); ++it)
        {
            ProgramResource &res = it->second;
            do_load(res);
        }
    }

    void ProgramCache::refresh_all()
    {
        reload_all();
    }

} // der
