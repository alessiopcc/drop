// Forward declarations

namespace drop
{
    class varint;
};

#if !defined(__forward__) && !defined(__src__data__varint__h)
#define __src__data__varint__h

// Libraries

#include <stdint.h>

// Includes

#include "bytewise/bytewise.h"

namespace drop
{
    class varint
    {
        // Members

        uint32_t _value;

    public:

        // Constructors

        varint() = default;
        varint(const uint32_t &);

        // Methods

        template <typename vtype> void accept(bytewise :: reader <vtype> &) const;
        template <typename vtype> void accept(bytewise :: writer <vtype> &);

        // Operators

        varint & operator = (const uint32_t &);

        // Casting

        operator uint32_t & ();
        operator const uint32_t & () const;
    };
};

#endif
