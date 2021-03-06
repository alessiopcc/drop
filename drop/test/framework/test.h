// Forward declarations

class test;

#if !defined(__test__framework__test__h)
#define __test__framework__test__h

// Libraries

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <iostream>

// Classes

class test
{
    // Service nested classes

    class interface;
    template <typename> class specialization;

public:

    // Nested classes

    struct configuration;

private:

    // Static members

    static std :: unordered_map <std :: string, configuration> tests;

public:

    // Constructors

    template <typename lambda> test(const std :: string &, const lambda &);
    template <typename lambda> test(const std :: string &, configuration, const lambda &);

    // Static methods

    static void run(const std :: string &);
    static configuration configuration(const std :: string &);
    static std :: vector <std :: string> enumerate();
};

class test :: interface
{
public:

    // Destructor

    virtual ~interface();

    // Methods

    virtual void run() = 0;
};

template <typename lambda> class test :: specialization : public test :: interface
{
    // Members

    lambda _test;

public:

    // Constructors

    specialization(const lambda &);

    // Methods

    void run();
};

struct test :: configuration
{
    test :: interface * test = nullptr;
    size_t instances = 1;
};

// Ostream integration

std :: ostream & operator << (std :: ostream &, const class test :: configuration &);

// Macros

#define $$$test(counter, ...) test __test__##counter##__(__VA_ARGS__)
#define $$test(counter, ...) $$$test(counter, __VA_ARGS__)
#define $test(...) $$test(__COUNTER__, __VA_ARGS__)

#endif
