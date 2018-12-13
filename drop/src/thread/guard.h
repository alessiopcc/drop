// Forward declarations

namespace drop
{
    // Tags

    class soft;
    class hard;

    // Classes

    template <typename> class guard;
};

#if !defined(__forward__) && !defined(__src__thread__guard__h)
#define __src__thread__guard__h

// Libraries

#include <mutex>

namespace drop
{
    template <> class guard <hard>
    {
        // Service nested classes

        class shield;

        // Members

        std :: mutex _mutex;

    public:

        // Operators

        template <typename lambda> auto operator () (lambda &&);
    };

    class guard <hard> :: shield
    {
        // Members

        std :: mutex & _mutex;

    public:

        // Constructors

        shield(std :: mutex &);

        // Destructor

        ~shield();
    };
};

#endif
