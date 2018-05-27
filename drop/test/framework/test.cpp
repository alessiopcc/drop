// Includes

#include "test.hpp"

// test

// Static members

std :: unordered_map <std :: string, test :: interface *> test :: tests;

// Static methods

void test :: run(const std :: string & name)
{
    interface * test = tests.at(name);
    test->run();
}

// interface

// Destructor

test :: interface :: ~interface()
{
}
