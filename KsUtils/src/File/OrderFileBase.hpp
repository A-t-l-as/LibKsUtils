#ifndef ORDER_FILE_BASE_HPP
#define ORDER_FILE_BASE_HPP

#include "../Utils/Globals.hpp"
#include "TextFile.hpp"

class OrderFileBase
{
public:
    OrderFileBase()
    {}

    void LoadFrom(const std::filesystem::path& arg_path, bool& arg_error_handler)
    {
        TextFile order_file;
        order_file.LoadFrom(arg_path);

        order_file.Informations();
        arg_error_handler = !(order_file.GetIsOpened());
        //order_file.ExitWhenFileDontExist();

        if (arg_error_handler == Globals::success_code)
        {
            this->GetOrderListFromFile(order_file);

            SpecialAction(arg_path, arg_error_handler);
        }
    }

    const std::vector<std::string>& GetOrderFileNames() const noexcept { return this->m_order_file_names; }

protected:

    void GetOrderListFromFile(const TextFile& order_file_buff)
    {
        const std::vector<char>& order_file_vec = order_file_buff.GetFileBuffer();
        const size_t order_file_vec_len = order_file_vec.size();

        size_t line_index = 0;
        size_t line_len = 0;

        for (size_t i = 0; i < order_file_vec_len; ++i)
        {
            if (order_file_vec[i] == '\n' || order_file_vec[i] == '\0')
            {
                const char* line_data = reinterpret_cast<const char*>(&order_file_vec[line_index]);

                std::string line = std::string(line_data, line_len);

                if (!line.empty())
                {
                    DEBUG_PRINT("line = \"", line, "\"", Mess::endl);

                    this->m_order_file_names.push_back(line);
                }

                line_len = 0;
                line_index = i + 1;
            }
            else
            {
                ++line_len;
            }

            if (order_file_vec[i] == '\0')
                break;

        }
    }

    virtual void SpecialAction(const std::filesystem::path& arg_path, bool& arg_error_handler)
    {}

    std::vector<std::string> m_order_file_names = {};

};



#endif // !ORDER_FILE_BASE_HPP
