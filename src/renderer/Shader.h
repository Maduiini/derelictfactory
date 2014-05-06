
#ifndef H_DER_SHADER_H
#define H_DER_SHADER_H

#include "../Types.h"
#include "GLTypes.h"

namespace der
{

    class VertexBuffer;
    struct Matrix4;
    struct Matrix3x4;
    struct Vector2;

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
        bool has_shader_attached(Shader *shader) const;

        bool link();

        void use();
        static void use_none();

        void bind_uniform_block(const char * const block_name, int binding);
        int get_uniform_location(const char * const name);

        void uniform_sampler2D(int location, int sampler);
        void uniform(int location, const Matrix4 &mat);
        void uniform(int location, const Matrix3x4 &mat);
        void uniform(int location, const Vector2 &vec);

    private:
        GLuint m_program;

        static const size_t MAX_ATTACHED_SHADERS = 2;
        Shader *m_shaders[MAX_ATTACHED_SHADERS];
    };

} // der

#endif // H_DER_SHADER_H

