
#include "Graphics.h"
#include "Texture.h"
#include "IndexBuffer.h"

#include "../Log.h"

#include <GL/glew.h>

void __stdcall gl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                                  GLsizei length, const GLchar *message, GLvoid *userParam)
{
    const char *src = "undefined";
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             src = "API"; break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   src = "Window system"; break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: src = "Shader compiler"; break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     src = "Third party"; break;
    case GL_DEBUG_SOURCE_APPLICATION:     src = "Application"; break;
    case GL_DEBUG_SOURCE_OTHER:           src = "Other"; break;
    }

    const char *tp = "";
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR_ARB:               tp = "error"; break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB: tp = "deprecated"; break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:  tp = "undefined"; break;
    case GL_DEBUG_TYPE_PORTABILITY_ARB:         tp = "portability"; break;
    case GL_DEBUG_TYPE_PERFORMANCE_ARB:         tp = "performance"; break;
    case GL_DEBUG_TYPE_OTHER_ARB:               tp = "other"; break;
    }

    const char *sev = "undefined";
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH_ARB:   sev = "high";   break;
    case GL_DEBUG_SEVERITY_MEDIUM_ARB: sev = "medium"; break;
    case GL_DEBUG_SEVERITY_LOW_ARB:    sev = "low"; break;
    }

    der::log::debug("% %(%): %, %", src, tp, sev, message, id);
}

namespace der
{

    Graphics::Graphics()
    {
        reset_state();
        for (TextureUnit &tu : m_current.m_texture_units)
            tu.m_texture = nullptr;
        m_prev = m_current;
        m_prev.m_cull_mode = CullMode::None;
    }

    bool Graphics::init()
    {
        ::glewExperimental = GL_TRUE; // OpenGL 3.3 requires experimental API

        if (::glewInit() != GLEW_OK)
        {
            log::fatal("GLEW not initialized!");
            return false;
        }

        #ifdef DER_DEBUG
        if (::glewIsSupported("GL_ARB_debug_output"))
        {
            ::glDebugMessageCallback(&gl_debug_callback, nullptr);
            ::glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        }
        #endif // DER_DEBUG

        ::glEnable(GL_DEPTH_TEST);

        return true;
    }

    void Graphics::set_viewport(int x, int y, int width, int height)
    {
        ::glViewport(x, y, width, height);
    }

    void Graphics::clear()
    {
        ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Graphics::reset_state()
    {
        m_current.m_depth_enabled = true;
        m_current.m_cull_mode = CullMode::BackFace;
    }

    void Graphics::update_state()
    {
        if (m_current.m_depth_enabled != m_prev.m_depth_enabled)
        {
            if (m_current.m_depth_enabled)
                ::glEnable(GL_DEPTH_TEST);
            else
                ::glDisable(GL_DEPTH_TEST);
            m_prev.m_depth_enabled = m_current.m_depth_enabled;
        }
        if (m_current.m_cull_mode != m_prev.m_cull_mode)
        {
            static const GLenum cull_modes[] = { GL_BACK, GL_FRONT };

            if (m_prev.m_cull_mode == CullMode::None)
                ::glEnable(GL_CULL_FACE);

            if (m_current.m_cull_mode == CullMode::None)
                ::glDisable(GL_CULL_FACE);
            else
                ::glCullFace(cull_modes[static_cast<int>(m_current.m_cull_mode)]);
            m_prev.m_cull_mode = m_current.m_cull_mode;
        }

        for (int i = 0; i < MAX_TEXTURE_UNITS; i++)
        {
            TextureUnit &tu = m_current.m_texture_units[i];
            TextureUnit &prev_tu = m_prev.m_texture_units[i];
            if (tu.m_texture && tu.m_texture != prev_tu.m_texture)
            {
                ::glActiveTexture(GL_TEXTURE0 + i);
                ::glBindTexture(tu.m_texture->get_target(), tu.m_texture->get_id());
                prev_tu = tu;
            }
        }
    }

    void Graphics::set_cull_mode(CullMode mode)
    { m_current.m_cull_mode = mode; }

    CullMode Graphics::get_cull_mode() const
    { return m_current.m_cull_mode; }

    void Graphics::set_texture(int unit, Texture *texture)
    {
        m_current.m_texture_units[unit].m_texture = texture;
    }


    void Graphics::draw_triangles(IndexBuffer *ib, size_t start_index, size_t index_count)
    { ib->draw_triangles(start_index, index_count); }

    void Graphics::draw_lines(IndexBuffer *ib, size_t start_index, size_t index_count)
    { ib->draw_lines(start_index, index_count); }

    void Graphics::draw_triangles(size_t start_index, size_t index_count)
    { ::glDrawArrays(GL_TRIANGLES, start_index, index_count); }

    void Graphics::draw_lines(size_t start_index, size_t index_count)
    { ::glDrawArrays(GL_LINES, start_index, index_count); }

} // der
