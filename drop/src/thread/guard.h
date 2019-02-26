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

// Includes

#include "shield.h"

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
};

#endif
