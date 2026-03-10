#include "Console.hpp"

#include <iostream>

using namespace std;

Console* Console::m_pInstance = new Console();

MessScanner Console::In()
{
	return MessScanner(cin);
}

MessPrinter Console::Out()
{
	return MessPrinter(cout);
}

MessPrinter Console::Err()
{
	return MessPrinter(cerr);
}

Console& Console::Get()
{
	return *m_pInstance;
}