#ifndef BIN_FILE_H
#define BIN_FILE_H

#include "MyFile.hpp"
#include <cstring>

class BinFile : public MyFile<uint8_t>
{
public:
	BinFile() :
		MyFile(),
		m_current_offset(0)
	{}

    inline void WriteBuffer(const std::vector<uint8_t>& arg_buff)
	{
		m_file_buffer.insert(m_file_buffer.end(), arg_buff.begin(), arg_buff.end());
	}

    inline void WriteBuffer(const BinFile& arg_file)
	{
		m_file_buffer.insert(m_file_buffer.end(), arg_file.GetFileBuffer().begin(), arg_file.GetFileBuffer().end());
	}

    inline void WriteBuffer(const uint8_t* arg_buff, size_t buff_len)
	{
		m_file_buffer.insert(m_file_buffer.end(), arg_buff, arg_buff + buff_len);
    }

    /*
     * Szablony template<> sa obliczanie w czasie kompilacji, czyli:
     * -> rozmiar typu T bedzie znany podczas kompilacji (sizeof(T)),
     * -> ze wzgledu na to ze szablony sa obliczane w czasie kompilacji to
     * sizeof(T) zadziala.
    */

	template<typename T>
	inline void WriteValue(const T& val)
	{
        static_assert(std::is_trivially_copyable_v<T>,
			"WriteValue can only be used with trivially copyable types");

		auto ptr = reinterpret_cast<const uint8_t*>(&val);
		this->m_file_buffer.insert(this->m_file_buffer.end(), ptr, ptr + sizeof(T));
	}

	template<typename T>
	inline void ReadValue(T& value)
	{
		value = reinterpret_cast<T&>(this->m_file_buffer[m_current_offset]);
		m_current_offset += sizeof(T);
	}

    template<typename T>
    inline void ReadArray(T array[], size_t size_of_array)
    {
        const size_t number_of_bytes = size_of_array * sizeof(T);

        if (m_current_offset + number_of_bytes > m_file_buffer.size())
            throw std::out_of_range("ReadArray: buffer overflow");

        memcpy(array, &m_file_buffer[m_current_offset], number_of_bytes);
        m_current_offset += number_of_bytes;
    }

    template<typename T>
    void ReadArrayViaIndexes(T arr[], const std::vector<std::size_t>& indexes)
    {
        for (const std::size_t& index : indexes)
            ReadValue( arr[index] );
    }


    template<typename T>
    void ReadArrayReverse(T arr[], std::size_t arr_size)
    {
        for (int64_t i = arr_size - 1; i >= 0; --i)
            ReadValue( arr[i] );
    }


    template<typename T>
    void ReadArrayFromTo(T arr[], std::size_t from, std::size_t to)
    {
        static_assert(std::is_trivially_copyable_v<T>);
        if (from > to) return;

        std::size_t count = to - from + 1;
        std::size_t bytes = count * sizeof(T);

        std::memcpy(&arr[from], &m_file_buffer[m_current_offset], bytes);
        m_current_offset += bytes;
    }


    template<typename T>
    void WriteArrayFromTo(const T array[], std::size_t from, std::size_t to)
    {
        static_assert(std::is_trivially_copyable_v<T>,
                      "WriteArrayFromTo can only be used with trivially copyable types");
        if (from > to) return;

        const auto* ptr = reinterpret_cast<const uint8_t*>(array + from);

        const std::size_t byteCount = (to - from + 1) * sizeof(T);

        this->m_file_buffer.insert(this->m_file_buffer.end(), ptr, ptr + byteCount);
    }

    template<typename T>
    void WriteArrayViaIndexes(const T array[], const std::vector<std::size_t>& indexes)
    {
        for(const std::size_t& index : indexes)
            WriteValue( array[index] );
    }

    template<typename T>
    void WriteArrayFromToReverse(const T array[], int64_t from, int64_t to)
    {
        for (int64_t i = from; i >= to; --i)
            WriteValue( array[i] );
    }

    void WriteStringWithLen(const std::string& str, bool null_terminator_switch = false)
    {
        uint32_t size_of_str = static_cast<uint32_t>(str.length());

        if (null_terminator_switch == true)
        {
            size_of_str += sizeof(char);
        }

        WriteValue(size_of_str);
        m_file_buffer.insert(m_file_buffer.end(), str.begin(), str.end());

        if (null_terminator_switch == true)
        {
            WriteValue<char>('\0');
        }
    }

