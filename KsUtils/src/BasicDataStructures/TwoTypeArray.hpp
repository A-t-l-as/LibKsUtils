#ifndef TWO_TYPE_ARRAY_HPP
#define TWO_TYPE_ARRAY_HPP

#include <cstddef>
#include <string>
#include "Utils/Globals.hpp"
#include "Utils/StringUtils.hpp"
#include "Utils/Misc.hpp"

namespace TypeOfTwoTypeArray
{
    constexpr static int first_type = 0;
    constexpr static int second_type = 1;
};

template <typename A, std::size_t A_SIZE, typename B, std::size_t B_SIZE>
class TwoTypeArray
{
public:


    A a[A_SIZE] = {};
    B b[B_SIZE] = {};


    void ExportToSs
    (
        const std::string& name,
        std::stringstream& output,
        std::size_t quadcount = Globals::two_quads
    )
    {
        Misc::Margin(output, quadcount);
        output << name << " ";

        for (std::size_t i = 0; i < A_SIZE; ++i)
        {
            output << this->a[i] << ", ";
        }

        const int64_t i64_b_size = static_cast<int64_t>(B_SIZE);
        for (int64_t i = 0; i < i64_b_size; ++i)
        {
            if (!(i == i64_b_size - 1))
            {
                output << this->b[i] << ", ";
            }
            else
            {
                output << this->b[i];
            }

        }

        output << std::endl;
    }




    void ExportToSsViaScheme
    (
        const std::string& name,
        std::string scheme,
        std::stringstream& output,
        std::size_t quadcount = Globals::two_quads
    )
    {
        Misc::Margin(output, quadcount);
        output << name << " ";

        StringUtils::RemoveCharFromTheString(scheme, ' ');

        size_t j = 0;
        size_t k = 0;
        const std::size_t scheme_len = scheme.size();
        for (size_t i = 0; i < scheme_len; ++i)
        {
            if (scheme[i] == 'a')
            {
                output << this->a[j];
                ++j;
            }

            if (scheme[i] == 'b')
            {
                output << this->b[k];
                ++k;
            }

            if (scheme[i] == ',')
            {
                output << ", ";
            }

        }

        output << std::endl;
    }


    void ParseFrom
    (
        const std::string& arg_line,
        const std::string& instruction_name,
        const std::string& format_a,
        const std::string& format_b
    )
    {
        if (arg_line.starts_with(instruction_name + ' '))
        {
            const std::size_t intruction_name_len = instruction_name.length() + 1;

            std::string help_str = arg_line.substr(intruction_name_len, arg_line.length() - intruction_name_len) + ',';

            std::size_t single_value_end_index;
            std::string single_value_str;

            StringUtils::RemoveCharFromTheString(help_str, ' ');
            StringUtils::RemoveCharFromTheString(help_str, '\n');

            //CONSOLE_OUT.PrintLn("\"", help_str, "\"");

            const int number_of_types = 2;
            std::size_t size_of_array = A_SIZE;

            for (int j = TypeOfTwoTypeArray::first_type; j < number_of_types; ++j)
            {
                for (size_t i = 0; i < size_of_array; ++i)
                {
                    single_value_end_index = help_str.find(",");

                    single_value_str = help_str.substr(0, single_value_end_index);

                    if ((j == TypeOfTwoTypeArray::first_type) && (size_of_array == A_SIZE) && (!single_value_str.empty()))
                    {
                        if (format_a == "%d")
                        {
                            this->a[i] = static_cast<A>(stoll(single_value_str, nullptr, 10));
                        }

                        if (format_a == "%u")
                        {
                            this->a[i] = static_cast<A>(stoull(single_value_str, nullptr, 10));
                        }

                        if (format_a == "%f")
                        {
                            this->a[i] = static_cast<A>(stof(single_value_str, nullptr));
                        }
                    }

                    if ((j == TypeOfTwoTypeArray::second_type) && (size_of_array == B_SIZE) && (!single_value_str.empty()))
                    {
                        if (format_b == "%d")
                        {
                            this->b[i] = static_cast<B>(stoll(single_value_str, nullptr, 10));
                        }

                        if (format_b == "%u")
                        {
                            this->b[i] = static_cast<B>(stoull(single_value_str, nullptr, 10));
                        }

                        if (format_b == "%f")
                        {
                            this->b[i] = static_cast<B>(stof(single_value_str, nullptr));
                        }
                    }

                    help_str = help_str.substr(single_value_end_index + 1, help_str.length() - single_value_end_index);

                }

                size_of_array = B_SIZE;

            }


        }
    }


