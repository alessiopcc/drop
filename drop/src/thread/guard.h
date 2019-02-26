// Forward declarations

namespace drop
{
    // Tags

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
    template <> class guard <recursive>
    {
        // Members

        std :: recursive_mutex _mutex;

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

        template <typename lambda> auto operator () (lambda &&);
    };
};

#endif
