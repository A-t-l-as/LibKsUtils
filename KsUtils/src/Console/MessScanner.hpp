#ifndef MESS_SCANNER_H
#define MESS_SCANNER_H

#include <istream>

class MessScanner
{
public:
    MessScanner(std::istream& arg_in) :
		m_in(arg_in)
	{
	}

	~MessScanner()
	{
	}

	template <typename T>
	void Scan(T& value)
	{
		this->m_in >> value;
	}

private:
	std::istream& m_in;

};


#endif // !MESS_SCANNER_H
