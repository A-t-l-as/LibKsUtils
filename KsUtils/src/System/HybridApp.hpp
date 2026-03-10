#ifndef HYBRID_APP_HPP
#define HYBRID_APP_HPP

#include "../Console/Console.hpp"
#include "IApp.hpp"
#include "Utils/Misc.hpp"
#include <string>
#include <vector>

class HybridApp : public IApp
{
public:

    HybridApp()
    {}

    void Run(int my_argc, char* my_argv[]) override
    {
        std::string flag;

        if(my_argc < 2)
        {
            StandardProgramExec(flag);
        }
        else
        {
            flag = my_argv[1];
        }

        if(flag == "-e")
        {
            RunExporter(my_argc, my_argv);
        }

        if(flag == "-c")
        {
            RunImporter(my_argc, my_argv);
        }

        if(flag != "-c" && flag != "-e")
        {
            CONSOLE_OUT.PrintLn("Invalid input flag. Available flags:", Mess::endl,
                                "-e   -   export mode", Mess::endl,
                                "-c   -   compile/import mode");
            exit(EXIT_FAILURE);
        }
    }

protected:

    virtual void ShowStartScreen() = 0;
    virtual void RunExporter(int my_argc, char* my_argv[]) = 0;
    virtual void RunImporter(int my_argc, char* my_argv[]) = 0;

private:

    void StandardProgramExec(std::string& flag)
    {
        ShowStartScreen();

        const std::vector<std::string> correct_modes = { "1", "2" };
        CONSOLE_OUT.PrintLn("Choose mode:", Mess::endl,
                            "1 - Exporter,", Mess::endl,
                            "2 - Compilator/Importer");

        std::string mode;

        do
        {
            CONSOLE_IN.Scan(mode);

            if (!Misc::CheckAnswerWithEveryElementInVec(mode, correct_modes))
            {
                CONSOLE_OUT.PrintLn("Choose from the given modes...");
            }

        } while (!Misc::CheckAnswerWithEveryElementInVec(mode, correct_modes));

        if(mode == "1") flag = "-e";

        if(mode == "2") flag = "-c";
    }


};



#endif // !HYBRID_APP_HPP
