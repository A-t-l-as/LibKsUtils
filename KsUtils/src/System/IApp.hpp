#ifndef IAPP_HPP
#define IAPP_HPP

class IApp
{
public:
    virtual ~IApp() = default;

    virtual int Run(int my_argc, char* my_argv[]) = 0;

};


#endif // IAPP_HPP
