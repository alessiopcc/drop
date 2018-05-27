#include "framework/test.hpp"

// Libraries

#include <iostream>

$test("hello-world", []
{
    std :: cout << "Hello World!" << std :: endl;
});

$test("hello-again", []
{
    std :: cout << "Hello again, World!" << std :: endl;
});
