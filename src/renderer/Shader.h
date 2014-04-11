
#ifndef H_DER_SHADER_H
#define H_DER_SHADER_H

#include "../Types.h"
#include "GLTypes.h"

namespace der
{

    class VertexBuffer;
    struct Matrix4;
    struct Matrix3x4;

    class Shader
    {
    public:
        enum Type
        {
            TYPE_Vert, TYPE_Frag
        };
    public:
        Shader(Type type);
        ~Shader();

        bool compile(const char * const source);

        GLuint get_object() const;

    private:
        GLuint m_object;
    };

    class Program
    {
    public:
        Program();
        ~Program();

        void attach(Shader *shader);
        void detach_shaders();

        bool link();

        void use();
        void pointer();

        int get_uniform_location(const char * const name);

        void uniform_sampler2D(int location, int sampler);
        void uniform(int location, const Matrix4 &mat);
        void uniform(int location, const Matrix3x4 &mat);

    private:
        GLuint m_program;
        GLuint m_vao;

        static const size_t MAX_ATTACHED_SHADERS = 2;
        Shader *m_shaders[MAX_ATTACHED_SHADERS];
    };

} // der

#endif // H_DER_SHADER_H
