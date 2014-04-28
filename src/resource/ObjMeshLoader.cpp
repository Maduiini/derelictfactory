
#include "ObjMeshLoader.h"
#include "MeshBuilder.h"
#include "../Log.h"

#include <cctype>
#include <cstdlib> // for atof
#include <fstream>
#include <string>
#include <vector>

namespace der
{

    bool skip_whitespace(char * &data, const char * const end)
    {
        if (data != end && std::isspace(*data))
        {
            do {
                data++;
            } while (data != end && std::isspace(*data));
            return true;
        }
        return false;
    }

    bool skip_hor_whitespace(char * &data, const char * const end)
    {
        if (data != end && std::isspace(*data) && *data != '\n')
        {
            do {
                data++;
            } while (data != end && std::isspace(*data));
            return true;
        }
        return false;
    }

    bool skip_until_newline(char * &data, const char * const end)
    {
        if (data != end && *data != '\n')
        {
            while (data != end && *data != '\n')
                data++;
            return true;
        }
        return false;
    }

    bool skip_comment(char * &data, const char * const end)
    {
        if (data != end && *data == '#')
        {
            skip_until_newline(data, end);
            if (data != end && *data == '\n')
                data++;
            return true;
        }
        return false;
    }

    bool skip_empty(char * &data, const char * const end)
    {
        return skip_whitespace(data, end) || skip_comment(data, end);
    }

    bool accept(char * &data, const char * const end, const char *str)
    {
        char *p = data;
        while (p != end && *p == *str && *str != 0)
        {
            p++;
            str++;
        }
        if (*str == 0) data = p;
        return *str == 0;
    }

    bool skip_mtllib(char * &data, const char * const end)
    {
        if (accept(data, end, "mtllib "))
        {
            skip_until_newline(data, end);
            return true;
        }
        return false;
    }

    bool read_object(char * &data, const char * const end, const char * &submesh_name)
    {
        if (data != end && accept(data, end, "o "))
        {
            skip_hor_whitespace(data, end);

            const char *start = data;
            skip_until_newline(data, end);

            if (data != end && *data == '\n')
            {
                *data = 0;
                submesh_name = start;
                data++;
            }
            else
            {
                // Error ?
            }
            return true;
        }
        return false;
    }

    bool read_material(char * &data, const char * const end, const char * &material_name)
    {
        if (data != end && accept(data, end, "usemtl "))
        {
            skip_hor_whitespace(data, end);

            const char *start = data;
            skip_until_newline(data, end);

            if (data != end && *data == '\n')
            {
                *data = 0;
                material_name = start;
                data++;
            }
            else
            {
                // Error ?
            }
            return true;
        }
        return false;
    }

    void split(std::vector<char*> &parts, char * const str, char delim)
    {
        char *s = str;

        while (*s != '\0')
        {
            while (*s == delim)
                s++;

            if (*s != '\0')
            {
                char *start = s;
                while (*s != '\0' && *s != delim)
                    s++;

                if (*s != '\0')
                {
                    *s = '\0';
                    s++;
                }
                parts.push_back(start);
            }
        }
    }

    bool parse_vector(char *line, Vector3 &vec)
    {
        std::vector<char *> parts;
        split(parts, line, ' ');
        if (parts.size() != 3)
            return false;

        vec.x = std::atof(parts.at(0));
        vec.y = std::atof(parts.at(1));
        vec.z = std::atof(parts.at(2));
        return true;
    }

    bool parse_vector2(char *line, Vector2 &vec)
    {
        std::vector<char *> parts;
        split(parts, line, ' ');
        if (parts.size() != 2)
            return false;

        vec.x = std::atof(parts.at(0));
        vec.y = std::atof(parts.at(1));
        return true;
    }

    char* trim(char *str)
    {
        while (*str != '\0')
            if (std::isspace(*str)) str++;
        return str;
    }

    bool read_vertex(char * &data, const char * const end, MeshBuilder &builder)
    {
        if (data != end && accept(data, end, "v "))
        {
            skip_hor_whitespace(data, end);

            char *start = data;
            skip_until_newline(data, end);

            if (data != end && *data == '\n')
            {
                *data = 0;
                data++;

                Vector3 position;
                if (parse_vector(start, position))
                {
                    builder.add_position(position);
                    return true;
                }
                log::error("Mesh obj: Invalid vertex");
                builder.add_position(Vector3::zero);
            }
            else
            {
                // Error ?
            }
        }
        return false;
    }

    bool read_normal(char *&data, const char * const end, MeshBuilder &builder)
    {
        if (data != end && accept(data, end, "vn "))
        {
            skip_hor_whitespace(data, end);

            char *start = data;
            skip_until_newline(data, end);

            if (data != end && *data == '\n')
            {
                *data = 0;
                data++;

                Vector3 normal;
                if (parse_vector(start, normal))
                {
                    builder.add_normal(normal);
                    return true;
                }
                log::error("Mesh obj: Invalid normal");
                builder.add_normal(Vector3::zero);
            }
            else
            {
                // Error ?
            }
        }
        return false;
    }

    bool read_tex_coord(char *&data, const char * const end, MeshBuilder &builder)
    {
        if (data != end && accept(data, end, "vt "))
        {
            skip_hor_whitespace(data, end);

            char *start = data;
            skip_until_newline(data, end);

            if (data != end && *data == '\n')
            {
                *data = 0;
                data++;

                Vector2 tex_coord;
                if (parse_vector2(start, tex_coord))
                {
                    builder.add_tex_coord(tex_coord);
                    return true;
                }
                log::error("Mesh obj: Invalid texture coordinate");
                builder.add_tex_coord(Vector2::zero);
            }
            else
            {
                // Error ?
            }
        }
        return false;
    }

