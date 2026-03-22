#include "Console.hpp"

#include <iostream>

using namespace std;

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

