// Forward declarations

namespace drop
{
    // Tags

    class unknown_event;
    class kevent_failed;
    class epoll_ctl_failed;

    // Classes

    class queue;
};

#if !defined(__forward__) && !defined(__src__network__queue__h)
#define __src__network__queue__h

// Libraries

#ifdef __APPLE__
#include <sys/event.h>
#endif

#ifdef __linux__
#include <sys/epoll.h>
#endif

// Includes

#include "chrono/time.hpp"

namespace drop
{
    class queue
    {
    public:

        // Settings

        struct settings
        {
            static constexpr size_t buffer = 1024;
        };

        // Nested enums

        enum type {write, read};

    private:

        // Private typedefs

        #ifdef __APPLE__
        typedef struct kevent sysevent;
        #endif

        #ifdef __linux__
        typedef epoll_event sysevent;
        #endif

    public:

        // Nested classes

        class event : public sysevent
        {
        public:

            // Getters

            int descriptor() const;
            type type() const;
            bool error() const;
        };

    private:

        // Members

        int _descriptor;
        std :: array <event, settings :: buffer> _events;

    public:

        // Constructors

        queue();

        // Destructor

        ~queue();

        // Methods

        void add(const type &, const int &);
        void remove(const type &, const int &);

    private:

        // Private methods

        #ifdef __APPLE__
        void kevent(const type &, const int &, const uint16_t &);
        #endif

        #ifdef __linux__
        void epoll_ctl(const type &, const int &, const int &);
        #endif
    };
};

#endif
