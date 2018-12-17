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

    class descriptor;
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
    class send_failed;
    class receive_failed;

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

// Forward includes

#define __forward__
#include "connection.h"
#undef __forward__

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

        // Static methods

        static connection connectsync(const address &);
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

    private:

        // Private constructors

        socket(const int &);

    public:

        // Getters

        address remote() const;
        template <typename> inline auto get() const;

        // Setters

        template <typename tag, typename type> inline void set(const type &) const;

        // Methods

        void bind(const class address :: port &);
        void bind(const address &);

        void listen() const;
        socket accept() const;

        void connect(const address &);

        size_t send(const uint8_t *, const size_t &) const;
        size_t receive(uint8_t *, const size_t &) const;

        void close();

    private:

        // Private methods

        void opencheck() const;

        int fcntl() const;
        void fcntl(const int &) const;

        int ioctl(const unsigned long &) const;

        template <typename type> type getsockopt(const int &, const int &) const;
        template <typename type> void setsockopt(const int &, const int &, const type &) const;

    public:

        // Casting

        operator bool () const;

        // Static methods

        static socket IPv4();
        static socket IPv6();

        static socket any();
    };
};

#endif
