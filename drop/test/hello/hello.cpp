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

$test("break-me", []
{
    throw "Something went wrong, as expected!";
});

$test("many-instances", {.instances = 32}, []
{
    std :: cout << "Soooo many instances!" << std :: endl;
});
