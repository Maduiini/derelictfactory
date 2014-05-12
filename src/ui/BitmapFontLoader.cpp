
#include "BitmapFontLoader.h"
#include "BitmapFont.h"

#include "../Types.h"
#include "../Log.h"

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <exception>

namespace der
{

    static constexpr uint8_t BmfVersion = 3;

    struct BmfInfoBlock
    {
        static constexpr uint8_t TYPE = 0x01;

        int16_t font_size;
        uint8_t bit_field;
        uint8_t char_set;
        uint16_t stretch_h;
        uint8_t anti_aliasing;
        uint8_t padding_up;
        uint8_t padding_right;
        uint8_t padding_down;
        uint8_t padding_left;
        uint8_t spacing_horizontal;
        uint8_t spacing_vertical;
        uint8_t outline;
    };

    struct BmfCommonBlock
    {
        static constexpr uint8_t TYPE = 0x02;

        uint16_t line_height;
        uint16_t base;
        uint16_t scale_w;
        uint16_t scale_h;
        uint16_t pages;
        uint8_t bit_field;
        uint8_t alpha_channel;
        uint8_t red_channel;
        uint8_t green_channel;
        uint8_t blue_channel;
    };

    struct BmfPageBlock
    {
        static constexpr uint8_t TYPE = 0x03;
    };

    struct BmfCharBlock
    {
        static constexpr uint8_t TYPE = 0x04;

        uint32_t id;
        uint16_t x;
        uint16_t y;
        uint16_t width;
        uint16_t height;
        int16_t offset_x;
        int16_t offset_y;
        uint16_t advance_x;
        uint8_t page;
        uint8_t channel;
    };

    struct BmfKerningPairBlock
    {
        static constexpr uint8_t TYPE = 0x05;

        uint32_t first;
        uint32_t second;
        int16_t amount;
    };

    class EndOfFile : public std::exception
    {
    public:
        virtual const char* what() const throw()
        {
            return "End of file";
        }
    };

    class ParseError : public std::exception
    {
    public:
        ParseError(const char * const msg)
            : m_message(msg)
        { }

        ParseError(std::string msg)
            : m_message(msg.c_str())
        { }

        virtual const char* what() const throw()
        {
            return m_message;
        }

    private:
        const char * const m_message;
    };

    template<typename T>
    std::string to_string(T value)
    {
        std::ostringstream string_stream;
        string_stream << value;
        return string_stream.str();
    }

    uint8_t read_uint8(std::ifstream &file)
    {
        char byte;
        file.read(&byte, 1);
        if (file.eof()) throw EndOfFile();
        return static_cast<uint8_t>(byte);
    }

    uint16_t read_uint16(std::ifstream &file)
    {
        char bytes[2];
        file.read(bytes, 2);
        if (file.eof()) throw EndOfFile();
        uint16_t value = static_cast<unsigned char>(bytes[0]) | (static_cast<unsigned char>(bytes[1]) << 8);
        return value;
    }

    int16_t read_int16(std::ifstream &file)
    {
        char bytes[2];
        file.read(bytes, 2);
        if (file.eof()) throw EndOfFile();
        int16_t value = bytes[0] | (bytes[1] << 8);
        return value;
    }

    uint32_t read_uint32(std::ifstream &file)
    {
        char bytes[4];
        file.read(bytes, 4);
        if (file.eof()) throw EndOfFile();
        uint32_t a = static_cast<unsigned char>(bytes[0]);
        uint32_t b = static_cast<unsigned char>(bytes[1]) << 8;
        uint32_t c = static_cast<unsigned char>(bytes[2]) << 16;
        uint32_t d = static_cast<unsigned char>(bytes[3]) << 24;
        uint32_t value = a | b | c | d;
        return value;
    }

    void read_string(std::ifstream &file)
    {
        // TODO
        char byte;
        for(;;)
        {
            file.read(&byte, 1);
            if (file.eof()) throw EndOfFile();
            if (byte == '\0')
                break;
        }
    }

