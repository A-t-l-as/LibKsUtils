#ifndef MY_SYSTEM_HPP
#define MY_SYSTEM_HPP

#include <filesystem>

namespace MySystem
{
    enum class ENFileType
    {
        none,
        file,
        directory
    };

    enum class ENAppType
    {
        invalid,
        console,
        gui
    };

    inline bool CheckThatDirExists(const std::filesystem::path& arg_input_directory_path)
    {
        return std::filesystem::exists(arg_input_directory_path) && std::filesystem::is_directory(arg_input_directory_path);
    }

    inline bool SafeLocalTime(const time_t* t, tm* tm_out)
    {
#ifdef _WIN32
        return localtime_s(tm_out, t) == 0;
#else
        return localtime_r(t, tm_out) != nullptr;
#endif
    }

};

#endif // !MY_SYSTEM_HPP
