#ifndef TEXT_FILE_H
#define TEXT_FILE_H

#include "MyFile.hpp"
#include "../Utils/Misc.hpp"

class TextFile : public MyFile<char>
{
public:
	TextFile() : 
        MyFile()
	{}

    template<typename T>
    TextFile& operator<<(const T& value)
    {
        SwitchToStringStream();

        this->m_text_file_stream << value;
        return *this;
    }

    TextFile& operator<<(std::ostream& (*manip)(std::ostream&))
    {
        SwitchToStringStream();

        manip(this->m_text_file_stream);
        return *this;
    }

    std::string str() const { return this->m_text_file_stream.str(); }

    void DeleteComments()
    {
        bool its_comment = false;

        const size_t file_len = this->m_file_buffer.size();
        for (size_t i = 0; i < file_len; ++i)
        {
            if (this->m_file_buffer[i] == '\0')
            {
                this->m_file_buffer.resize(i + 1);
                break;
            }

            if ((this->m_file_buffer[i] == '/') && (this->m_file_buffer[i + 1] == '/'))
            {
                its_comment = !its_comment;
            }

            if ( this->m_file_buffer[i] == '\n' && its_comment == true)
            {
                its_comment = !its_comment;
            }

            if (its_comment == true)
            {
                this->m_file_buffer[i] = ' ';
                continue;
            }
        }

    }

    void CleanTxtFileBufferFromMultiSpaces(bool change_newline_switch = true)
    {
        bool its_space = false;

        const size_t file_len = this->m_file_buffer.size();
        this->m_clean_file.resize(file_len);

        size_t current_offset_in_clean_file = 0;
        for (size_t i = 0; i < file_len; ++i)
        {

            if (m_file_buffer[i] == ' ' && its_space == false)
            {
                m_clean_file[current_offset_in_clean_file] = m_file_buffer[i];
                ++current_offset_in_clean_file;

                its_space = true;
                continue;
            }

            if (m_file_buffer[i] != ' ')
            {
                its_space = false;
            }

            if (its_space == true)
            {
                continue;
            }

            if (m_file_buffer[i] == '\t')
            {
                continue;
            }

            if ((m_file_buffer[i] == '\n') && (change_newline_switch == true))
            {
                m_clean_file[current_offset_in_clean_file] = ';';
                ++current_offset_in_clean_file;

                continue;
            }

            m_clean_file[current_offset_in_clean_file] = m_file_buffer[i];
            ++current_offset_in_clean_file;

        }


        this->m_clean_file.resize(current_offset_in_clean_file + 1);
    }

    void CleanCppFileBufferFromContrChars()
    {
        size_t new_size = 0;
        bool its_string = false;

        const size_t file_len = this->m_file_buffer.size();
        this->m_clean_file.resize(file_len);
        for (size_t i = 0; i < file_len; ++i)
        {
            if (this->m_file_buffer[i] == '\"')
            {
                its_string = !its_string;
            }

            if ((this->m_file_buffer[i] <= 32) && (its_string == false))
            {
                continue;
            }

            this->m_clean_file[new_size] = this->m_file_buffer[i];
            ++new_size;
        }

        this->m_clean_file.resize(new_size + 1);
    }


    const std::vector<char>& GetCleanFileBuffer() const noexcept { return this->m_clean_file; }

	inline size_t CountOccurrencesOfText(const std::string& word)
	{
		return Misc::CountOccurrences(reinterpret_cast<const char*>(this->m_file_buffer.data()), this->m_file_buffer.size(), word);
	}

    void LoadFrom(const std::filesystem::path& arg_file_path) override
    {
        std::fstream input_file;
        input_file.open(arg_file_path, std::ios::in);

        if (input_file)
        {
            this->m_is_opened = this->m_is_open_now = true;

            this->m_file_buffer = std::vector<char>(std::istreambuf_iterator<char>(input_file), {});

            input_file.close();
            m_is_open_now = false;

            DEBUG_PRINT(arg_file_path, "::FILE_LEN = ", m_file_buffer.size(), Mess::endl);
        }

        SwitchToVector();
    }

    inline void SaveTo(const std::filesystem::path& arg_output_file_path)
	{
        BasicSaveTo(arg_output_file_path, std::ios::out);
	}

    inline void SaveOnlyStringStream(const std::filesystem::path& arg_output_file_path)
    {
        std::fstream output_file;
        output_file.open(arg_output_file_path, std::ios::out);

        if (!output_file)
            throw std::runtime_error("The file cannot be opened for writing.");

        auto str_data(this->m_text_file_stream.str());
        output_file.write(&str_data[0], str_data.size());
        output_file.close();
    }

    inline void Sync()
    {
        if(m_is_ss_fresh)
        {
            this->m_file_buffer.clear();

            std::string str = m_text_file_stream.str();
            this->m_file_buffer.insert(this->m_file_buffer.end(), str.begin(), str.end());
        }

        if(m_is_vector_fresh)
        {
            m_text_file_stream = std::stringstream();
            m_text_file_stream.write(this->m_file_buffer.data(), this->m_file_buffer.size());
        }
    }


    inline void ClearCleanBuffer()
    {
        this->m_clean_file.clear();
    }

    std::string GetRepresentationInString() const
    {
        return std::string(m_file_buffer.begin(), m_file_buffer.end());
    }

private:

    std::stringstream m_text_file_stream;
	std::vector <char> m_clean_file = {};

    inline void SwitchToStringStream()
    {
        m_is_vector_fresh = false;
        m_is_ss_fresh     = true;
    }

};


#endif // !TEXT_FILE_H
