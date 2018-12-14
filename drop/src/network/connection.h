// Forward declarations

namespace drop
{
    // Tags

    class send_failed;
    class send_locked;
    class receive_failed;
    class receive_locked;
    class synchrony_locked;

    class send_timeout;
    class receive_timeout;

    class send;
    class receive;

    // Classes

    class connection;
};

#if !defined(__forward__) && !defined(__src__network__connection__h)
#define __src__network__connection__h

// Libraries

#include <memory>

// Includes

#include "tcp.hpp"
#include "streamer.hpp"
#include "bytewise/bytewise.hpp"
#include "data/variant.hpp"
#include "concept/stltraits.h"
#include "thread/guard.hpp"

namespace drop
{
    class connection
    {
        // Constraints

        struct constraints
        {
            template <typename> static constexpr bool buffer();
        };

        // Service nested classes

        class arc;

        // Members

        std :: shared_ptr <arc> _arc;

    public:

        // Constructors

        connection(const tcp :: socket &);

        // Methods

        template <typename type, std :: enable_if_t <constraints :: buffer <type> ()> * = nullptr> void sendsync(const type &) const;
        template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: serializable <types> ()) && !((sizeof...(types) == 1) && (... && constraints :: buffer <types> ()))> * = nullptr> void sendsync(const types & ...) const;

        template <typename type, std :: enable_if_t <constraints :: buffer <type> ()> * = nullptr> type receivesync() const;
        template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: deserializable <types> ()) && !((sizeof...(types) == 1) && (... && constraints :: buffer <types> ()))> * = nullptr> auto receivesync() const;

    private:

        // Private methods

        template <bool> void block() const;

        template <typename, bool> void setup() const;
        template <typename> void release() const;
    };

    class connection :: arc
    {
        // Friends

        friend class connection;

        // Members

        variant <tcp :: socket> _socket;

        struct
        {
            bool blocking;
        } _cache;

        struct
        {
            bool send;
            bool receive;
        } _locks;

        guard <soft> _guard;

    public:

        // Destructors

        ~arc();
    };
};

#endif
