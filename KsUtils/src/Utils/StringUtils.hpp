#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

#include <string>
#include <algorithm>

namespace StringUtils
{

inline void ToUpper(std::string& str)
{
    transform(str.begin(), str.end(), str.begin(),
              [](unsigned char c) { return toupper(c); });
}

static std::string GetToUpper(const std::string& str)
{
    std::string result = str;

    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char c) { return toupper(c); });

    return result;
}

inline void RemoveControlChars(std::string& line)
{
    line.erase
        (
            std::remove_if(
                line.begin(),
                line.end(),

                [](unsigned char c)
                {
                    return std::iscntrl(c);
                }
                ),
            line.end()
            );
}

inline bool IsInString(char c, const std::string& str)
{
    return str.find(c) != std::string::npos;
}


inline void RemoveCharFromTheString(std::string& str, char symbol)
{
    str.erase( std::remove(str.begin(), str.end(), symbol), str.end() );
}


inline void ReplaceAllOccurrences(std::string& str_buffer, const std::string& search_str, const std::string& replace_str)
{
    if (search_str.empty()) return;

    std::size_t pos = str_buffer.find(search_str);

    while (pos != std::string::npos)
    {
        str_buffer.replace(pos, search_str.length(), replace_str);
        pos = str_buffer.find(search_str, pos + replace_str.length());
    }
}


inline bool IsNumber(const std::string& s)
{
    return !s.empty() && find_if(s.begin(),
                                 s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
}



};



#endif // !STRING_UTILS_HPP
