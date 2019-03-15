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
#include "drop/async/promise.hpp"
#include "drop/thread/semaphore.h"
#include "drop/thread/guard.h"

namespace drop
{
    class crontab
    {
        // Settings

        struct settings
        {
            static constexpr interval tick = 1_ms;
        };

    private:

        // Service nested classes

        class event : public promise <void>, public timestamp
        {
        public:

            // Constructors

            event(const timestamp &);
        };

        class system;

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

    public:

        // Static members

        static system system;
    };

    class crontab :: system
    {
        // Friends

        friend class crontab;

        // Members

        crontab * _crontabs;
        size_t _size;

        // Private constructors

        system();

    public:

        // Destructor

        ~system();

        // Methods

        crontab & get();

    private:

        // Private static members

        static thread_local size_t roundrobin;
    };

    // Functions

    promise <void> wait(const timestamp &);
    promise <void> wait(const interval &);
};

#endif
