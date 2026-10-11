#ifndef MY_MATH_UTILS_HPP
#define MY_MATH_UTILS_HPP

#include <stdexcept>

namespace MyMathUtils
{
    template <typename T>
    T SafeDiv(T a, T b)
    {
        if (b == T{})
            throw
            std::runtime_error
            ("Error: Division by zero (" + std::to_string(a) + "/" + std::to_string(b) + ")!");

        return a / b;
    }

    template <typename T>
    T SafeMod(T a, T b)
    {
        if (b == T{})
            throw
                std::runtime_error
                ("Error: Division by zero (" + std::to_string(a) + "%" + std::to_string(b) + ")!");

        return a % b;
    }

}


#endif // MY_MATH_UTILS_HPP end
