
#ifndef H_DER_GRAPHICS_TYPES_H
#define H_DER_GRAPHICS_TYPES_H

namespace der
{

//    template <class T>
//    struct GraphicsResource
//    {
//        size_t m_id;
//    };
//
//    typedef GraphicsResource<Texture> TextureID;
//    typedef GraphicsResource<Program> ProgramID;

    class Texture;

    enum class CullMode
    {
        BackFace = 0,
        FrontFace = 1,
        None
    };

    enum class PrimitiveType
    {
        Triangles,
        TriangleStrip,
        TriangleFan,
        Lines,
        LineStrip,
        LineLoop
    };

} // der

#endif // H_DER_GRAPHICS_TYPES_H

