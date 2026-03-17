#ifndef CPP_FILE_H
#define CPP_FILE_H

#include <filesystem>
#include <sstream>
#include <unordered_map>
#include <set>
#include <fstream>
#include "Compilator/CompilatorValueTypes.hpp"
#include "Console/Console.hpp"
#include "Utils/CppWriter.hpp"
#include "Utils/BitsetUtils.hpp"
#include "System/MySystem.hpp"

class CppFile
{
public:

	CppFile() = default;

	template<typename T>
	CppFile& operator<<(const T& value)
	{
		this->m_stream_data << value;
		return *this;
	}

	CppFile& operator<<(std::ostream& (*manip)(std::ostream&))
	{
		manip(this->m_stream_data);
		return *this;
	}

	std::string str() const { return this->m_stream_data.str(); }

	inline void WriteSymbol(char symbol, int count)
	{
		for (int i = 0; i < count; ++i)
			this->m_stream_data << symbol;
	}

	template<typename Type>
    inline void WriteTrivialValue(int quad_cound, const std::string& name, Type& value, bool make_new_line = true)
	{
		WriteSymbol('\t', quad_cound);
        CppWriter::WriteTrivialValue(this->m_stream_data, name, value, make_new_line);
	}

    template<typename Type, std::size_t Size>
    inline void WriteTrivialArray
    (
        int quad_count,
        const std::string& name,
        const std::array<Type, Size>& array,
        bool make_new_line = true
    )
	{
		size_t i = 0;
        for (const Type& value : array)
		{
			this->WriteTrivialValue(quad_count, name + std::to_string(i), value, make_new_line);
			++i;
		}
	}

    inline void WriteString(int quadcount, const std::string& name, const std::string& value, bool make_new_line = true)
	{
		WriteSymbol('\t', quadcount);
        CppWriter::WriteString(this->m_stream_data, name, value, make_new_line);
	}

    inline void WriteGuid(int quad_count, const std::string& name, const std::string& guid, bool make_new_line = true)
	{
		WriteSymbol('\t', quad_count);
        CppWriter::WriteGuid(this->m_stream_data, name, guid, make_new_line);
	}


	template<typename Type>
    void WriteUniqueKeyWithConst
    (
		int quad_count,
        const std::string& name,
		Type& value,
        std::unordered_map<Type, std::string>& list_of_masks,
		bool make_new_line = true
    )
	{
		
		if (list_of_masks[value] == "")
		{
			this->WriteTrivialValue(quad_count, name, value, make_new_line);
			return;
		}

		WriteSymbol('\t', quad_count);
		this->m_stream_data
            << sizeof(value) * Globals::byte_to_bits
            << CompilatorValueTypes::c_bituniquekey_type_str << " "
			<< name
			<< " = "
			<< list_of_masks[value]
			<< ";";
			
		if (make_new_line)
			this->m_stream_data << std::endl;
		
	}


	template<typename T>
    void WriteNBitMask
    (
		int quadcount, 
        const std::string& name,
		T& value, 
        const std::string& main_label,
		std::unordered_map<uint32_t, std::string>& list_of_masks
    )
	{
		std::set<std::string> masks_strings = {};

        const int digits = (sizeof(T) * Globals::byte_to_bits) / Globals::tetrad_to_bits;

		T hex_value_4bits[digits] = {};
		T and_mask = static_cast<T>(0x0000000F);

		this->m_stream_data << std::endl;
		WriteSymbol('\t', quadcount);
		this->m_stream_data << "// " << main_label << std::endl;
		WriteSymbol('\t', quadcount);

        this->m_stream_data
            << sizeof(value) * Globals::byte_to_bits
            << CompilatorValueTypes::c_bitmask_type_str
            << " "
            << name
            << " = (";

		for (int i = 0; i < digits; ++i)
		{
			hex_value_4bits[i] = value & and_mask;
            and_mask = and_mask << Globals::tetrad_to_bits;

			masks_strings.insert(list_of_masks[hex_value_4bits[i]]);
		}

		int j = 0;
		for (const std::string& mask : masks_strings)
		{
			this->m_stream_data << mask;

			if (j < masks_strings.size() - 1) this->m_stream_data << " | ";

			++j;
		}

		this->m_stream_data << ");" << std::endl << std::endl;

	}

    inline void Write1BitMask(int quad_count, const std::string& name, uint8_t value)
	{
		WriteSymbol('\t', quad_count);
        CppWriter::Write1BitMask(this->m_stream_data, name, value);
	}

    template<std::size_t Size>
	void WriteAll1BitMasks(
		int quad_count,
        const std::vector<std::string>& mask_labels,
        const std::array<uint8_t, Size>& array,
        const std::string& main_label
	)
	{
		std::string binary_value;

		this->m_stream_data << std::endl;
		WriteSymbol('\t', quad_count);
		this->m_stream_data << "// " << main_label << std::endl;

        const size_t array_size = array.size();
        for (int j = 0; j < array_size; ++j)
		{
            binary_value = BitsetUtils::DecByteToReversedBin(array[j]);

			for (int i = 0; i < binary_value.length(); ++i)
			{
				this->Write1BitMask(quad_count, mask_labels[j * binary_value.length() + i], binary_value[i]);
			}
		}

		this->m_stream_data << std::endl;

	}


    void WriteTime32(int quad_count, const std::string& name, uint32_t value)
    {
        time_t mytime = static_cast<time_t>(value);

        char buffer[64];
        tm time_info;

        this->WriteTrivialValue(quad_count, name, value, false);
        this->m_stream_data << " // ";

        //if (localtime_s(&time_info, &mytime) == 0)
        if ( MySystem::SafeLocalTime(&mytime, &time_info) )
        {
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &time_info);
            this->m_stream_data << buffer << std::endl;
        }
        else
        {
            DEBUG_PRINT("Incorrect time value: ", value, Mess::endl);
        }

    }

    void SaveToTextFile(const std::filesystem::path& path)
    {
        std::fstream file;
        file.open(path, std::ios::out);

        if (!file)
            throw std::runtime_error("The file cannot be opened for writing.");

        auto str_data(this->m_stream_data.str());
        file.write(&str_data[0], str_data.size());
        file.close();
    }

private:
	std::stringstream m_stream_data;


};


#endif // !CPP_FILE_H
