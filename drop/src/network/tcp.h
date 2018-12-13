// Forward declarations

namespace drop
{
    // Tags

    namespace timeouts
    {
        class send;
        class receive;
    };

    namespace buffers
    {
        namespace send
        {
            class size;
            class pending;
            class available;
        };

        namespace receive
        {
            class size;
            class available;
        }
    }

    class blocking;
    class readable;
    class writable;

    class bad_access;
    class socket_closed;
    class socket_open;
    class fcntl_failed;
    class getsockopt_failed;
    class setsockopt_failed;
    class bind_failed;
    class listen_failed;
    class accept_failed;
    class connect_failed;
    class getpeername_failed;
    class ioctl_failed;

    // Classes

    class tcp;
};

#if !defined(__forward__) && !defined(__src__network__tcp__h)
#define __src__network__tcp__h

// Libraries

#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>

// Includes

#include "network/address.hpp"
#include "chrono/time.hpp"

namespace drop
{
    class tcp
    {
    public:

        // Nested classes

        class socket;
    };

    class tcp :: socket
    {
    public:

        // Settings

        struct settings
        {
            struct listen
            {
                static constexpr size_t slots = 128;
            };
        };

    private:

        // Members

        int _descriptor;

    public:

        // Constructors

        socket();

    private:

        // Private constructors

        socket(const int &);

    public:

        // Getters

        address remote() const;
        template <typename> inline auto get() const;

        // Setters

        template <typename tag, typename type> inline void set(const type &);

        // Methods

        void bind(const class address :: port &);
        void bind(const address &);

        void listen();
        socket accept();

        void connect(const address &);

    private:

        // Private methods

        void opencheck() const;
        void closedcheck() const;

        int fcntl() const;
        void fcntl(const int &);

        int ioctl(const unsigned long &) const;

        template <typename type> type getsockopt(const int &, const int &) const;
        template <typename type> void setsockopt(const int &, const int &, const type &);

        void bind(const address &, const bool &);

    public:

        // Casting

        operator bool () const;
    };
};

#endif
