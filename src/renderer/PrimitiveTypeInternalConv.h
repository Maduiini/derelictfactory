
namespace der
{

    static const GLenum g_gl_modes[] {
        [static_cast<int>(PrimitiveType::Triangles)] = GL_TRIANGLES,
        [static_cast<int>(PrimitiveType::TriangleStrip)] = GL_TRIANGLE_STRIP,
        [static_cast<int>(PrimitiveType::TriangleFan)] = GL_TRIANGLE_FAN,
        [static_cast<int>(PrimitiveType::Lines)] = GL_LINES,
        [static_cast<int>(PrimitiveType::LineStrip)] = GL_LINE_STRIP,
        [static_cast<int>(PrimitiveType::LineLoop)] = GL_LINE_LOOP
    };

} // der
