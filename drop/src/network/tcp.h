// Forward declarations

namespace drop
{
    // Tags

    namespace timeouts
    {
        class send;
        class receive;
    };

    class blocking;

    class bad_access;
    class socket_closed;
    class fcntl_failed;
    class getsockopt_failed;
    class setsockopt_failed;

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

        template <typename> inline auto get() const;

        // Setters

        template <typename tag, typename type> inline void set(const type &);

    private:

        // Private methods

        void opencheck() const;

        int fcntl() const;
        void fcntl(const int &);

        template <typename type> type getsockopt(const int &, const int &) const;
        template <typename type> void setsockopt(const int &, const int &, const type &);
    };
};

#endif