    void ParseViaSchemeFrom
    (
        const std::string& arg_line,
        const std::string& instruction_name,
        const std::string& format_a,
        const std::string& format_b,
        std::string scheme
    )
    {
        if (arg_line.starts_with(instruction_name + ' '))
        {
            const std::size_t intruction_name_len = instruction_name.length() + 1;

            std::string help_str = arg_line.substr(intruction_name_len, arg_line.length() - intruction_name_len) + ',';

            std::size_t single_value_end_index;
            std::string single_value_str;

            StringUtils::RemoveCharFromTheString(help_str, ' ');
            StringUtils::RemoveCharFromTheString(help_str, '\n');

            StringUtils::RemoveCharFromTheString(scheme, ' ');
            StringUtils::RemoveCharFromTheString(scheme, ',');

            //CONSOLE_OUT.PrintLn("\"", help_str, "\"");

            size_t size_of_array = 0;

            const size_t temp_help_str_len = help_str.size();
            for (size_t i = 0; i < temp_help_str_len; ++i)
            {
                if (help_str[i] == ',')
                    ++size_of_array;
            }

            std::size_t j = 0;
            std::size_t k = 0;

            for (size_t i = 0; i < size_of_array; ++i)
            {
                single_value_end_index = help_str.find(",");

                single_value_str = help_str.substr(0, single_value_end_index);

                if ((!single_value_str.empty()) && (scheme[i] == 'a'))
                {
                    if (format_a == "%d")
                    {
                        this->a[j] = static_cast<A>(stoll(single_value_str, nullptr, 10));
                    }

                    if (format_a == "%u")
                    {
                        this->a[j] = static_cast<A>(stoull(single_value_str, nullptr, 10));
                    }

                    if (format_a == "%f")
                    {
                        this->a[j] = static_cast<A>(stof(single_value_str, nullptr));
                    }
                    ++j;
                }

                if ((!single_value_str.empty()) && (scheme[i] == 'b'))
                {
                    if (format_b == "%d")
                    {
                        this->b[k] = static_cast<B>(stoll(single_value_str, nullptr, 10));
                    }

                    if (format_b == "%u")
                    {
                        this->b[k] = static_cast<B>(stoull(single_value_str, nullptr, 10));
                    }

                    if (format_b == "%f")
                    {
                        this->b[k] = static_cast<B>(stof(single_value_str, nullptr));
                    }
                    ++k;
                }

                help_str = help_str.substr(single_value_end_index + 1, help_str.length() - single_value_end_index);

            }

        }
    }


    static void ParseArrayOfTwoTypeArrayFrom
    (
        const std::string& arg_line,
        const std::string& instruction_name,
        const std::string& format_a,
        const std::string& format_b,
        TwoTypeArray<A, A_SIZE, B, B_SIZE> dst[],
        std::size_t size
    )
    {
        if (arg_line.starts_with(instruction_name + ' ') )
        {
            const size_t intruction_name_len = instruction_name.length() + 1;

            std::string help_str = arg_line.substr(intruction_name_len, arg_line.length() - intruction_name_len) + ',';

            std::size_t single_value_end_index;
            std::string single_value_str;

            StringUtils::RemoveCharFromTheString(help_str, ' ');
            StringUtils::RemoveCharFromTheString(help_str, '\n');

            // CONSOLE_OUT.PrintLn("\"", help_str, "\"");

            for (std::size_t k = 0; k < size; ++k)
            {

                const int number_of_types = 2;
                std::size_t size_of_array = A_SIZE;
                for (int j = TypeOfTwoTypeArray::first_type; j < number_of_types; ++j)
                {
                    for (size_t i = 0; i < size_of_array; ++i)
                    {
                        single_value_end_index = help_str.find(",");

                        single_value_str = help_str.substr(0, single_value_end_index);

                        if ((j == TypeOfTwoTypeArray::first_type) && (size_of_array == A_SIZE) && (!single_value_str.empty()))
                        {
                            if (format_a == "%d")
                            {
                                dst[k].a[i] = static_cast<A>(stoll(single_value_str, nullptr, 10));
                            }

                            if (format_a == "%u")
                            {
                                dst[k].a[i] = static_cast<A>(stoull(single_value_str, nullptr, 10));
                            }

                            if (format_a == "%f")
                            {
                                dst[k].a[i] = static_cast<A>(stof(single_value_str, nullptr));
                            }
                        }

                        if ((j == TypeOfTwoTypeArray::second_type) && (size_of_array == B_SIZE) && (!single_value_str.empty()))
                        {
                            if (format_b == "%d")
                            {
                                dst[k].b[i] = static_cast<B>(stoll(single_value_str, nullptr, 10));
                            }

                            if (format_b == "%u")
                            {
                                dst[k].b[i] = static_cast<B>(stoull(single_value_str, nullptr, 10));
                            }

                            if (format_b == "%f")
                            {
                                dst[k].b[i] = static_cast<B>(stof(single_value_str, nullptr));
                            }
                        }

                        help_str = help_str.substr(single_value_end_index + 1, help_str.length() - single_value_end_index);

                    }

                    size_of_array = B_SIZE;

                }
            }

        }
    }



