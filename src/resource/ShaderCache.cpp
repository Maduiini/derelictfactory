
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
//        : BaseResourceCache("")
        : m_resource_cache(res_cache)
        , m_default_vert(InvalidResource)
        , m_default_frag(InvalidResource)
    {
        m_default_vert = make_resource("default.vert");
        m_default_frag = make_resource("default.frag");
    }

    void ProgramCache::do_load(ProgramResource &res)
    {
        if (res.fragment_shader != m_default_frag ||
            res.vertex_shader != m_default_vert)
        {
            delete res.program;
        }

        Shader *vs = m_resource_cache.get<Shader>(res.vertex_shader);
        Shader *fs = m_resource_cache.get<Shader>(res.fragment_shader);
        if (vs && fs)
        {
            res.program = new Program();
            res.program->attach(vs);
            res.program->attach(fs);
            if (!res.program->link())
            {
                delete res.program;
                res.program = get(m_default_vert, m_default_frag);
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
            res.program = 0;

            do_load(res);
            m_resources[id] = res;
            return res.program;

//            Shader *vs = m_resource_cache.get<Shader>(v);
//            Shader *fs = m_resource_cache.get<Shader>(f);
//            if (vs && fs)
//            {
//                res.program = new Program();
//                res.program->attach(vs);
//                res.program->attach(fs);
//                if (res.program->link())
//                {
//                    m_resources[id] = res;
//                    return program;
//                }
//                delete res.program;
//            }
//            res.program = get(m_default_vert, m_default_frag);
//            m_resources[id] = res;
//            return res.program;
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
//            delete res.program;
//
//            Shader *vs = m_resource_cache.get<Shader>(res.vertex_shader);
//            Shader *fs = m_resource_cache.get<Shader>(res.fragment_shader);
//
//            if (vs && fs)
//            {
//                Program *program = new Program();
//                program->attach(vs);
//                program->attach(fs);
//                if (program->link())
//                {
//                    res.resource = program;
//                    m_resources[id] = res;
//                    return program;
//                }
//                else
//                {
//                    delete program;
//                    program = get(m_default_vert, m_default_frag);
//
//                    Resource res;
//                    res.resource = program;
//                    m_resources[id] = res;
//                    return program;
//                }
//            }
//            return nullptr;
        }
    }
//
//    Program* ProgramCache::load(const char * const filepath)
//    { return nullptr; }

} // der
