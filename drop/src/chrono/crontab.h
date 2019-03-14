// Forward declarations

namespace drop
{
    class crontab;
};

#if !defined(__forward__) && !defined(__src__chrono__crontab__h)
#define __src__chrono__crontab__h

// Libraries

#include <thread>
#include <queue>

// Includes

#include "time.hpp"
#include "async/promise.hpp"
#include "thread/semaphore.h"
#include "thread/guard.h"

namespace drop
{
    class crontab
    {
        // Settings

        struct settings
        {
            static constexpr interval tick = 1_ms;
        };

        // Service nested classes

        class event : public promise <void>, public timestamp
        {
        public:

            // Constructors

            event(const timestamp &);
        };

        // Members

        std :: priority_queue <event, std :: vector <event>, std :: greater <event>> _events;
        std :: vector <event> _pending;

        timestamp _waketime;

        semaphore _semaphore;
        guard <simple> _guard;

        volatile bool _alive;
        std :: thread _thread;

    public:

        // Constructors

        crontab();

        // Destructor

        ~crontab();

        // Methods

        promise <void> wait(const timestamp &);
        promise <void> wait(const interval &);

    private:

        // Private methods

        void update(const timestamp &);
        void flush(const timestamp &);
        void sleep(const timestamp &);

        void run();
    };
};

#endif
