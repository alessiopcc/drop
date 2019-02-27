// Forward declarations

namespace drop
{
    // Tags

    class simple;
    class recursive;
    class sequential;

    // Classes

    template <typename> class guard;
};

#if !defined(__forward__) && !defined(__src__thread__guard__h)
#define __src__thread__guard__h

// Libraries

#include <mutex>
#include <deque>

namespace drop
{
    template <typename type> class guard
    {
        // Asserts

        static_assert(std :: is_same <type, simple> :: value || std :: is_same <type, recursive> :: value, "Valid tags for a `guard` are: `simple`, `recursive`, `sequential`.");

        // Members

        std :: conditional_t <std :: is_same <type, simple> :: value, std :: mutex, std :: recursive_mutex> _mutex;

    public:

        // Operators

        template <typename lambda> auto operator () (lambda &&);
    };

    template <> class guard <sequential>
    {
        // Members

        std :: recursive_mutex _mutex;
        bool _lock;
        std :: deque <std :: function <void()>> _queue;

    public:

        // Contructors

        guard();

        // Operators

        template <typename lambda> void operator () (lambda &&);
    };
};

#endif