    bool parse_face_vertex(char *line, int &ip, int &in, int &it, const MeshBuilder &builder)
    {
        std::vector<char *> parts;
        split(parts, line, '/');
        if (parts.size() == 0 || parts.size() > 3)
            return false;

        ip = std::atoi(parts.at(0));
        if (parts.size() > 1)
            it = std::atoi(parts.at(1));
        if (parts.size() > 2)
            in = std::atoi(parts.at(2));

        const int pos_count = static_cast<int>(builder.get_position_count());
        ip += (ip < 0) ? pos_count : -1;
        in += (ip < 0) ? pos_count : -1;
        it += (ip < 0) ? pos_count : -1;

        return true;
    }

    bool parse_face(char *line, MeshBuilder &builder)
    {
        std::vector<char *> parts;
        split(parts, line, ' ');
        if (parts.size() != 3 && parts.size() != 4)
        {
            log::error("Mesh obj: Invalid face");
            return false;
        }

        int ip1, in1, it1;
        if (!parse_face_vertex(parts.at(0), ip1, in1, it1, builder))
        {
            log::error("Mesh obj: Invalid face vertex");
            return false;
        }
        int ip2, in2, it2;
        if (!parse_face_vertex(parts.at(1), ip2, in2, it2, builder))
        {
            log::error("Mesh obj: Invalid face vertex");
            return false;
        }
        int ip3, in3, it3;
        if (!parse_face_vertex(parts.at(2), ip3, in3, it3, builder))
        {
            log::error("Mesh obj: Invalid face vertex");
            return false;
        }
        if (parts.size() == 4)
        {
            int ip4, in4, it4;
            if (!parse_face_vertex(parts.at(3), ip4, in4, it4, builder))
            {
                log::error("Mesh obj: Invalid face vertex");
                return false;
            }
//            builder.add_face(ip1, ip2, ip3, ip4,
//                            in1, in2, in3, in4,
//                            it1, it2, it3, it4);
            // Need to change the face vertex order (rh to lh coords)
//            builder.add_face(ip1, ip4, ip3, ip2,
//                            in1, in4, in3, in2,
//                            it1, it4, it3, it2);
            builder.add_face(ip2, ip1, ip4, ip3,
                            in2, in1, in4, in3,
                            it2, it1, it4, it3);
        }
        else
        {
//            builder.add_face(ip1, ip2, ip3,
//                            in1, in2, in3,
//                            it1, it2, it3);
            // Need to change the face vertex order (rh to lh coords)
            builder.add_face(ip1, ip3, ip2,
                            in1, in3, in2,
                            it1, it3, it2);
        }
        return true;
    }

    bool read_face(char *&data, const char * const end, MeshBuilder &builder)
    {
        if (data != end && accept(data, end, "f "))
        {
            skip_hor_whitespace(data, end);

            char *start = data;
            skip_until_newline(data, end);

            if (data != end && *data == '\n')
            {
                *data = 0;
                data++;

                if (parse_face(start, builder))
                    return true;

                log::error("Mesh obj: Invalid face");
            }
            else
            {
                // Error ?
            }
        }
        return false;
    }

    bool ObjMeshLoader::load(const char * const filepath, Mesh &mesh)
    {
        std::ifstream input;
        input.open(filepath);

        if (!input.is_open())
        {
            log::error("Mesh: Could not open file %", filepath);
            return false;
        }

        input.seekg(0, std::ios::end);
        const size_t data_size = input.tellg();
        input.seekg(0, std::ios::beg);

        std::vector<char> buffer;
        buffer.resize(data_size);
        char *data = buffer.data();
        input.read(data, data_size);

        input.close();

        MeshBuilder builder;

        const char *submesh_name = 0;
        const char *material_name = 0;

        bool submesh_started = false;

        const char * const end = data + data_size;
        while (data != end)
        {
            if (skip_empty(data, end)) continue;
            if (skip_mtllib(data, end)) continue;
            if (read_material(data, end, material_name))
                continue;

            if (read_object(data, end, submesh_name))
            {
                if (submesh_started)
                {
                    builder.add_sub_mesh(submesh_name, material_name);
                    submesh_started = false;
                }
                continue;
            }

            if (read_vertex(data, end, builder))
            {
                if (submesh_started)
                {
                    builder.add_sub_mesh(submesh_name, material_name);
                    submesh_started = false;
                }
                do {
                    skip_empty(data, end);
                } while (read_vertex(data, end, builder));
            }
            else if (read_tex_coord(data, end, builder))
            {
                if (submesh_started)
                {
                    builder.add_sub_mesh(submesh_name, material_name);
                    submesh_started = false;
                }
                do {
                    skip_empty(data, end);
                } while (read_tex_coord(data, end, builder));
            }
            else if (read_normal(data, end, builder))
            {
                if (submesh_started)
                {
                    builder.add_sub_mesh(submesh_name, material_name);
                    submesh_started = false;
                }
                do {
                    skip_empty(data, end);
                } while (read_normal(data, end, builder));
            }
            else if (read_face(data, end, builder))
            {
                submesh_started = true;
                do {
                    skip_empty(data, end);
                } while (read_face(data, end, builder));
            }
            else if (data != end)
            {
                skip_until_newline(data, end);
            }
        }
        if (submesh_started)
            builder.add_sub_mesh(submesh_name, material_name);

        builder.build(mesh);

        return true;
    }

} // der
