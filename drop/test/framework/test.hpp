#ifndef __test__framework__test__hpp
#define __test__framework__test__hpp

// Includes

#include "test.h"

// test

// Constructors

template <typename lambda> test :: test(const std :: string & name, const lambda & test)
{
    tests[name] = {.test = new specialization <lambda> (test)};
}

template <typename lambda> test :: test(const std :: string & name, struct configuration configuration, const lambda & test)
{
    configuration.test = new specialization <lambda> (test);
    tests[name] = configuration;
}

// specialization

// Constructors

template <typename lambda> test :: specialization <lambda> :: specialization(const lambda & test) : _test(test)
{
}

// Methods

template <typename lambda> void test :: specialization <lambda> :: run()
{
    this->_test();
}

#endif
