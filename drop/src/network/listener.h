// Forward declarations

namespace drop
{
    class listener;
};

#if !defined(__forward__) && !defined(__src__network__listener__h)
#define __src__network__listener__h

// Libraries

#include <memory>

// Includes

#include "connection.h"
#include "tcp.h"
#include "drop/data/variant.hpp"
#include "drop/utils/parameters.h"

namespace drop
{
    class listener
    {
    public:

        // Constraints

        struct constraints
        {
            template <typename, typename...> static constexpr bool construct();
        };

    private:

        // Service nested classes

        class arc;

        // Service typedefs

        typedef variant <tcp :: listener> variant;

        // Members

        std :: shared_ptr <variant> _arc;

        // Private constructors

        listener(const std :: shared_ptr <variant> &);

    public:

        // Methods

        connection acceptsync() const;
        promise <connection> acceptasync() const;

        promise <connection> accept() const;

        void bind(pool &) const;
        void unbind() const;

        // Static methods

        template <typename type, typename... atypes, std :: enable_if_t <constraints :: construct <type, atypes...> ()> * = nullptr> static listener construct(atypes && ...);
    };
};

#endif
