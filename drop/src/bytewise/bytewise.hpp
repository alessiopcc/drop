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

    template <typename type, size_t index> constexpr bool bytewise :: constraints :: fixedloop()
    {
        if constexpr (index < introspection :: template count <type, bytewise> ())
        {
            typedef std :: remove_reference_t <decltype(introspection :: template get <bytewise, index> (std :: declval <type &> ()))> mtype;
            return fixed <mtype> () && fixedloop <type, index + 1> ();
        }
        else
            return true;
    }

    // Constraints

    template <typename type, typename vtype> constexpr bool bytewise :: constraints :: readable()
    {
        if constexpr (introspection :: count <type, bytewise> () > 0)
            return readableloop <type, vtype, 0> ();

        if constexpr ($expression($type(const type).accept($type(reader <vtype> &))))
            return true;

        if constexpr (stltraits :: array <type> :: value)
            return readable <typename stltraits :: array <type> :: type, vtype> ();

        if constexpr (stltraits :: vector <type> :: value)
            return readable <typename stltraits :: vector <type> :: type, vtype> ();

        if constexpr (std :: is_integral <type> :: value)
            return true;

        return false;
    }

    template <typename type, typename vtype> constexpr bool bytewise :: constraints :: writable()
    {
        if constexpr (introspection :: count <type, bytewise> () > 0)
            return writableloop <type, vtype, 0> ();

        if constexpr ($expression($type(type).accept($type(writer <vtype> &))))
            return true;

        if constexpr (stltraits :: array <type> :: value)
            return writable <typename stltraits :: array <type> :: type, vtype> ();

        if constexpr (stltraits :: vector <type> :: value)
            return writable <typename stltraits :: vector <type> :: type, vtype> ();

        if constexpr (std :: is_integral <type> :: value)
            return true;

        return false;
    }

    template <typename type> constexpr bool bytewise :: constraints :: fixed()
    {
        if constexpr (introspection :: count <type, bytewise> () > 0)
            return fixedloop <type, 0> ();

        if constexpr (stltraits :: array <type> :: value)
            return fixed <typename stltraits :: array <type> :: type> ();

        if constexpr (std :: is_integral <type> :: value)
            return true;

        return false;
    }

    // Traits

    template <typename type, size_t index> constexpr size_t bytewise :: traits :: sizeloop()
    {
        if constexpr (index < introspection :: count <type, bytewise> ())
        {
            typedef std :: remove_reference_t <decltype(introspection :: template get <bytewise, index> (std :: declval <type &> ()))> mtype;
            return size <mtype> () + sizeloop <type, index + 1> ();
        }
        else
            return 0;
    }

    template <typename type> constexpr size_t bytewise :: traits :: size()
    {
        if constexpr (constraints :: fixed <type> ())
        {
            if constexpr (introspection :: count <type, bytewise> () > 0)
                return sizeloop <type, 0> ();

            if constexpr (stltraits :: array <type> :: value)
                return stltraits :: array <type> :: size * size <typename stltraits :: array <type> :: type> ();

            if constexpr (std :: is_integral <type> :: value)
                return sizeof(type);
        }
        else
            return 0;
    }
};

#endif
