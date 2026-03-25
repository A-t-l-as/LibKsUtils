#ifndef HASHMAPS_BASE_HPP
#define HASHMAPS_BASE_HPP

#include <unordered_map>
#include <string>
#include <cstdint>
#include "Enums.hpp"
#include "CompilatorValueTypes.hpp"

class HashMapsBase
{
public:
    HashMapsBase()
    {}

    void Init()
    {
        InitializeHashMapsOfConsts();
        InitializeHashMapOfMasks();
        InitializeHashMapOfValueTypesEnums();
        InitializeHashMapOfBooleanValues();
    }

    virtual void InitializeHashMapsOfConsts()
    {}

    virtual void InitializeHashMapOfMasks()
    {}

    void InitializeHashMapOfValueTypesEnums()
    {
        constexpr size_t number_of_value_types = 30;
        const char* value_types[] =
            {
                CVT::c_char_type_str,
                CVT::c_char_arr_type_str,

                CVT::c_uint8_t_type_str,
                CVT::c_uint8_t_arr_type_str,

                CVT::c_uint16_t_type_str,
                CVT::c_uint16_t_arr_type_str,

                CVT::c_uint32_t_type_str,
                CVT::c_uint32_t_arr_type_str,

                CVT::c_uint64_t_type_str,
                CVT::c_uint64_t_arr_type_str,

                CVT::c_guid_type_str,
                CVT::c_string_type_str,
                CVT::c_struct_type_str,

                CVT::c_int8_t_type_str,
                CVT::c_int8_t_arr_type_str,

                CVT::c_int16_t_type_str,
                CVT::c_int16_t_arr_type_str,

                CVT::c_int32_t_type_str,
                CVT::c_int32_t_arr_type_str,

                CVT::c_int64_t_type_str,
                CVT::c_int64_t_arr_type_str,

                CVT::c_bool_type_str,
                CVT::c_bool_arr_type_str,

                CVT::c_1bitmask_type_str,

                CVT::c_32bitmask_type_str,

                CVT::c_8bitmask_type_str,

                CVT::c_32bituniquekey_type_str,

                CVT::c_16bituniquekey_type_str,

                CVT::c_namespace_type_str,

                ""
            };


        for (size_t i = 0; i < number_of_value_types; ++i)
            this->m_hash_map_of_value_types_enums[std::string(value_types[i])] =
                static_cast<Enums::ENCompilatorValueTypes>(i);
    }


    void InitializeHashMapOfBooleanValues()
    {
        this->m_hash_map_of_boolean_values[ CVT::c_bool_false_value ] = false;
        this->m_hash_map_of_boolean_values[ CVT::c_bool_true_value  ] = true;
        this->m_hash_map_of_boolean_values[ CVT::c_bool_zero_value  ] = false;
        this->m_hash_map_of_boolean_values[ CVT::c_bool_one_value   ] = true;
    }

    uint16_t GetU16ConstFromString(const std::string& str)
    {
        return m_list_of_16_bit_consts[str];
    }

    uint32_t GetU32ConstFromString(const std::string& str)
    {
        return m_list_of_32_bit_consts[str];
    }

    uint32_t GetU32MaskFromString(const std::string& str)
    {
        return m_list_of_masks[str];
    }

    Enums::ENCompilatorValueTypes GetValueTypeEnFromString(const std::string& str)
    {
        return m_hash_map_of_value_types_enums[str];
    }

    bool GetBooleanValueFromString(const std::string& str)
    {
        return m_hash_map_of_boolean_values[str];
    }

protected:

    std::unordered_map<std::string, uint16_t> m_list_of_16_bit_consts = {};
    std::unordered_map<std::string, uint32_t> m_list_of_32_bit_consts = {};

    std::unordered_map<std::string, uint32_t> m_list_of_masks = {};

    std::unordered_map<std::string, Enums::ENCompilatorValueTypes> m_hash_map_of_value_types_enums = {};
    std::unordered_map<std::string, bool> m_hash_map_of_boolean_values = {};

};


#endif // !HASHMAPS_BASE_HPP
