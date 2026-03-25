#ifndef COMPILATOR_BASE_HPP
#define COMPILATOR_BASE_HPP


#include "HashMapsBase.hpp"
#include "../File/BinFile.hpp"
#include <bitset>
#include "../Utils/Globals.hpp"
#include <algorithm>

class CompilatorBase
{
public:
    CompilatorBase(
                   BinFile& arg_output_file_buffer,
                   HashMapsBase& arg_hash_maps,
                   bool& arg_error_handler,
                   bool arg_write_null_terminator_after_char_str = false
                   ) :
        r_output_file_buffer(arg_output_file_buffer),
        r_hash_maps(arg_hash_maps),
        r_error_handler(arg_error_handler),

        m_binary_value(0),
        m_bits_counter(0),

        write_null_terminator_after_char_str(arg_write_null_terminator_after_char_str)
    {}

    virtual void CompileFile(const std::filesystem::path& arg_input_path) = 0;

protected:
    bool write_null_terminator_after_char_str;

    HashMapsBase& r_hash_maps;
    BinFile& r_output_file_buffer;
    bool& r_error_handler;

    std::bitset<Globals::byte_to_bits> m_binary_value;
    uint32_t m_bits_counter;

    void CompileCppFileToBin(const std::vector <char>& arg_clean_file);

    template <typename T>
    void WriteTabToFile(const std::string& str)
    {
        this->r_output_file_buffer.WriteValue(static_cast<uint32_t>(CountValuesInTab(str)));

        T writed_value = 0;

        if (str == "{}") return;

        size_t instruction_index = 0;
        size_t instruction_len = 0;
        const size_t str_len = str.length();
        for (size_t i = 0; i < str_len; ++i)
        {
            if (str[i] == '{')
            {
                ++instruction_index;
                continue;
            }

            if ((str[i] == ',') || (str[i] == '}'))
            {
                std::string scaned_value = str.substr(instruction_index, instruction_len);

                if (std::is_unsigned_v<T>)
                {
                    writed_value = static_cast<T>(stoul(scaned_value));
                }
                else
                {
                    writed_value = static_cast<T>(stol(scaned_value));
                }

                this->r_output_file_buffer.WriteValue(writed_value);

                instruction_len = 0;
                instruction_index = i + 1;

                continue;
            }

            ++instruction_len;
        }
    }

    void WriteBooleanTabToFile(const std::string& str);

    void GetValuesFromString(
        std::string& arg_instruction,
        std::string& arg_value_type,
        Enums::ENCompilatorValueTypes& arg_en_value_type,
        std::string& arg_name,
        std::string& arg_value
        );


    virtual bool SpecialAction0(
        std::string& arg_instruction,
        std::string& arg_value_type,
        Enums::ENCompilatorValueTypes& arg_en_value_type,
        std::string& arg_name,
        std::string& arg_value
        )
    {
        return false;
    }

    virtual bool SpecialAction1(
        std::string& arg_instruction,
        std::string& arg_value_type,
        Enums::ENCompilatorValueTypes& arg_en_value_type,
        std::string& arg_name,
        std::string& arg_value
        )
    {
        return false;
    }


    virtual bool TryResolveSpecialValue(
        std::string& arg_instruction,
        std::string& arg_value_type,
        Enums::ENCompilatorValueTypes& arg_en_value_type,
        std::string& arg_name,
        std::string& arg_value
        )
    {
        return false;
    }


    void WriteInstructionToBin
    (
        const std::string& value_type,
        const Enums::ENCompilatorValueTypes& en_value_type,
        const std::string& value
    );


    inline uint64_t CountValuesInTab(const std::string& value)
    {
        uint64_t tabsize = std::ranges::count(value, ',');

        if (tabsize != 0) ++tabsize;

        return tabsize;
    }

    template <typename T>
    void WriteNBitMaskToFile(const std::string& str)
    {
        T writed_value = 0;

        if (str == "()") return;

        size_t instruction_index = 0;
        size_t instruction_len = 0;
        const size_t str_len = str.length();
        for (size_t i = 0; i < str_len; ++i)
        {
            if (str[i] == '(')
            {
                ++instruction_index;
                continue;
            }

            if ((str[i] == '|') || (str[i] == ')'))
            {
                std::string scanedvalue = str.substr(instruction_index, instruction_len);

                writed_value |= this->r_hash_maps.GetU32MaskFromString(scanedvalue);

                instruction_len = 0;
                instruction_index = i + 1;

                continue;
            }

            ++instruction_len;
        }

        this->r_output_file_buffer.WriteValue(writed_value);

    }

};




#endif // !COMPILATOR_BASE_HPP
