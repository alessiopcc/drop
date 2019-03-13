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
    class reuse;

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
    class listener_locked;

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
#include "pool.h"
#include "async/promise.h"
#include "listener.h"
#undef __forward__

// Includes

#include "address.hpp"
#include "chrono/time.hpp"
#include "thread/guard.hpp"

namespace drop
{
    class tcp
    {
        // Friends

        friend class drop :: listener;

    public:

        // Nested classes

        class socket;

    private:

        // Service nested classes

        class listener;

    public:

        // Static methods

        static connection connectsync(const address &);

        static promise <connection> connectasync(const address &);
        static promise <connection> connectasync(const address &, pool &);

        static promise <connection> connect(const address &);
        static promise <connection> connect(const address &, pool &);

        static drop :: listener listen(const class address :: port &);
        static drop :: listener listen(const address &);
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
        int _type;

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

    class tcp :: listener
    {
        // Members

        socket _socket;

        struct
        {
            bool blocking;
        } _cache;

        bool _lock;
        pool * _pool;

        guard <recursive> _guard;

    public:

        // Constructors

        listener(const class address :: port &);
        listener(const address &);

        // Destructor

        ~listener();

        // Methods

        connection acceptsync();
        promise <connection> acceptasync();

        void bind(pool &);
        void unbind();

    private:

        // Private methods

        template <bool> void block();

        template <bool> void setup();
        void release();
    };
};

#endif
