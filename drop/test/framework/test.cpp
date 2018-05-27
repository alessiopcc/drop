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

std :: vector <std :: string> test :: enumerate()
{
    std :: vector <std :: string> names;

    for(auto const & [name, test] : tests)
        names.push_back(name);

    std :: sort(names.begin(), names.end());

    return names;
}

// interface

// Destructor

test :: interface :: ~interface()
{
}
