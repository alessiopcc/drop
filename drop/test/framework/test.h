// Forward declarations

class test;

#if !defined(__test__framework__test__h)
#define __test__framework__test__h

// Libraries

#include <unordered_map>
#include <string>
#include <functional>

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

#endif
