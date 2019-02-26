// Forward declarations

namespace drop
{
    class shield;
};

#if !defined(__forward__) && !defined(__src__thread__shield__h)
#define __src__thread__shield__h

// Libraries

#include <mutex>

namespace drop
{
    class shield
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
