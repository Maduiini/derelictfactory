
#include "Material.h"
#include "Graphics.h"
#include "Texture.h"

#include "../resource/ResourceCache.h"
#include "../resource/ImageTga.h"

#include "../math/Math.h"

#include <fstream>

namespace der
{

    static Texture2D *g_env_brdf = nullptr;
    static size_t g_env_brdf_users = 0;

//   static Vector2 hammersley(size_t i, size_t n)
//    {
//        Vector2 Xi(Vector2::zero);
//
//        float p = 0.5f;
//        for (size_t k = i; k; p *= 0.5f, k >>= 1)
//        {
//            if (k & 0x1) // k mod 2 == 1
//                Xi.x += p;
//        }
//        Xi.y = (i + 0.5f) / n;
//        return Xi;
//    }

    static float radical_inverse_VdC(size_t bits)
    {
        bits = (bits << 16u) | (bits >> 16u);
        bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
        bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
        bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
        bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
        return float(bits) * 2.3283064365386963e-10f; // / 0x100000000

    }

    static Vector2 hammersley(size_t i, size_t N)
    {
        return Vector2(float(i) / float(N), radical_inverse_VdC(i));
    }

    static Vector3 importance_sample_GGX(const Vector2 &Xi, float roughness, const Vector3 &N)
    {
        float a = roughness * roughness;
        float phi = 2.0f * Math::PI * Xi.x;
        float cos_theta = std::sqrt((1.0f - Xi.y) / (1.0f + (a * a - 1.0f) * Xi.y));
        float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);

        Vector3 H;
        H.x = sin_theta * std::cos(phi);
        H.y = sin_theta * std::sin(phi);
        H.z = cos_theta;

