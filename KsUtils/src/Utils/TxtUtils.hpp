#ifndef TXT_UTILS_HPP
#define TXT_UTILS_HPP

#include <string>
#include <sstream>
#include <queue>
#include <unordered_map>
#include <cstdint>
#include "Console/Console.hpp"
#include "Globals.hpp"
#include "StringUtils.hpp"
#include "Misc.hpp"

#include <filesystem>
#include <fstream>

namespace TxtUtils
{


inline void WriteSsToFile(const std::stringstream& output_stream, const std::filesystem::path& output_file_path)
{
    std::fstream output_file;

    output_file.open(output_file_path, std::ios::out);

    auto output_stream_data(output_stream.str());
    output_file.write(&output_stream_data[0], output_stream_data.size());

    output_file.close();
}



template<typename T>
static void WriteConstValueToSs
(
    const std::string name,
    const T& value,
    std::unordered_map<T, std::string>& list_of_masks,
    std::stringstream& output,
    std::size_t quadcount
)
{
    if (list_of_masks[value] == "" && value == 0) return;

    Misc::Margin(output, quadcount);

    if (list_of_masks[value] == "" && value != 0)
    {
        output << name << " " << value << std::endl;
        return;
    }

    output << name << " " << list_of_masks[value] << std::endl;
}


template<typename T>
inline void WriteOneValueToSs(const std::string& name, T value, std::stringstream& output, std::size_t margin_val = Globals::two_quads)
{
    Misc::Margin(output, margin_val); output << name << " " << value << std::endl;
}


template<typename T>
static bool GetOneValue(const std::string& arg_line, const std::string& instruction_name, T& dst)
{
    if ( !arg_line.starts_with(instruction_name + ' ') )
        return Globals::error_code;

    std::string value_str = arg_line.substr( instruction_name.length() + 1 );

    std::istringstream iss(value_str);
    if (!(iss >> dst))
        return Globals::error_code;

    DEBUG_PRINT("VALUE = ", std::to_string(dst), Mess::endl);

    return Globals::success_code;
}


template<typename T>
static void GetArrayValues
(
    const std::string& arg_line,
    const std::string& instruction_name,
    const std::string& format,
    T dst[],
    std::size_t size_of_dst
)
{

    if (arg_line.starts_with(instruction_name + ' '))
    {
        const size_t intruction_name_len = instruction_name.length() + 1;

        std::string help_str = arg_line.substr(intruction_name_len, arg_line.length() - intruction_name_len) + ',';

        std::size_t single_value_end_index;
        std::string single_value_str;

        StringUtils::RemoveCharFromTheString(help_str, ' ');
        StringUtils::RemoveCharFromTheString(help_str, '\n');

        for (size_t i = 0; i < size_of_dst; ++i)
        {
            single_value_end_index = help_str.find(",");

            single_value_str = help_str.substr(0, single_value_end_index);

            if (!single_value_str.empty())
            {
                if (format == "%d")
                {
                    dst[i] = static_cast<T>(stoll(single_value_str, nullptr, 10));
                }

                if (format == "%u")
                {
                    dst[i] = static_cast<T>(stoull(single_value_str, nullptr, 10));
                }

                if (format == "%f")
                {
                    dst[i] = static_cast<T>(stof(single_value_str, nullptr));
                }
            }

            help_str = help_str.substr(single_value_end_index + 1, help_str.length() - single_value_end_index);

        }
    }
}

template<typename T>
static void WriteArrayToSs
(
    const std::string& name,
    const T arr[],
    std::size_t size,
    std::stringstream& output,
    std::size_t quadcount = Globals::two_quads)
{
    Misc::Margin(output, quadcount);
    output << name << " ";

    for (std::size_t i = 0; i < size; ++i)
    {
        if (!(i == size - 1))
        {
            output << arr[i] << ", ";
        }
        else
        {
            output << arr[i];
        }

    }

    output << std::endl;
}




inline void WriteSingleStringToSs(const std::string& sin_str, std::stringstream& output, std::size_t margin_val = Globals::one_quad)
{
    Misc::Margin(output, margin_val); output << sin_str << std::endl;
}


inline bool GetSingleString
(
    const std::string& arg_line,
    const std::string& instruction_name,
    std::string& dst
)
{
    if ( !arg_line.starts_with(instruction_name + ' ') )
        return Globals::error_code;

    dst = arg_line.substr( instruction_name.length() + 1 );

    DEBUG_PRINT("STR = ", dst, Mess::endl);

    return Globals::success_code;
}


static void GetSingleStringFromQueue
(
    const std::string& arg_line,
    const std::string& instruction_name,
    std::string& dst,
    std::queue<std::string>& arg_q
)
{
    if ( arg_line.starts_with(instruction_name + ' ') )
    {
        if(!arg_q.empty())
        {
            dst = arg_q.front();
            arg_q.pop();
        }

        DEBUG_PRINT("STR = \"");
        DEBUG_PRINT(dst.c_str());
        DEBUG_PRINT("\"\n");
    }
}


static void GetSingleStringAndInterpretConst
(
    const std::string& arg_line,
    const std::string& instruction_name,
    uint32_t& dst,
    std::unordered_map<std::string, uint32_t>& list_of_consts
)
{
    if ( arg_line.starts_with(instruction_name + ' ') )
    {
        const size_t intruction_name_len = instruction_name.length() + 1;

        std::string help_str = arg_line.substr(intruction_name_len, arg_line.length() - intruction_name_len);

        dst = list_of_consts[help_str];

        DEBUG_PRINT("STR = ", help_str, Mess::endl);
        DEBUG_PRINT("Value = ", dst, Mess::endl);
    }
}


inline void PrepareString(std::string& arg_str, const std::string& sub_str)
{
    if (arg_str.find(sub_str) != std::string::npos)
        arg_str = sub_str;

}


inline bool GetObjType(const std::string& uppercased_instruction,
                       const std::string& original_instruction,
                       std::string& inside_obj_type,
                       std::string& inside_obj_name,
                       const std::string& obj_type_name)
{
    if ( !uppercased_instruction.starts_with(obj_type_name) )
        return Globals::error_code;


    inside_obj_type = original_instruction.substr(0, obj_type_name.length() - 1);

    GetSingleString(original_instruction, inside_obj_type, inside_obj_name);

    inside_obj_type = obj_type_name;
    inside_obj_type.pop_back();


    DEBUG_PRINT("m_inside_obj_type = |", inside_obj_type, "|", Mess::endl);
    DEBUG_PRINT("m_inside_obj_name = |", inside_obj_name, "|", Mess::endl);

    return Globals::success_code;
}


static void GetObjTypeFromQueue
(
    const std::string& arg_line,
    std::string& inside_obj_type,
    std::string& inside_obj_name,
    const std::string& obj_type_name,
    std::queue<std::string>& arg_q
)
{

    if (arg_line.starts_with(obj_type_name))
    {
        std::string obj_type_name_no_space = obj_type_name;
        obj_type_name_no_space.pop_back();

        inside_obj_type = arg_line.substr(0, obj_type_name.length() - 1);

        GetSingleStringFromQueue(arg_line, obj_type_name_no_space, inside_obj_name, arg_q);

        DEBUG_PRINT("m_inside_obj_type = |", inside_obj_type, "|", Mess::endl);
        DEBUG_PRINT("m_inside_obj_name = |", inside_obj_name, "|", Mess::endl);
    }
}

template<typename T>
inline void WriteVectorToSs
(
    const std::string& name,
    const std::vector<T>& vec,
    std::stringstream& output
)
{

    Misc::Margin(output, Globals::two_quads); output << name << " ";

    size_t j = 0;
    for (const T& single_element : vec)
    {
        output << single_element << ((j == vec.size() - 1) ? ("") : (", "));
        ++j;
    }
    output << std::endl;

}



static void GetVectorOfStrings
(
    const std::string& arg_line,
    const std::string& instruction_name,
    std::vector<std::string>& dst
)
{

    if ( arg_line.starts_with(instruction_name + ' ') )
    {
        const size_t intruction_name_len = instruction_name.length() + 1;

        std::string m_help_str = arg_line.substr(intruction_name_len, arg_line.length() - intruction_name_len);

        m_help_str += ",";

        std::size_t counter = 0;
        std::size_t single_value_end_index;
        std::string single_value_str;

        StringUtils::RemoveCharFromTheString(m_help_str, '\n');

        for (size_t i = 0; i < m_help_str.size(); ++i)
        {
            if (m_help_str[i] == ',')
                ++counter;
        }

        for (std::size_t i = 0; i < counter; ++i)
        {
            single_value_end_index = m_help_str.find(",");

            single_value_str = m_help_str.substr(0, single_value_end_index);

            if (!single_value_str.empty())
            {
                DEBUG_PRINT("STR = \"", single_value_str, "\"", Mess::endl);

                dst.push_back(single_value_str);
            }

            if (single_value_end_index < m_help_str.length() - 1)
            {
                m_help_str = m_help_str.substr(single_value_end_index + 2, m_help_str.length() - single_value_end_index);
            }
        }
    }
}


static void GetVectorOfStringsFromQueue
(
    const std::string& arg_line,
    const std::string& instruction_name,
    std::vector<std::string>& dst,
    std::queue<std::string>& arg_q
)
{

    if (arg_line.starts_with(instruction_name + ' ') )
    {
        std::string help_str;

        if (!arg_q.empty())
        {
            help_str = arg_q.front();
            arg_q.pop();
        }

        help_str += ",";

        std::size_t counter = 0;
        std::size_t single_value_end_index;
        std::string single_value_str;

        StringUtils::RemoveCharFromTheString(help_str, '\n');

        for (std::size_t i = 0; i < help_str.size(); ++i)
        {
            if (help_str[i] == ',')
                ++counter;
        }

        for (size_t i = 0; i < counter; ++i)
        {
            single_value_end_index = help_str.find(",");

            single_value_str = help_str.substr(0, single_value_end_index);

            if (!single_value_str.empty())
            {
                DEBUG_PRINT("STR = \"", single_value_str, "\"", Mess::endl);

                dst.push_back(single_value_str);
            }

            if (single_value_end_index < help_str.length() - 1)
            {
                help_str = help_str.substr(single_value_end_index + 2, help_str.length() - single_value_end_index);
            }
        }
    }
}



};

namespace TU = TxtUtils;

#endif // !TXT_UTILS_HPP
