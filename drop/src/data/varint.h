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

#include "drop/bytewise/bytewise.h"

namespace drop
{
    class varint
    {
    public:

        // Constraints

        struct constraints
        {
            template <typename> static constexpr bool visitor();
        };

    private:
        // Members

        uint32_t _value;

    public:

        // Constructors

        varint() = default;
        varint(const uint32_t &);

        varint(const uint8_t *);

        // Getters

        size_t size() const;

        // Bytewise

        template <typename vtype> void accept(bytewise :: reader <vtype> &) const;
        template <typename vtype> void accept(bytewise :: writer <vtype> &);

        // Methods

        template <typename lambda, std :: enable_if_t <constraints :: visitor <lambda> ()> * = nullptr> void visit(lambda &&) const;

        // Operators

        varint & operator = (const uint32_t &);

        // Casting

        operator uint32_t & ();
        operator const uint32_t & () const;

        // Static methods

        static size_t size(const uint8_t &);
    };
};

#endif
