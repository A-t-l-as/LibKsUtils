#include <format>
#include "BinFile.hpp"
#include "../Utils/Globals.hpp"
#include "../System/Platform.hpp"

using namespace std;
namespace fs = filesystem;

string BinFile::ReadSimpleString()
{
	uint8_t temp_length_of_string = 0;
	this->ReadValue(temp_length_of_string);
	
	const size_t length_of_string = static_cast<size_t>(temp_length_of_string);
	
	if (this->m_current_offset + length_of_string > this->m_file_buffer.size())
		throw runtime_error("Error: string BinFile::ReadSimpleString(): Buffer overflow in ReadSimpleString()");

	// Bez kopiowania po jednym bajcie
	const char* str_data = reinterpret_cast<const char*>(&this->m_file_buffer[this->m_current_offset]);
	string result(str_data, length_of_string);

	this->m_current_offset += length_of_string;
	return result;
}


string BinFile::ReadString()
{
	uint32_t length_of_string = 0;
	this->ReadValue(length_of_string);

	if (this->m_current_offset + length_of_string > this->m_file_buffer.size())
		throw runtime_error("Error: string BinFile::ReadString(): Buffer overflow in ReadString()");

	// Bez kopiowania po jednym bajcie
	const char* str_data = reinterpret_cast<const char*>( &this->m_file_buffer[this->m_current_offset] );
	string result(str_data, length_of_string);

	this->m_current_offset += length_of_string;
	return result;
}

string BinFile::ReadGuid()
{
	constexpr const int static number_of_uint16_fields = 2;

	string my_guid = string();
	uint32_t four_bytes = 0;
	uint16_t two_bytes = 0;

	this->ReadValue(four_bytes);
	my_guid += "{" + format("{:08X}", four_bytes);

	for (int i = 0; i < number_of_uint16_fields; ++i)
	{
		this->ReadValue(two_bytes);
		my_guid += "-" + format("{:04X}", two_bytes);
	}

	this->ReadValue(two_bytes);
    my_guid += "-" + format("{:04X}", bswap16(two_bytes)) + "-";

	this->ReadValue(four_bytes);
    my_guid += format("{:08X}", bswap32(four_bytes));

	this->ReadValue(two_bytes);
    my_guid += format("{:04X}", bswap16(two_bytes)) + "}";

	return my_guid;
}

void BinFile::WriteGuid(const string &guid)
{
	uint32_t four_bytes = stoul(guid.substr(0, 8), nullptr, Globals::hex_base);
	this->WriteValue(four_bytes);

	uint16_t two_bytes = static_cast<uint16_t>(stoul(guid.substr(9, 4), nullptr, Globals::hex_base));
	this->WriteValue(two_bytes);

	two_bytes = static_cast<uint16_t>(stoul(guid.substr(14, 4), nullptr, Globals::hex_base));
	this->WriteValue(two_bytes);

    two_bytes = static_cast<uint16_t>(bswap16(static_cast<unsigned short>(stoul(guid.substr(19, 4), nullptr, Globals::hex_base))));
	this->WriteValue(two_bytes);

    four_bytes = static_cast<uint32_t>(bswap32(stoul(guid.substr(24, 8), nullptr, Globals::hex_base)));
	this->WriteValue(four_bytes);

    two_bytes = static_cast<uint32_t>(bswap16(static_cast<unsigned short>(stoul(guid.substr(32, 4), nullptr, Globals::hex_base))));
	this->WriteValue(two_bytes);
}
