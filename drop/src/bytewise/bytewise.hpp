#ifndef __src__bytewise__bytewise__hpp
#define __src__bytewise__bytewise__hpp

// Includes

#include "bytewise.h"
#include "bytewise/endianess.hpp"
#include "introspection/introspection.hpp"
#include "concept/expression.hpp"

namespace drop
{
    // bytewise

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

        if constexpr ($expression($type(const type).accept($type(bytewise :: reader <vtype> &))))
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

        if constexpr ($expression($type(type).accept($type(bytewise :: writer <vtype> &))))
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

    template <typename type> constexpr bool bytewise :: constraints :: reader()
    {
        return $expression($type(type).update($type(const uint8_t *), $type(const size_t &)));
    }

    template <typename type> constexpr bool bytewise :: constraints :: writer()
    {
        return $expression($type(type).pop($type(const size_t &))).template casts <const uint8_t *> ();
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

    // Static methods

    template <typename vtype, typename type, std :: enable_if_t <bytewise :: constraints :: readable <type, vtype> ()> *> void bytewise :: read(vtype & visitor, const type & item)
    {
        reader <vtype> reader(visitor);
        visit(reader, item);
    }

    template <typename vtype, typename type, std :: enable_if_t <bytewise :: constraints :: writable <type, vtype> ()> *> void bytewise :: write(vtype & visitor, type & item)
    {
        writer <vtype> writer(visitor);
        visit(writer, item);
    }

    // Private static methods

    template <typename vtype, typename type> void bytewise :: visit(reader <vtype> & reader, const type & item)
    {
        std :: cout << __PRETTY_FUNCTION__ << std :: endl;

        if constexpr (introspection :: count <type, bytewise> () > 0)
        {
            introspection :: visit <bytewise> (item, [&](const auto & element)
            {
                visit(reader, element);
            });

            return;
        }

        if constexpr ($expression($type(const type).accept($type(bytewise :: reader <vtype> &))))
        {
            item.accept(reader);
            return;
        }

        if constexpr (stltraits :: array <type> :: value)
        {
            typedef typename stltraits :: array <type> :: type itype;

            if constexpr (std :: is_integral <itype> :: value && ((sizeof(itype) == 1) || (endianess :: local == endianess :: network)))
                reader._visitor.update((const uint8_t *) item.data(), sizeof(itype) * item.size());
            else
                for(const itype & element : item)
                    visit(reader, element);

            return;
        }

        if constexpr (stltraits :: vector <type> :: value)
        {
            typedef typename stltraits :: vector <type> :: type itype;

            // TODO: First serialize the size of the vector. Needs varints.

            if constexpr (std :: is_integral <itype> :: value && ((sizeof(itype) == 1) || (endianess :: local == endianess :: network)))
                reader._visitor.update((const uint8_t *) item.data(), sizeof(itype) * item.size());
            else
                for(const itype & element : item)
                    visit(reader, element);

            return;
        }

        if constexpr (std :: is_integral <type> :: value)
        {
            auto swap = endianess :: translate(item);
            reader._visitor.update((const uint8_t *) &swap, sizeof(type));
        }
    }

    // reader

    // Private constructors

    template <typename vtype> bytewise :: reader <vtype> :: reader(vtype & visitor) : _visitor(visitor)
    {
    }

    // Methods

    template <typename vtype> template <typename type, std :: enable_if_t <bytewise :: constraints :: readable <type, vtype> ()> *> void bytewise :: reader <vtype> :: visit(const type & item)
    {
        bytewise :: visit(*this, item);
    }

    // writer

    // Private constructors

    template <typename vtype> bytewise :: writer <vtype> :: writer(vtype & visitor) : _visitor(visitor)
    {
    }

    // Methods

    template <typename vtype> template <typename type, std :: enable_if_t <bytewise :: constraints :: writable <type, vtype> ()> *> void bytewise :: writer <vtype> :: visit(type & item)
    {
        bytewise :: visit(*this, item);
    }
};

#endif
