#ifndef SAFE_FILE_WRITER_HPP
#define SAFE_FILE_WRITER_HPP

#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>

// Writes to a temporary file next to the target and only replaces the target
// on Commit(). If anything fails (or Commit() is never called), the temporary
// file is removed and the original file stays untouched.
//
// Usage:
//     SafeFileWriter output_file(path);
//     if (!output_file.Open(&error)) return false;
//     output_file.GetStream().write(...);
//     return output_file.Commit(&error);
class SafeFileWriter
{
public:

    explicit SafeFileWriter(const std::filesystem::path& arg_target_path) :
        m_target_path(arg_target_path),
        m_temp_path(arg_target_path),
        m_committed(false)
    {
        this->m_temp_path += ".safewrite.tmp";   // unusual suffix, won't hit a real file
        this->m_stream.open(this->m_temp_path, std::ios::binary | std::ios::trunc);
    }

    ~SafeFileWriter()
    {
        if (!this->m_committed)
        {
            this->m_stream.close();
            std::error_code ignored;
            std::filesystem::remove(this->m_temp_path, ignored);
        }
    }

    SafeFileWriter(const SafeFileWriter&) = delete;
    SafeFileWriter& operator=(const SafeFileWriter&) = delete;

    // Checks whether the temporary file could be created.
    bool Open(std::string* error) const
    {
        if (!this->m_stream.is_open())
        {
            SetError(error, "Cannot create " + PathText(this->m_target_path));
            return false;
        }

        return true;
    }

    std::ostream& GetStream() { return this->m_stream; }

    const std::filesystem::path& GetTargetPath() const { return this->m_target_path; }

    // Closes the temporary file and moves it in place of the target file.
    bool Commit(std::string* error)
    {
        this->m_stream.close();

        if (this->m_stream.fail())
        {
            SetError(error, "Failed to write " + PathText(this->m_target_path));
            return false;
        }

        std::error_code error_code;
        std::filesystem::rename(this->m_temp_path, this->m_target_path, error_code);

        if (error_code)
        {
            // Some platforms refuse to rename over an existing file.
            std::error_code ignored;
            std::filesystem::remove(this->m_target_path, ignored);
            error_code.clear();
            std::filesystem::rename(this->m_temp_path, this->m_target_path, error_code);
        }

        if (error_code)
        {
            SetError(error, "Cannot save " + PathText(this->m_target_path) + ": " + error_code.message());
            return false;
        }

        this->m_committed = true;
        return true;
    }

private:

    static void SetError(std::string* error, const std::string& message)
    {
        if (error)
        {
            *error = message;
        }
    }

    // Path as UTF-8 text for error messages (works in both C++17 and C++20).
    static std::string PathText(const std::filesystem::path& path)
    {
        const auto utf8 = path.u8string();
        return std::string(utf8.begin(), utf8.end());
    }

    std::filesystem::path m_target_path;
    std::filesystem::path m_temp_path;
    std::ofstream m_stream;
    bool m_committed;
};

#endif // SAFE_FILE_WRITER_HPP
