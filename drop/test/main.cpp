#include <iostream>

#include "framework/test.h"

int main(int argc, char * argv[])
{
    auto usage = []
    {
        std :: cout << "usage: test enumerate" << std :: endl;
        std :: cout << "       test run [test_name]" << std :: endl;

        return -1;
    };

    if(argc < 2)
        return usage();

    if(strcmp(argv[1], "enumerate") && strcmp(argv[1], "run"))
        return usage();

    if(!strcmp(argv[1], "enumerate"))
    {
        std :: cout << "[";

        bool first = true;
        for(const auto & name : test :: enumerate())
            std :: cout << (first ? "" : ", ") << "\"" << name << "\"", first = false;

        std :: cout << "]" << std :: endl;
        return 0;
    }

    if(!strcmp(argv[1], "run"))
    {
        if(argc < 3)
            return usage();

        try
        {
            test :: run(argv[2]);
        }
        catch(const std :: exception & exception)
        {
            std :: cerr << "Test failed: " << exception.what() << std :: endl;
        }
        catch(const char * exception)
        {
            std :: cerr << "Test failed: " << exception << std :: endl;
        }
        catch(...)
        {
            std :: cerr << "Test failed: (unknown exception)" << std :: endl;
        }
    }
}
