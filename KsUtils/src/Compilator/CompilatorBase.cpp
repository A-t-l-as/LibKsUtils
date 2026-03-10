#include "CompilatorBase.hpp"

using namespace std;

namespace fs = filesystem;


void CompilatorBase::CompileCppFileToBin(const vector<char>& arg_clean_file)
{
    string_view file_view(arg_clean_file.data(), arg_clean_file.size());

    string ins_value_type;
    string ins_name;
    string ins_value;
    Enums::value_types_enum en_ins_value_type = Enums::value_types_enum::e_invalid;

    size_t start = 0;
    while (true)
    {
        size_t end = file_view.find(';', start);

        if (end == string_view::npos)
            break;

        string instruction(file_view.substr(start, end - start + 1));

        DEBUG_PRINT("----", Mess::endl, instruction, Mess::endl);

        GetValuesFromString(instruction, ins_value_type, en_ins_value_type, ins_name, ins_value);
        WriteInstructionToBin(ins_value_type, en_ins_value_type, ins_value);

        DEBUG_PRINT(ins_value_type, Mess::endl, ins_name, Mess::endl, ins_value, Mess::endl, "----", Mess::endl);

        // Reset wartosci
        en_ins_value_type = Enums::value_types_enum::e_invalid;
        ins_value_type.clear();
        ins_name.clear();
        ins_value.clear();

        start = end + 1;
    }
}

void CompilatorBase::WriteBooleanTabToFile(const string& str)
{
    this->r_output_file_buffer.WriteValue(static_cast<uint32_t>(CountValuesInTab(str)));

    if (str == "{}") return;

    const char* ptr = str.data() + 1;
    const char* end = str.data() + str.size() - 1;

    while (ptr < end)
    {
        this->r_output_file_buffer.WriteValue( static_cast<uint8_t>( *ptr - '0' ) );
        ptr += 2;
    }
}


void CompilatorBase::GetValuesFromString(
    string& arg_instruction,
    string& arg_value_type,
    Enums::value_types_enum& arg_en_value_type,
    string& arg_name,
    string& arg_value
    )
{
    while (1)
    {

        bool nopswitch = true;

        // Sprawdzenie czy jest prawidlowy typ
        for (uint32_t i = 0; i < CVT::number_of_compilator_value_types; ++i)
        {
            if (arg_instruction.starts_with(CVT::compilator_value_types[i]) )
            {
                arg_value_type = CVT::compilator_value_types[i];
                nopswitch = false;
                break;
            }
        }

        // Jesli typ byl nieprawidlowy czyli nopswitch == true to wychodze
        if (nopswitch == true)
        {
            arg_value_type = string();

            arg_en_value_type = Enums::value_types_enum::e_invalid;

            arg_name = string();
            arg_value = string();
            return;
        }
        //-----------------------------------

        // Wyodrebnienie nazwy od instrukcji
        const size_t val_type_len = arg_value_type.length();
        size_t eq_index = arg_instruction.find('=');
        size_t bracket_index = arg_instruction.find('[');
        if (eq_index != string::npos)
        {
            arg_name = arg_instruction.substr(val_type_len, eq_index - val_type_len);
        }

        if (bracket_index != string::npos)
        {
            arg_value_type += "[]";
            arg_name = arg_instruction.substr(val_type_len, bracket_index - val_type_len);
        }
        //----------------------------------

        arg_en_value_type = this->r_hash_maps.GetValueTypeEnFromString(arg_value_type);

        // Wyodrebnienie wartosci od instrukcji
        uint64_t valueindex = eq_index + 1;
        arg_value = arg_instruction.substr(valueindex, arg_instruction.length() - valueindex - 1);
        //-------------------------------------


        // Typ string lub tablica char[] lub GUID lub samo char
        if (
            arg_en_value_type == Enums::value_types_enum::e_string   ||
            arg_en_value_type == Enums::value_types_enum::e_char_arr ||
            arg_en_value_type == Enums::value_types_enum::e_GUID     ||
            arg_en_value_type == Enums::value_types_enum::e_char
            )
        {
            arg_value = arg_instruction.substr(valueindex + 1, arg_instruction.length() - (valueindex + 1) - 2);
            return;
        }

        // ( ... -- SLOT 0 )
        if(
            SpecialAction0(
                arg_instruction,
                arg_value_type,
                arg_en_value_type,
                arg_name,
                arg_value
                )
            )
        {
            return;
        }
        //--------------------


        // Typ struktury lub namespace
        if (
            arg_en_value_type == Enums::value_types_enum::e_struct
            ||
            arg_en_value_type == Enums::value_types_enum::e_namespace
            )
        {

            // ( ... -- SLOT 1 )
            if(
                SpecialAction1(
                    arg_instruction,
                    arg_value_type,
                    arg_en_value_type,
                    arg_name,
                    arg_value
                    )
                )
            {
                return;
            }
            //--------------------


            valueindex = arg_instruction.find("{") + 1;
            arg_instruction = arg_instruction.substr(valueindex, arg_instruction.length() - valueindex);

            arg_value_type = string();

            arg_en_value_type = Enums::value_types_enum::e_invalid;

            arg_name = string();
            arg_value = string();

            continue;
        }

        // Typ 1bitmask
        if (arg_en_value_type == Enums::value_types_enum::e_1bitmask)
        {
            this->m_binary_value.set(this->m_bits_counter, static_cast<bool>(arg_value[0] - '0' ));
            ++this->m_bits_counter;
            return;
        }

        if(
            TryResolveSpecialValue(
                arg_instruction,
                arg_value_type,
                arg_en_value_type,
                arg_name,
                arg_value
                )
            )
        {
            return;
        }

        break;
    }

}


