#include "framework/test.hpp"

// Libraries

#include <iostream>

test _("hello-world", []
{
    std :: cout << "Hello World!" << std :: endl;
});
