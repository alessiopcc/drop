#include <iostream>

#include "framework/test.h"

int main()
{
    for(const auto & name : test :: enumerate())
        std :: cout << name << std :: endl;
}
