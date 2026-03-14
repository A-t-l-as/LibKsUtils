#ifndef MY_FILE_H
#define MY_FILE_H

#include <filesystem>
#include <vector>
#include <fstream>
#include "../Console/Console.hpp"

//-------------
// MyFile CLASS
//-------------

template<class Type>
class MyFile
{
public:
    MyFile() :
		m_file_path(std::filesystem::path()),
		m_is_opened(false),
        m_is_open_now(false),

        m_is_vector_fresh(false),
        m_is_ss_fresh(false)
	{
		DEBUG_PRINT("A new file has been initialized...", Mess::endl);
	}

	~MyFile() { this->m_file_buffer.clear();  }

    virtual void LoadFrom(const std::filesystem::path& arg_file_path) = 0;

    void BasicSaveTo(const std::filesystem::path& output_file_path, const std::ios_base::openmode& arg_mode)
	{
		std::fstream output_file;
		output_file.open(output_file_path, std::ios::out | arg_mode);

        if (!output_file)
            throw std::runtime_error("The file cannot be opened for writing.");

		if (this->m_file_buffer.empty() == false && this->m_file_buffer.size() > 0)
		{
			output_file.write(reinterpret_cast<char*>(&this->m_file_buffer[0]), this->m_file_buffer.size());
		}
		else
		{
			DEBUG_PRINT
			(
				Mess::endl, 
				Mess::endl,
                "void MyFile::SaveTo(fs::path & output_file_path, const ios_base::openmode & arg_mode):",
                Mess::endl,
				"Error while saving data - vector is empty!", Mess::endl, Mess::endl
			);
		}

		output_file.close();
	}

	void Informations() const
	{
		if (m_is_opened == false)
		{
			CONSOLE_OUT.Print(Mess::endl,
				"WARNING: The specified file could not be opened.", Mess::endl,
				"-------------------------------------", Mess::endl,
				"Opening the file:", this->m_file_path, Mess::endl,
				"Was the file successfully opened?: ", this->m_is_opened, Mess::endl,
				"Is the file currently open?: ", this->m_is_open_now, Mess::endl,
				"-------------------------------------", Mess::endl,
				Mess::endl);
		}

	}

	void ExitWhenFileDontExist() const
	{
		if (this->m_is_opened == false && this->m_is_open_now == false)
		{
			CONSOLE_OUT.PrintLn("The specified file does not exist!");
			exit(EXIT_FAILURE);
		}
	}

    inline void WriteString(const std::string& str, bool null_terminator = false)
    {
        SwitchToVector();

        m_file_buffer.insert(m_file_buffer.end(), str.begin(), str.end());

        if(null_terminator)
        {
            m_file_buffer.push_back('\0');
        }
    }

    inline void WriteStringStream(const std::stringstream& ss)
    {
        SwitchToVector();

        std::string str = ss.str();
        m_file_buffer.insert(m_file_buffer.end(), str.begin(), str.end());
    }

	//Gettery:
	bool GetIsOpened() const { return m_is_opened; }
	bool GetIsOpenNow() const { return m_is_open_now; }
	size_t GetFileLength() const { return this->m_file_buffer.size(); }
	const std::vector<Type>& GetFileBuffer() const noexcept { return this->m_file_buffer; }
	constexpr bool Empty() const noexcept { return this->m_file_buffer.empty(); }

protected:
	//Pola:
	std::vector<Type> m_file_buffer;

	std::filesystem::path m_file_path;
	bool m_is_opened;
	bool m_is_open_now;

    bool m_is_vector_fresh;
    bool m_is_ss_fresh;

    inline void SwitchToVector()
    {
        m_is_vector_fresh = true;
        m_is_ss_fresh     = false;
    }
};

#endif // !MY_FILE_H
