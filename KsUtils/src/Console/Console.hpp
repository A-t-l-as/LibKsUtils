#ifndef CONSOLE_H
#define CONSOLE_H

#include "MessPrinter.hpp"
#include "MessScanner.hpp"

#ifdef _DEBUG
    #define DEBUG_PRINT(...) Console::Get().Out().Print(__VA_ARGS__)
#else
    #define DEBUG_PRINT(...)
#endif

//#define DEBUG_PRINT(...) Console::Get().Out().Print(__VA_ARGS__)

#define CONSOLE_IN Console::Get().In()
#define CONSOLE_OUT Console::Get().Out()
#define CONSOLE_ERR Console::Get().Err()

class Console
{
public:
	static Console& Get();
	
	MessScanner In();
	MessPrinter Out();
	MessPrinter Err();

private:
	Console() {};
	~Console()
	{
		delete m_pInstance;
	}

	static Console* m_pInstance;

};


#endif // !CONSOLE_H
