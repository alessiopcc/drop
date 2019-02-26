// Forward declarations

namespace drop
{
    // Tags

    class recursive;
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
    template <> class guard <recursive>
    {
        // Service nested classes

        class shield;

        // Members

        std :: recursive_mutex _mutex;

    public:

        // Operators

        template <typename lambda> auto operator () (lambda &&);
    };

    class guard <recursive> :: shield
    {
        // Members

        std :: recursive_mutex & _mutex;

    public:

        // Constructors

        shield(std :: recursive_mutex &);

        // Destructor

        ~shield();
    };
};

#endif
