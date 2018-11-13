#ifndef __src__bytewise__bytewise__hpp
#define __src__bytewise__bytewise__hpp

// Includes

#include "bytewise.h"
#include "bytewise/endianess.hpp"
#include "introspection/introspection.hpp"
#include "concept/expression.hpp"
#include "data/variant.hpp"
#include "data/varint.hpp"

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

    template <typename type> constexpr bool bytewise :: constraints :: constructible()
    {
        return std :: is_default_constructible <type> :: value || std :: is_constructible <type, bytewise> :: value;
    }

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
            return constructible <typename stltraits :: vector <type> :: type> () && writable <typename stltraits :: vector <type> :: type, vtype> ();

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

    template <typename type> constexpr bool bytewise :: constraints :: serializable()
    {
        return readable <type, bytewise :: sizer> () && readable <type, bytewise :: serializer <0>> ();
    }

    template <typename type> constexpr bool bytewise :: constraints :: deserializable()
    {
        return constructible <type> () && writable <type, bytewise :: deserializer <0>> ();
    }

    // Trait helpers

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

    // Traits

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

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: fixed <types> ())> *> std :: array <uint8_t, (... + bytewise :: traits :: size <types> ())> bytewise :: serialize(const types & ... items)
    {
        std :: array <uint8_t, (... + traits :: size <types> ())> data;

        serializer <(... + traits :: size <types> ())> serializer(data);
        (read(serializer, items), ...);

        return data;
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: serializable <types> ()) && !(... && bytewise :: constraints :: fixed <types> ())> *> std :: vector <uint8_t> bytewise :: serialize(const types & ... items)
    {
        sizer sizer;
        (read(sizer, items), ...);

        std :: vector <uint8_t> data(sizer);

        serializer <0> serializer(data);
        (read(serializer, items), ...);

        return data;
    }

    template <typename type, std :: enable_if_t <bytewise :: constraints :: fixed <type> ()> *> type bytewise :: deserialize(const std :: array <uint8_t, bytewise :: traits :: size <type> ()> & data)
    {
        auto execute = [&](const auto & ... constrargs)
        {
            type item(constrargs...);

            deserializer <traits :: size <type> ()> deserializer(data);
            write(deserializer, item);

            return item;
        };

        if constexpr (std :: is_constructible <type, bytewise> :: value)
            return execute(bytewise{});
        else
            return execute();
    }

    template <typename type, std :: enable_if_t <bytewise :: constraints :: deserializable <type> () && !(bytewise :: constraints :: fixed <type> ())> *> type bytewise :: deserialize(const std :: vector <uint8_t> & data)
    {
        auto execute = [&](const auto & ... constrargs)
        {
            type item(constrargs...);

            deserializer <0> deserializer(data);
            write(deserializer, item);

            return item;
        };

        if constexpr (std :: is_constructible <type, bytewise> :: value)
            return execute(bytewise{});
        else
            return execute();
    }

    // Private static methods

    template <typename vtype, typename type> void bytewise :: visit(reader <vtype> & reader, const type & item)
    {
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

            visit(reader, varint(item.size()));

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

    template <typename vtype, typename type> void bytewise :: visit(writer <vtype> & writer, type & item)
    {
        if constexpr (introspection :: count <type, bytewise> () > 0)
        {
            introspection :: visit <bytewise> (item, [&](auto & element)
            {
                visit(writer, element);
            });

            return;
        }

        if constexpr ($expression($type(type).accept($type(bytewise :: writer <vtype> &))))
        {
            item.accept(writer);
            return;
        }

        if constexpr (stltraits :: array <type> :: value)
        {
            typedef typename stltraits :: array <type> :: type itype;

            if constexpr (std :: is_integral <itype> :: value && ((sizeof(itype) == 1) || (endianess :: local == endianess :: network)))
                memcpy((uint8_t *) item.data(), writer._visitor.pop(sizeof(itype) * item.size()), sizeof(itype) * item.size());
            else
                for(itype & element : item)
                    visit(writer, element);

            return;
        }

        if constexpr (stltraits :: vector <type> :: value)
        {
            typedef typename stltraits :: vector <type> :: type itype;

            varint size;
            visit(writer, size);

            if constexpr (std :: is_constructible <itype, bytewise> :: value)
                item.resize(size, bytewise());
            else
                item.resize(size);

            if constexpr (std :: is_integral <itype> :: value && ((sizeof(itype) == 1) || (endianess :: local == endianess :: network)))
                memcpy((const uint8_t *) item.data(), writer._visitor.pop(sizeof(itype) * size), sizeof(itype) * size);
            else
                for(itype & element : item)
                    visit(writer, element);

            return;
        }

        if constexpr (std :: is_integral <type> :: value)
            item = endianess :: translate(*((const type *) writer._visitor.pop(sizeof(type))));
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

    // serializer

    // Private constructors

    template <size_t size> bytewise :: serializer <size> :: serializer(dtype & data) : _data(data), _cursor(0)
    {
    }

    // Methods

    template <size_t size> void bytewise :: serializer <size> :: update(const uint8_t * chunk, const size_t & csize)
    {
        memcpy(this->_data.data() + this->_cursor, chunk, csize);
        this->_cursor += csize;
    }

    // deserializer

    // Private constructors

    template <size_t size> bytewise :: deserializer <size> :: deserializer(const dtype & data) : _data(data), _cursor(0)
    {
    }

    // Methods

    template <size_t size> const uint8_t * bytewise :: deserializer <size> :: pop(const size_t & csize)
    {
        size_t cursor = this->_cursor;
        this->_cursor += csize;

        if constexpr (size == 0)
        {
            if(this->_cursor > this->_data.size())
                exception <buffer_error, out_of_range> :: raise(this);
        }

        return (this->_data.data() + cursor);
    }
};

// Ostream integration

template <size_t size> std :: ostream & operator << (std :: ostream & out, const std :: array <uint8_t, size> & data)
{
    out << std :: hex << "<";

    for(size_t i = 0; i < size; i++)
        out << std :: setw(2) << std :: setfill('0') << (unsigned int)(data[i]);

    out << ">" << std :: dec;

    return out;
}

#endif
