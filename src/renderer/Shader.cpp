
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexArrayObject.h"
#include "../Debug.h"

#include "../math/Matrix4.h"

#include <GL/glew.h>

namespace der
{

    Shader::Shader(Type type)
        : m_object()
    {
        static const GLenum types[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
        m_object = ::glCreateShader(types[type]);
    }

    Shader::~Shader()
    {
        ::glDeleteShader(m_object);
    }

    bool Shader::compile(const char * const source)
    {
        const char *src = source;
        ::glShaderSource(m_object, 1, &src, 0);
        ::glCompileShader(m_object);

        GLint compiled = GL_FALSE;
        ::glGetShaderiv(m_object, GL_COMPILE_STATUS, &compiled);
        return compiled == GL_TRUE;
    }

    GLuint Shader::get_object() const
    { return m_object; }



    Program::Program()
        : m_program()
    {
        m_program = ::glCreateProgram();
        for (size_t i = 0; i < MAX_ATTACHED_SHADERS; i++)
            m_shaders[i] = nullptr;
    }

    Program::~Program()
    {
        ::glDeleteProgram(m_program);
    }

    void Program::attach(Shader *shader)
    {
        for (size_t i = 0; i < MAX_ATTACHED_SHADERS; i++)
        {
            if (!m_shaders[i])
            {
                m_shaders[i] = shader;
                ::glAttachShader(m_program, shader->get_object());
                return;
            }
        }
        DER_ASSERT(0 && "Too many shaders attached");
    }

    void Program::detach_shaders()
    {
        for (size_t i = 0; i < MAX_ATTACHED_SHADERS; i++)
        {
            if (m_shaders[i])
            {
                ::glDetachShader(m_program, m_shaders[i]->get_object());
                m_shaders[i] = nullptr;
            }
        }
    }

    bool Program::link()
    {
        static const char *attrib_names[] = {
            [static_cast<int>(VertexAttrib::Position)]  = "in_position",
            [static_cast<int>(VertexAttrib::Normal)]    = "in_normal",
            [static_cast<int>(VertexAttrib::Tangent)]   = "in_tangent",
            [static_cast<int>(VertexAttrib::TexCoord)]  = "in_tex_coord",
        };
        const size_t attrib_count = sizeof(attrib_names) / sizeof(const char*);
        for (size_t i = 0; i < attrib_count; i++)
        {
            ::glBindAttribLocation(m_program, i, attrib_names[i]);
        }

        ::glLinkProgram(m_program);

        GLint linked = GL_FALSE;
        ::glGetProgramiv(m_program, GL_LINK_STATUS, &linked);
        if (linked == GL_TRUE)
        {
            bind_uniform_block("Globals", 0);
            bind_uniform_block("Instance", 1);
            bind_uniform_block("Lights[0]", 2);
            bind_uniform_block("Lights[1]", 2);
            bind_uniform_block("Lights[2]", 2);
            bind_uniform_block("Lights[3]", 2);
            bind_uniform_block("Lights[4]", 2);
            bind_uniform_block("Lights[5]", 2);
            bind_uniform_block("Lights[6]", 2);
            bind_uniform_block("Lights[7]", 2);
            bind_uniform_block("Lights[8]", 2);
            bind_uniform_block("Lights[9]", 2);
            bind_uniform_block("Lights[10]", 2);
            bind_uniform_block("Lights[11]", 2);
            bind_uniform_block("Lights[12]", 2);
            bind_uniform_block("Lights[13]", 2);
            bind_uniform_block("Lights[14]", 2);
            bind_uniform_block("Lights[15]", 2);

            return true;
        }
        return false;
    }


    void Program::use()
    {
        ::glUseProgram(m_program);
    }

    void Program::bind_uniform_block(const char * const block_name, int binding)
    {
        GLuint block_index = ::glGetUniformBlockIndex(m_program, block_name);
        if (block_index != GL_INVALID_INDEX)
            ::glUniformBlockBinding(m_program, block_index, binding);
    }

    int Program::get_uniform_location(const char * const name)
    {
        return ::glGetUniformLocation(m_program, name);
    }

    void Program::uniform(int location, const Matrix4 &mat)
    {
        ::glUniformMatrix4fv(location, 1, GL_TRUE, reinterpret_cast<const float*>(mat.m));
    }

    void Program::uniform(int location, const Matrix3x4 &mat)
    {
        ::glUniformMatrix4x3fv(location, 1, GL_TRUE, reinterpret_cast<const float*>(mat.m));
    }

    void Program::uniform_sampler2D(int location, int sampler)
    {
        ::glUniform1i(location, sampler);
    }

} // der
