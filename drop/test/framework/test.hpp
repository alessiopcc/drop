#ifndef __test__framework__test__hpp
#define __test__framework__test__hpp

// Includes

#include "test.h"

// test

// Constructors

template <typename lambda> test :: test(const std :: string & name, const lambda & test)
{
    tests[name] = new specialization <lambda> (test);
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
