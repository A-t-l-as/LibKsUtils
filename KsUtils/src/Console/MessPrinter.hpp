#ifndef MESS_PRINTER_H
#define MESS_PRINTER_H

#include <ostream>
#include <sstream>

namespace Mess
{
	static constexpr char endl[] = "\n";
};

class MessPrinter
{
public:
	

	MessPrinter(std::ostream& arg_o) :
		m_o(arg_o)
	{
	}

	~MessPrinter()
	{
	}

	template <typename T>
	void Print(const T& value)
	{
		this->m_o << value;
	}

	template<typename T, typename... Args>
	void Print(const T& first, const Args&... rest)
	{
		this->m_o << first;
		Print(rest...);
	}

	template <typename T>
	void PrintLn(const T& value)
	{
		this->m_o << value << std::endl;
	}

	template<typename... Args>
	void PrintLn(const Args&... args)
	{
		Print(args...);
		this->m_o << std::endl;
	}

    inline void PrintSs(const std::stringstream& message)
	{
		this->m_o << message.str();
	}

    inline void PrintSsLn(const std::stringstream& message)
	{
		this->m_o << message.str() << std::endl;
	}

private:
	std::ostream& m_o;

};


#endif // !MESS_PRINTER_H