        Vector3 up = (std::abs(N.z) < 0.999f) ? Vector3::unit_z : Vector3::unit_x;
        Vector3 tangent_x = up.cross(N).normalized();
        Vector3 tangent_y = N.cross(tangent_x);
        // Tangent to world space
        return tangent_x * H.x + tangent_y * H.y + N * H.z;
    }

    static float G1_Smith(const float XoN, const float k)
    {
        return XoN / (XoN * (1.0f - k) + k);
    }

    static float G_Smith(const float NoL, const float NoV, const float roughness)
    {
        float r = roughness;
        float k = r * r / 2.0f;
        return G1_Smith(NoL, k) * G1_Smith(NoV, k);
    }

    static Vector2 integrate_brdf(float roughness, float NoV)
    {
        Vector3 V;
        V.x = std::sqrt(1.0f - NoV * NoV); // sin
        V.y = 0.0f;
        V.z = NoV; // cos

        const Vector3 N = Vector3::unit_z;
//        const Vector3 N = V;

        float A = 0.0f;
        float B = 0.0f;
        const size_t num_samples = 1024;
//        const size_t num_samples = 32;
        for (size_t i = 0; i < num_samples; i++)
        {
            const Vector2 Xi = hammersley(i, num_samples);
            const Vector3 H = importance_sample_GGX(Xi, roughness, N);
            const Vector3 L = 2.0f * V.dot(H) * H - V;

            const float NoL = max(L.z, 0.0f);
            const float NoH = max(H.z, 0.0f);
            const float VoH = max(V.dot(H), 0.0f);

            if (NoL > 0.0)
            {
                const float G = G_Smith(roughness, NoV, NoL);
                const float G_Vis = G * VoH / (NoH * NoV);
                const float cVoH = 1.0f - VoH;
//                const float Fc = std::pow(1.0f - VoH, 5);
                const float Fc = cVoH * cVoH * cVoH * cVoH * cVoH;
                A += (1.0f - Fc) * G_Vis;
                B += Fc * G_Vis;
            }
        }
        return clamp(Vector2(A, B) / num_samples, Vector2::zero, Vector2(1.0f, 1.0f));
    }

    static bool load_env_brdf(unsigned short *data, const size_t w, const size_t h)
    {
        std::ifstream in("assets/env_brdf.data", std::ios::binary);
        if (!in.is_open())
            return false;

        size_t data_w = 0, data_h = 0;
        in.read(reinterpret_cast<char*>(&data_w), sizeof(size_t));
        in.read(reinterpret_cast<char*>(&data_h), sizeof(size_t));
        if (!in || data_w != w || data_h != h)
            return false;

        in.read(reinterpret_cast<char*>(data), w * h * 2 * sizeof(unsigned short));
        return bool(in);
    }

    static void save_env_brdf_image(unsigned short *data, const size_t w, const size_t h)
    {
        unsigned char *image_data = new unsigned char[w * h * 3];
        for (size_t y = 0; y < h; y++)
        {
            for (size_t x = 0; x < w; x++)
            {
                const size_t pos = (y * w + x);
                const size_t data_pos = pos * 2;
                const size_t image_pos = pos * 3;
                image_data[image_pos + 0] = data[data_pos + 0] / 256;
                image_data[image_pos + 1] = data[data_pos + 1] / 256;
                image_data[image_pos + 2] = 0;
            }
        }

        std::ofstream out("assets/env_brdf.tga", std::ios::binary);

        TgaReader reader;
        reader.save(out, w, h, 3, 3, image_data);

        delete[] image_data;
    }

    static void generate_env_brdf(unsigned short *data, const size_t w, const size_t h)
    {
        unsigned short scale = -1;

        const float inv_w = 1.0f / (w + 1);
        const float inv_h = 1.0f / (h + 1);

        for (size_t y = 0; y < h; y++)
        {
            const size_t ypos = y * w;
//            const float roughness = (h - y - 1) * inv_h;
            const float roughness = y * inv_h;
            for (size_t x = 0; x < w; x++)
            {
                const size_t pos = (ypos + x) * 2;
                const float NoV = x * inv_w + 0.001f;

                const Vector2 sample = integrate_brdf(roughness, NoV);
                data[pos + 0] = sample.x * scale;
                data[pos + 1] = sample.y * scale;
            }
        }

        save_env_brdf_image(data, w, h);

        std::ofstream out("assets/env_brdf.data", std::ios::binary);
        if (!out.is_open())
            return;

        out.write(reinterpret_cast<const char*>(&w), sizeof(size_t));
        out.write(reinterpret_cast<const char*>(&h), sizeof(size_t));
        out.write(reinterpret_cast<const char*>(data), w * h * 2 * sizeof(unsigned short));
    }

    static void create_env_brdf()
    {
        if (g_env_brdf_users == 0)
        {
            log::debug("Creating env brdf LUT");

            const size_t w = 256;
            const size_t h = 256;
            unsigned short *data = new unsigned short[w * h * 2];

            if (!load_env_brdf(data, w, h))
                generate_env_brdf(data, w, h);

            g_env_brdf = new Texture2D();
            g_env_brdf->TexImage(w, h, Texture::RG16, Texture::IN_RG16, data);

            delete[] data;
        }
        g_env_brdf_users++;
    }

    static void delete_env_brdf()
    {
        g_env_brdf_users--;
        if (g_env_brdf_users == 0)
        {
            delete g_env_brdf;
        }
    }


    Material::Material()
        : m_vert_shader(InvalidResource)
        , m_frag_shader(InvalidResource)
        , m_program(nullptr)
        , m_tex_albedo(InvalidResource)
        , m_tex_normal(InvalidResource)
        , m_tex_roughness(InvalidResource)
        , m_tex_metallic(InvalidResource)
        , m_tex_env(InvalidResource)
        , m_cull_mode(CullMode::BackFace)
    {
        m_vert_shader = make_resource("pbs.vert");
        m_frag_shader = make_resource("pbs.frag");

        create_env_brdf();
    }

    Material::~Material()
    { delete_env_brdf(); }

    void Material::set_shaders(ResourceID vert_id, ResourceID frag_id)
    {
        m_vert_shader = vert_id;
        m_frag_shader = frag_id;
    }

    void Material::set_albedo_texture(ResourceID tex_id)
    { m_tex_albedo = tex_id; }

    ResourceID Material::get_albedo_texture() const
    { return m_tex_albedo; }

    void Material::set_normal_texture(ResourceID tex_id)
    { m_tex_normal = tex_id; }

    ResourceID Material::get_normal_texture() const
    { return m_tex_normal; }

    void Material::set_roughness_texture(ResourceID tex_id)
    { m_tex_roughness = tex_id; }

    ResourceID Material::get_roughness_texture() const
    { return m_tex_roughness; }

    void Material::set_metallic_texture(ResourceID tex_id)
    { m_tex_metallic = tex_id; }

    ResourceID Material::get_metallic_texture() const
    { return m_tex_metallic; }

    void Material::set_env_texture(ResourceID tex_id)
    { m_tex_env = tex_id; }

    ResourceID Material::get_env_texture() const
    { return m_tex_env; }

    void Material::set_cull_mode(CullMode mode)
    { m_cull_mode = mode; }

    CullMode Material::get_cull_mode() const
    { return m_cull_mode; }


    void Material::use(Graphics *graphics, ResourceCache *cache)
    {
        Texture *albedo = cache->get<Texture2D>(get_albedo_texture());
        Texture *normal = cache->get<Texture2D>(get_normal_texture());
        Texture *roughness = cache->get<Texture2D>(get_roughness_texture());
        Texture *metallic = cache->get<Texture2D>(get_metallic_texture());
        Texture *env = cache->get<TextureCube>(get_env_texture());

        graphics->set_texture(0, albedo);
//        graphics->set_texture(0, g_env_brdf);
        graphics->set_texture(1, normal);
        graphics->set_texture(2, roughness);
        graphics->set_texture(3, metallic);
        graphics->set_texture(4, g_env_brdf);
        graphics->set_texture(5, env);

        Program *program = cache->get_program(m_vert_shader, m_frag_shader);
        update_program(program);

        graphics->set_cull_mode(get_cull_mode());
    }

    void Material::update_program(Program *program)
    {
        if (program != m_program)
        {
            m_program = program;
//            if (m_program)
//            {
//                m_program->use();
//                const int tex_albedo = m_program->get_uniform_location("tex_albedo");
//                const int tex_normal = m_program->get_uniform_location("tex_normal");
//                const int tex_roughness = m_program->get_uniform_location("tex_roughness");
//                const int tex_metallic = m_program->get_uniform_location("tex_metallic");
//                const int tex_env = m_program->get_uniform_location("tex_env");
//
//                m_program->uniform_sampler2D(tex_albedo, 0);
//                m_program->uniform_sampler2D(tex_normal, 1);
//                m_program->uniform_sampler2D(tex_roughness, 2);
//                m_program->uniform_sampler2D(tex_metallic, 3);
//                m_program->uniform_sampler2D(tex_env, 4);
//            }
        }
//        else
        if (m_program)
        {
            m_program->use();

            const int tex_albedo = m_program->get_uniform_location("tex_albedo");
            const int tex_normal = m_program->get_uniform_location("tex_normal");
            const int tex_roughness = m_program->get_uniform_location("tex_roughness");
            const int tex_metallic = m_program->get_uniform_location("tex_metallic");
            const int tex_env = m_program->get_uniform_location("tex_env");
            const int tex_env_brdf = m_program->get_uniform_location("tex_env_brdf");

            m_program->uniform_sampler2D(tex_albedo, 0);
            m_program->uniform_sampler2D(tex_normal, 1);
            m_program->uniform_sampler2D(tex_roughness, 2);
            m_program->uniform_sampler2D(tex_metallic, 3);
            m_program->uniform_sampler2D(tex_env_brdf, 4);
            m_program->uniform_sampler2D(tex_env, 5);
        }
    }

} // der
