#ifndef SIMPLE_COMPILATOR_HPP
#define SIMPLE_COMPILATOR_HPP

#include "CompilatorBase.hpp"
#include "File/TextFile.hpp"
#include "HashMapsBase.hpp"

class SimpleCompilator : public CompilatorBase
{
public:
    SimpleCompilator(
        BinFile& arg_output_file_buffer,
        HashMapsBase& arg_hash_maps,
        bool& arg_error_handler
        ) :

        CompilatorBase(arg_output_file_buffer, arg_hash_maps, arg_error_handler)
    {}

    void CompileFile(const std::filesystem::path& arg_input_path) override
    {
        TextFile input_file;
        input_file.LoadFrom(arg_input_path);

        input_file.Informations();
        this->r_error_handler = !(input_file.GetIsOpened());

        if ((input_file.Empty() == false) && (this->r_error_handler == Globals::success_code))
        {
            input_file.DeleteComments();

            input_file.CleanCppFileBufferFromContrChars();

            CompileCppFileToBin(input_file.GetCleanFileBuffer());
        }
    }


};

#endif // !SIMPLE_COMPILATOR_HPP