    static void ParseArrayOfTwoTypeArrayPlusBonusFrom
    (
        const std::string& arg_line,
        const std::string& instruction_name,
        const std::string& format_a,
        const std::string& format_b,
        TwoTypeArray<A, A_SIZE, B, B_SIZE> dst[],
        std::size_t size,
        A& single_bonus_value
    )
    {
        if ( arg_line.starts_with(instruction_name + ' ') )
        {
            const size_t intruction_name_len = instruction_name.length() + 1;

            std::string help_str = arg_line.substr(intruction_name_len, arg_line.length() - intruction_name_len) + ',';

            std::size_t single_value_end_index;
            std::string single_value_str;

            StringUtils::RemoveCharFromTheString(help_str, ' ');
            StringUtils::RemoveCharFromTheString(help_str, '\n');

            //CONSOLE_OUT.PrintLn("\"", help_str, "\"");

            const std::size_t temp_help_str_len = help_str.size();
            std::size_t counter = 0;
            for (size_t i = 0; i < temp_help_str_len; ++i)
            {
                if (help_str[i] == ',')
                    ++counter;
            }

            for (std::size_t k = 0; k < size; ++k)
            {

                const int number_of_types = 2;
                std::size_t size_of_array = A_SIZE;
                for (int j = TypeOfTwoTypeArray::first_type; j < number_of_types; ++j)
                {
                    for (size_t i = 0; i < size_of_array; ++i)
                    {
                        single_value_end_index = help_str.find(",");

                        single_value_str = help_str.substr(0, single_value_end_index);

                        if (
                            (j == TypeOfTwoTypeArray::first_type)
                            &&
                            (size_of_array == A_SIZE)
                            &&
                            (!single_value_str.empty()
                             )
                            )
                        {
                            if (format_a == "%d")
                            {
                                dst[k].a[i] = static_cast<A>(stoll(single_value_str, nullptr, 10));
                            }

                            if (format_a == "%u")
                            {
                                dst[k].a[i] = static_cast<A>(stoull(single_value_str, nullptr, 10));
                            }

                            if (format_a == "%f")
                            {
                                dst[k].a[i] = static_cast<A>(stof(single_value_str, nullptr));
                            }
                        }

                        if ((j == TypeOfTwoTypeArray::second_type) && (size_of_array == B_SIZE) && (!single_value_str.empty()))
                        {
                            if (format_b == "%d")
                            {
                                dst[k].b[i] = static_cast<B>(stoll(single_value_str, nullptr, 10));
                            }

                            if (format_b == "%u")
                            {
                                dst[k].b[i] = static_cast<B>(stoull(single_value_str, nullptr, 10));
                            }

                            if (format_b == "%f")
                            {
                                dst[k].b[i] = static_cast<B>(stof(single_value_str, nullptr));
                            }
                        }

                        help_str = help_str.substr(single_value_end_index + 1, help_str.length() - single_value_end_index);

                    }

                    size_of_array = B_SIZE;

                }
            }

            if (counter > static_cast<size_t>((A_SIZE + B_SIZE) * size) )
            {
                single_value_end_index = help_str.find(",");

                single_value_str = help_str.substr(0, single_value_end_index);

                if (!single_value_str.empty())
                {
                    if (format_a == "%d")
                    {
                        single_bonus_value = static_cast<A>(stoll(single_value_str, nullptr, 10));
                    }

                    if (format_a == "%u")
                    {
                        single_bonus_value = static_cast<A>(stoull(single_value_str, nullptr, 10));
                    }

                    if (format_a == "%f")
                    {
                        single_bonus_value = static_cast<A>(stof(single_value_str, nullptr));
                    }
                }

            }

        }
    }


    static void ExportArrayOfTwoTypeArrayToSs
    (
        const std::string& name,
        TwoTypeArray<A, A_SIZE, B, B_SIZE> arr[],
        int64_t size,
        std::size_t quadcount,
        std::stringstream& output,
        bool print_endl = true
    )
    {
        Misc::Margin(output, quadcount);
        output << name << " ";

        const int64_t i64_a_size = static_cast<int64_t>(A_SIZE);
        const int64_t i64_b_size = static_cast<int64_t>(B_SIZE);

        for (int64_t j = 0; j < size; ++j)
        {

            for (int64_t i = 0; i < i64_a_size; ++i)
            {
                output << arr[j].a[i] << ", ";
            }

            for (int64_t i = 0; i < i64_b_size; ++i)
            {
                if ( (j == size - 1) && (i == i64_b_size - 1) && (print_endl == true) )
                {
                    output << arr[j].b[i];
                    break;
                }

                output << arr[j].b[i] << ", ";
            }
        }

        if (print_endl == true)
        {
            output << std::endl;
        }
    }





};


#endif // !TWO_TYPE_ARRAY_HPP
