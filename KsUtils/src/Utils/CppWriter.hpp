#ifndef CPP_WRITER_H
#define CPP_WRITER_H

#include <string>
#include <sstream>
#include <cstdint>
#include "Globals.hpp"
#include <unordered_map>

namespace CppWriter
{
	template<typename Type>
	inline std::string ConvertTrivialTypeToString()
	{
		std::stringstream result = { };

		if (std::is_unsigned_v<Type>)
		{
			result << 'u';
		}

        result << "int" << sizeof(Type) * Globals::byte_to_bits << "_t";

		return result.str();
	}


	template<typename Type>
	inline std::string ConvertTrivialTypeToString(Type)
	{
		return ConvertTrivialTypeToString<Type>();
	}

	template<typename Type>
    inline void WriteTrivialValue(std::stringstream& ss, const std::string& name, const Type& value, bool make_new_line = true)
	{
		ss << ConvertTrivialTypeToString(value) << " " << name << " = ";

        if (sizeof(Type) * Globals::byte_to_bits == Globals::byte_to_bits)
		{
			ss << static_cast<uint32_t>(value) << ";";
		}
		else
		{
			ss << value << ";";
		}

		if (make_new_line)
			ss << std::endl;
	}

    inline void WriteString(std::stringstream& ss, const std::string& name, const std::string& value, bool make_new_line = true)
    {
        ss << "string " << name << " = \"" << value << "\";";
        if (make_new_line)
            ss << std::endl;
    }

    template<typename Type>
    void WriteUniqueKeyWithConst(
        std::stringstream& ss,
        const std::string& name,
        const Type& value,
        std::unordered_map<Type, std::string>& list_of_masks,
        bool make_new_line = true
        )
    {

        if (list_of_masks[value] == "")
        {
            WriteTrivialValue( ss, name, value, make_new_line);
            return;
        }

        ss  << sizeof(value) * Globals::byte_to_bits
            << "bituniquekey "
            << name
            << " = "
            << list_of_masks[value]
            << ";";

        if (make_new_line)
            ss << std::endl;

    }

    inline void WriteGuid(std::stringstream& ss, const std::string& name, const std::string& guid, bool make_new_line = true)
    {
        ss << "GUID " << name << " = " << guid << ";";
        if (make_new_line)
            ss << std::endl;
    }

    inline void Write1BitMask(std::stringstream& ss, const std::string& name, uint8_t value)
    {
        ss << "1bitmask " << name << " = " << value << ";" << std::endl;
    }

};

namespace CW = CppWriter;

#endif // !CPP_WRITER_H
