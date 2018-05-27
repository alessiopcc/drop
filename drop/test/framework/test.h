// Forward declarations

class test;

#if !defined(__test__framework__test__h)
#define __test__framework__test__h

// Libraries

#include <unordered_map>
#include <string>
#include <functional>

// Classes

class test
{
    // Service nested classes

    class interface;
    template <typename> class specialization;

    // Static members

    static std :: unordered_map <std :: string, interface *> tests;

public:

    // Constructors

    template <typename lambda> test(const std :: string &, const lambda &);

    // Static methods

    static void run(const std :: string &);
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

// Macros

#define $$$test(counter, ...) test __test__##counter##__(__VA_ARGS__)
#define $$test(counter, ...) $$$test(counter, __VA_ARGS__)
#define $test(...) $$test(__COUNTER__, __VA_ARGS__)

#endif
