// Forward declarations

namespace drop
{
    class semaphore;
};

#if !defined(__forward__) && !defined(__src__thread__semaphore__h)
#define __src__thread__semaphore__h

// Libraries

#include <stddef.h>
#include <time.h>

#ifdef __APPLE__
#include <dispatch/dispatch.h>
#endif

#ifdef __linux__
#include <semaphore.h>
#endif

// Includes

#include "chrono/time.hpp"

namespace drop
{
    class semaphore
    {
        // Members

        #ifdef __APPLE__
        dispatch_semaphore_t _semaphore;
        #endif

        #ifdef __linux__
        sem_t _semaphore;
        #endif

    public:

        // Constructors

        semaphore();

        // Destructor

        ~semaphore();

        // Methods

        bool wait();
        bool wait(const timestamp &);
        bool wait(const interval &);

        void post();
    };
};

#endif