    // static
    bool BitmapFontLoader::load(const char * const filepath, BitmapFont &font)
    {
        uint32_t largest_id = 0;
        std::vector<BmfCharBlock> character_blocks;
        BmfCommonBlock common_block;

        std::ifstream file(filepath, std::ifstream::in | std::ifstream::binary);

        if (!file.is_open())
        {
            log::error("BitmapFontLoader: Count not open file %", filepath);
            return false;
        }

        try
        {
            if (! (read_uint8(file) == 'B'
                && read_uint8(file) == 'M'
                && read_uint8(file) == 'F'))
            {
                throw ParseError("Invalid file header.");
            }

            uint8_t version = read_uint8(file);
            if (version != BmfVersion)
            {
                throw ParseError("Wrong version. Expected " + to_string(BmfVersion) + ", found " + to_string(static_cast<unsigned int>(version)));
            }

            while (file.peek() != std::istream::traits_type::eof())
            {
                uint8_t block_type = read_uint8(file);
                uint32_t block_size = read_uint32(file);

                switch (block_type)
                {
                case BmfInfoBlock::TYPE:
                    {
                        BmfInfoBlock block;
                        block.font_size = read_int16(file);
                        block.bit_field = read_uint8(file);
                        block.char_set = read_uint8(file);
                        block.stretch_h = read_uint16(file);
                        block.anti_aliasing = read_uint8(file);
                        block.padding_up = read_uint8(file);
                        block.padding_right = read_uint8(file);
                        block.padding_down = read_uint8(file);
                        block.padding_left = read_uint8(file);
                        block.spacing_horizontal = read_uint8(file);
                        block.spacing_vertical = read_uint8(file);
                        block.outline = read_uint8(file);
                        read_string(file);
                    }
                    break;

                case BmfCommonBlock::TYPE:
                    common_block.line_height = read_uint16(file);
                    common_block.base = read_uint16(file);
                    common_block.scale_w = read_uint16(file);
                    common_block.scale_h = read_uint16(file);
                    common_block.pages = read_uint16(file);
                    common_block.bit_field = read_uint8(file);
                    common_block.alpha_channel = read_uint8(file);
                    common_block.red_channel = read_uint8(file);
                    common_block.green_channel = read_uint8(file);
                    common_block.blue_channel = read_uint8(file);
                    break;

                case BmfPageBlock::TYPE:
                    read_string(file);
                    break;

                case BmfCharBlock::TYPE:
                    {
                        int number_of_characters = block_size / 20;
                        log::debug("BitmapFontLoader: number of characters in font file: %", number_of_characters);

                        for (int i=0; i<number_of_characters; i++)
                        {
                            BmfCharBlock block;
                            block.id = read_uint32(file);
                            block.x = read_uint16(file);
                            block.y = read_uint16(file);
                            block.width = read_uint16(file);
                            block.height = read_uint16(file);
                            block.offset_x = read_int16(file);
                            block.offset_y = read_int16(file);
                            block.advance_x = read_uint16(file);
                            block.page = read_uint8(file);
                            block.channel = read_uint8(file);
                            //log::debug("BitmapFontLoader: char id: % (%)", block.id, (char)block.id);
                            character_blocks.push_back(block);
                            if (block.id > largest_id)
                                largest_id = block.id;
                        }
                    }
                    break;

                case BmfKerningPairBlock::TYPE:
                    {
                        int number_of_kerning_pairs = block_size / 10;
                        for (int i=0; i<number_of_kerning_pairs; i++)
                        {
                            BmfKerningPairBlock block;
                            block.first = read_uint32(file);
                            block.second = read_uint32(file);
                            block.amount = read_int16(file);
                        }
                    }
                    break;

                default:
                    throw ParseError("Invalid block type " + to_string(static_cast<unsigned int>(block_type)));
                }
            }
        }
        catch (EndOfFile &eof)
        {
            log::error("BitmapFontLoader: Encountered end of file unexpectedly.");
            log::debug("File position: %", file.tellg());
            file.close();
            return false;
        }
        catch (ParseError &err)
        {
            log::error("BitmapFontLoader: Failed to parse font definition. Position: %\n       Reason: %", file.tellg(), err.what());
            file.close();
            return false;
        }

        file.close();

        font.m_base = common_block.base;
        font.m_line_height = common_block.line_height;
        font.reserve(largest_id+1);
        for (BmfCharBlock &cblock : character_blocks)
        {
            BitmapFontCharacter c;
            c.id = cblock.id;
            c.x = cblock.x;
            c.y = cblock.y;
            c.width = cblock.width;
            c.height = cblock.height;
            c.offset_x = cblock.offset_x;
            c.offset_y = cblock.offset_y;
            c.advance_x = cblock.advance_x;
            font.add_character(c);
        }

        return true;
    }

} // der
