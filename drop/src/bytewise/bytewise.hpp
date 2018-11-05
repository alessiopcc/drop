#ifndef __src__bytewise__bytewise__hpp
#define __src__bytewise__bytewise__hpp

// Includes

#include "bytewise.h"
#include "bytewise/endianess.hpp"
#include "introspection/introspection.hpp"
#include "concept/expression.hpp"

namespace drop
{
    // Constraint helpers

    template <typename type, typename vtype, size_t index> constexpr bool bytewise :: constraints :: readableloop()
    {
        if constexpr (index < introspection :: template count <type, bytewise> ())
        {
            typedef std :: remove_reference_t <decltype(introspection :: template get <bytewise, index> (std :: declval <type &> ()))> mtype;
            return readable <mtype, vtype> () && readableloop <type, vtype, index + 1> ();
        }
        else
            return true;
    }

    template <typename type, typename vtype, size_t index> constexpr bool bytewise :: constraints :: writableloop()
    {
        if constexpr (index < introspection :: template count <type, bytewise> ())
        {
            typedef std :: remove_reference_t <decltype(introspection :: template get <bytewise, index> (std :: declval <type &> ()))> mtype;
            return writable <mtype, vtype> () && writableloop <type, vtype, index + 1> ();
        }
        else
            return true;
    }

    // Constraints

    template <typename type, typename vtype> constexpr bool bytewise :: constraints :: readable()
    {
        if constexpr ($expression($type(const type).accept($type(reader <vtype> &))))
            return true;

        if constexpr (introspection :: count <type, bytewise> () > 0)
            return readableloop <type, vtype, 0> ();

        if constexpr (std :: is_integral <type> :: value)
            return true;

        return false;
    }

    template <typename type, typename vtype> constexpr bool bytewise :: constraints :: writable()
    {
        if constexpr ($expression($type(type).accept($type(writer <vtype> &))))
            return true;

        if constexpr (introspection :: count <type, bytewise> () > 0)
            return writableloop <type, vtype, 0> ();

        if constexpr (std :: is_integral <type> :: value)
            return true;

        return false;
    }
};

#endif
