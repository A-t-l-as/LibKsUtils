#ifndef COMPILATOR_VALUE_TYPES_H
#define COMPILATOR_VALUE_TYPES_H

#include <cstdio>

namespace CompilatorValueTypes
{
	constexpr char c_char_type_str[] = "char";
	constexpr char c_char_arr_type_str[] = "char[]";

	constexpr char c_uint8_t_type_str[] = "uint8_t";
	constexpr char c_uint8_t_arr_type_str[] = "uint8_t[]";

	constexpr char c_uint16_t_type_str[] = "uint16_t";
	constexpr char c_uint16_t_arr_type_str[] = "uint16_t[]";

	constexpr char c_uint32_t_type_str[] = "uint32_t";
	constexpr char c_uint32_t_arr_type_str[] = "uint32_t[]";

	constexpr char c_uint64_t_type_str[] = "uint64_t";
	constexpr char c_uint64_t_arr_type_str[] = "uint64_t[]";

	constexpr char c_guid_type_str[] = "GUID";
	constexpr char c_string_type_str[] = "string";

    constexpr char c_struct_type_str[] = "struct";

	constexpr char c_int8_t_type_str[] = "int8_t";
	constexpr char c_int8_t_arr_type_str[] = "int8_t[]";

	constexpr char c_int16_t_type_str[] = "int16_t";
	constexpr char c_int16_t_arr_type_str[] = "int16_t[]";

	constexpr char c_int32_t_type_str[] = "int32_t";
	constexpr char c_int32_t_arr_type_str[] = "int32_t[]";

	constexpr char c_int64_t_type_str[] = "int64_t";
	constexpr char c_int64_t_arr_type_str[] = "int64_t[]";

	constexpr char c_bool_type_str[] = "bool";
	constexpr char c_bool_arr_type_str[] = "bool[]";

	constexpr char c_1bitmask_type_str[] = "1bitmask";

	constexpr char c_32bitmask_type_str[] = "32bitmask";

	constexpr char c_8bitmask_type_str[] = "8bitmask";

    constexpr char c_32bituniquekey_type_str[] = "32bituniquekey";

    constexpr char c_16bituniquekey_type_str[] = "16bituniquekey";

    constexpr char c_namespace_type_str[] = "namespace";

    constexpr size_t number_of_compilator_value_types = 20;
	static const char* compilator_value_types[] =
	{
		c_1bitmask_type_str,
		c_uint32_t_type_str,
		c_uint8_t_type_str,
		c_uint16_t_type_str,
		c_uint64_t_type_str,
		c_string_type_str,
        c_32bituniquekey_type_str,
        c_16bituniquekey_type_str,
		c_bool_type_str,
		c_int32_t_type_str,
		c_int8_t_type_str,
		c_int16_t_type_str,
		c_int64_t_type_str,
		c_8bitmask_type_str,
		c_32bitmask_type_str,
		c_char_type_str,
		c_guid_type_str,
		c_struct_type_str,
        c_namespace_type_str,
		""
	};

	constexpr char c_bool_false_value[] = "false";
	constexpr char c_bool_true_value[]  = "true";
	constexpr char c_bool_zero_value[]  = "0";
	constexpr char c_bool_one_value[]   = "1";

};

namespace CVT = CompilatorValueTypes;

#endif // !COMPILATOR_VALUE_TYPES_H