    template<typename T>
    void WriteVector(const std::vector<T>& values)
    {
        static_assert(std::is_trivially_copyable_v<T>,
                      "WriteVector can only be used with trivially copyable types");

        auto ptr = reinterpret_cast<const uint8_t*>(values.data());

        this->m_file_buffer.insert( this->m_file_buffer.end(), ptr, ptr + values.size() * sizeof(T));
    }


    void WriteVectorOfStringsWithLen(const std::vector<std::string>& strings)
    {
        WriteValue( static_cast<uint64_t>( strings.size() ) );

        for (const std::string& single_str : strings)
        {
            WriteStringWithLen(single_str);
        }
    }


    void WriteFirstStringFromVectorWithLen(const std::vector<std::string>& strings, bool null_terminator_switch = false)
    {
        std::string single_string = std::string();
        if ( !strings.empty() )
        {
            single_string = strings[0];
        }

        WriteStringWithLen(single_string, null_terminator_switch);
    }



    template<typename T>
    inline void ChangeValue(std::size_t arg_offset, const T& value)
    {
        if(!this->m_file_buffer.empty() && arg_offset < this->m_file_buffer.size() )
        {
            std::memcpy( &this->m_file_buffer[arg_offset], &value, sizeof(value) );
        }
        else
        {
            DEBUG_PRINT("template<typename T> inline void BinFile::ChangeValue(std::size_t arg_offset, const T& value):",
                        Mess::endl,
                        "Error!: You are trying to change the location of a file that you do not have access to.",
                        Mess::endl,
                        "BinFile::m_file_buffer.empty() = ", this->m_file_buffer.empty(), Mess::endl,
                        "BinFile::m_file_buffer.size() = ", this->m_file_buffer.size(), Mess::endl,
                        "arg_offset = ", arg_offset, Mess::endl);

        }
    }


    std::vector<uint8_t> ReadBuffer(size_t buffer_size)
    {
        std::vector<uint8_t> dst_buff(buffer_size);

        memcpy(dst_buff.data(), this->m_file_buffer.data() + this->m_current_offset, buffer_size);

        m_current_offset += buffer_size;

        return dst_buff;
    }

    std::string ReadLogicTable()
    {
        uint32_t len = 0;
        this->ReadValue(len);

        if (len > 0)
        {
            const size_t result_len = static_cast<size_t>(len) * 2 - 1;
            std::string result(result_len, ',');

            for (size_t i = 0; i < result_len; i += 2)
            {
                result[i] = this->m_file_buffer[this->m_current_offset] + '0';
                ++this->m_current_offset;
            }

            return result;
        }

        return std::string();
    }

    std::string ReadStringWithoutLen()
    {
        std::string result;
        uint8_t c;

        while (ReadValue(c), c != '\0')
            result.push_back(c);

        return result;
    }


	std::string ReadSimpleString();
	std::string ReadString();

    std::string ReadStringWithNullTerminator()
    {
        uint32_t length = 0;
        this->ReadValue(length);

        if (m_current_offset + length > m_file_buffer.size())
            throw std::out_of_range("String extends beyond buffer");

        std::string result(m_file_buffer.begin() + m_current_offset,
                           m_file_buffer.begin() + m_current_offset + length);

        m_current_offset += static_cast<std::size_t>(length);

        if (!result.empty() && result.back() == '\0')
            result.pop_back();

        return result;
    }



    void WriteGuid(const std::string& guid);
	std::string ReadGuid();

    void LoadFrom(const std::filesystem::path& arg_file_path) override
    {
        std::fstream input_file;
        input_file.open(arg_file_path, std::ios::in | std::ios::binary | std::ios::ate);

        if (input_file)
        {
            this->m_is_opened = this->m_is_open_now = true;

            size_t file_length = input_file.tellg();
            DEBUG_PRINT(arg_file_path, "::FILE_LEN = ", file_length, Mess::endl);

            input_file.seekg(0);

            this->m_file_buffer.resize(file_length);

            if (this->m_file_buffer.empty() == false && file_length > 0)
            {
                input_file.read(reinterpret_cast<char*>(&this->m_file_buffer[0]), file_length);
            }

            input_file.close();
            m_is_open_now = false;
        }


        SwitchToVector();
    }

    inline void SaveTo(const std::filesystem::path& arg_output_file_path)
	{
        BasicSaveTo(arg_output_file_path, std::ios_base::binary);
	}

    inline void MoveToBegin()
    {
        this->m_current_offset = 0;
    }

	inline void MoveOverBy(int64_t count)
	{
		this->m_current_offset += count;
	}

    std::size_t GetCurrentOffset() const { return this->m_current_offset; }

private:
    std::size_t m_current_offset;
	
};

#endif // !BIN_FILE_H
