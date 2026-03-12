#ifndef MY_GUID_HPP
#define MY_GUID_HPP

#include "File/BinFile.hpp"
#include <string>
#include <sstream>
#include <iomanip>

#include "GuidUtils/CrossGuidStruct.hpp"

#ifdef _WIN32
    #include "GuidUtils/Win32GuidStruct.hpp"
    typedef CrossGuidStruct<Win32GuidStruct>    GuidStruct;
#else
    #include "GuidUtils/MyGuidStruct.hpp"
    typedef CrossGuidStruct<MyGuidStruct>       GuidStruct;
#endif


class MyGuid
{
public:

    MyGuid()
    {
        GuidStruct::GenerateGuid(this->m_guid);
    }

    std::string ToString() const
    {
        std::stringstream ss;
        ss  << std::uppercase << std::hex << std::setfill('0')
            << std::setw(8) << m_guid.Data1 << "-"
            << std::setw(4) << m_guid.Data2 << "-"
            << std::setw(4) << m_guid.Data3 << "-"
            << std::setw(2) << static_cast<int>(m_guid.Data4[0])
            << std::setw(2) << static_cast<int>(m_guid.Data4[1]) << "-"
            << std::setw(2) << static_cast<int>(m_guid.Data4[2])
            << std::setw(2) << static_cast<int>(m_guid.Data4[3])
            << std::setw(2) << static_cast<int>(m_guid.Data4[4])
            << std::setw(2) << static_cast<int>(m_guid.Data4[5])
            << std::setw(2) << static_cast<int>(m_guid.Data4[6])
            << std::setw(2) << static_cast<int>(m_guid.Data4[7]);

        return  "{" + ss.str() + "}";
    }

    void WriteTo(BinFile& buff)
    {
        buff.WriteValue(m_guid);
    }

private:
    GuidStruct m_guid;


};




#endif // !MY_GUID_HPP