void CompilatorBase::WriteInstructionToBin(const string& value_type, const Enums::value_types_enum& en_value_type, const string& value)
{

    if (en_value_type == Enums::value_types_enum::e_invalid)
        return;

    // 1bitmask
    if ((en_value_type == Enums::value_types_enum::e_1bitmask) && (m_bits_counter == Globals::byte_to_bits))
    {
        this->r_output_file_buffer.WriteValue( static_cast<uint8_t>(m_binary_value.to_ulong()) );
        m_binary_value.reset();
        m_bits_counter = 0;
        return;
    }

    // uints
    if (value_type.starts_with('u'))
    {
        // uint32_t
        if (en_value_type == Enums::value_types_enum::e_uint32_t)
        {
            this->r_output_file_buffer.WriteValue(static_cast<uint32_t>(stoul(value)));
            return;
        }

        // uint8_t
        if (en_value_type == Enums::value_types_enum::e_uint8_t)
        {
            this->r_output_file_buffer.WriteValue( static_cast<uint8_t>( stoul(value) ) );
            return;
        }

        // uint16_t
        if (en_value_type == Enums::value_types_enum::e_uint16_t)
        {
            this->r_output_file_buffer.WriteValue(static_cast<uint16_t>(stoul(value)));
            return;
        }

        // uint64_t
        if (en_value_type == Enums::value_types_enum::e_uint64_t)
        {
            this->r_output_file_buffer.WriteValue(static_cast<uint64_t>(stoull(value)));
            return;
        }

        // uint32_t[]
        if (en_value_type == Enums::value_types_enum::e_uint32_t_arr)
        {
            WriteTabToFile<uint32_t>(value);
            return;
        }


        // uint8_t[]
        if (en_value_type == Enums::value_types_enum::e_uint8_t_arr)
        {
            WriteTabToFile<uint8_t>(value);

            return;
        }

        // uint16_t[]
        if (en_value_type == Enums::value_types_enum::e_uint16_t_arr)
        {
            WriteTabToFile<uint16_t>(value);

            return;
        }

        // uint64_t[]
        if (en_value_type == Enums::value_types_enum::e_uint64_t_arr)
        {
            WriteTabToFile<uint64_t>(value);

            return;
        }
    }

    // string
    if (en_value_type == Enums::value_types_enum::e_string)
    {
        uint32_t string_len = static_cast<uint32_t>(value.size());
        this->r_output_file_buffer.WriteValue(string_len);
        this->r_output_file_buffer.WriteString(value);
        return;
    }

    // 32bituniquekey
    if (en_value_type == Enums::value_types_enum::e_32bituniquekey)
    {
        this->r_output_file_buffer.WriteValue( this->r_hash_maps.GetU32ConstFromString(value) );
        return;
    }

    // bool
    if (en_value_type == Enums::value_types_enum::e_bool)
    {
        bool boolean_val = this->r_hash_maps.GetBooleanValueFromString(value);
        this->r_output_file_buffer.WriteValue(boolean_val);
        return;
    }

    // bool[]
    if (en_value_type == Enums::value_types_enum::e_bool_arr)
    {
        WriteBooleanTabToFile(value);
        return;
    }


    if ( value_type.starts_with('i') )
    {

        // int32_t albo int
        if (en_value_type == Enums::value_types_enum::e_int32_t)
        {
            this->r_output_file_buffer.WriteValue(static_cast<int32_t>(stol(value)));
            return;
        }

        // int8_t
        if (en_value_type == Enums::value_types_enum::e_int8_t)
        {
            this->r_output_file_buffer.WriteValue(static_cast<int8_t>(stol(value)));
            return;
        }

        // int16_t
        if (en_value_type == Enums::value_types_enum::e_int16_t)
        {
            this->r_output_file_buffer.WriteValue(static_cast<int16_t>(stol(value)));
            return;
        }

        // int64_t
        if (en_value_type == Enums::value_types_enum::e_int64_t)
        {
            this->r_output_file_buffer.WriteValue(static_cast<int64_t>(stoll(value)));
            return;
        }


        // int32_t[] albo int[]
        if (en_value_type == Enums::value_types_enum::e_int32_t_arr)
        {
            WriteTabToFile<int32_t>(value);
            return;
        }

        // int8_t[]
        if (en_value_type == Enums::value_types_enum::e_int8_t_arr)
        {
            WriteTabToFile<int8_t>(value);

            return;
        }


        // int16_t[]
        if (en_value_type == Enums::value_types_enum::e_int16_t_arr)
        {
            WriteTabToFile<int16_t>(value);

            return;
        }


        // int64_t[]
        if (en_value_type == Enums::value_types_enum::e_int64_t_arr)
        {
            WriteTabToFile<int64_t>(value);

            return;
        }

    }

    // 8bitmask
    if (en_value_type == Enums::value_types_enum::e_8bitmask)
    {
        WriteNBitMaskToFile<uint8_t>(value);

        return;
    }

    // 32bitmask
    if (en_value_type == Enums::value_types_enum::e_32bitmask)
    {
        WriteNBitMaskToFile<uint32_t>(value);

        return;
    }


    //  char
    if (en_value_type == Enums::value_types_enum::e_char)
    {
        this->r_output_file_buffer.WriteString(value);
        return;
    }

    //  char[]
    if (en_value_type == Enums::value_types_enum::e_char_arr)
    {
        this->r_output_file_buffer.WriteString(value, this->write_null_terminator_after_char_str);
        return;
    }


    // GUID
    if (en_value_type == Enums::value_types_enum::e_GUID)
    {
        this->r_output_file_buffer.WriteGuid(value);
        return;
    }

    // 16bituniquekey
    if (en_value_type == Enums::value_types_enum::e_16bituniquekey)
    {
        this->r_output_file_buffer.WriteValue(  this->r_hash_maps.GetU16ConstFromString(value) );
        return;
    }

}



