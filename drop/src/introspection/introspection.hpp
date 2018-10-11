#ifndef __src__introspection__introspection__hpp
#define __src__introspection__introspection__hpp

// Includes

#include "introspection.h"

namespace drop
{
    // introspection

    // Private static methods

    template <template <typename, size_t, std :: nullptr_t> typename progressive, typename tag, ssize_t shuffle, size_t index> constexpr size_t introspection :: countloop()
    {
        if constexpr ((exists <progressive, tag, index, shuffle> ()))
            return introspection :: countloop <progressive, tag, shuffle, index + 1> ();
        else
            return index;
    }

    template <typename tag, typename type, typename lambda, size_t index> void inline introspection :: visitloop(type & instance, const lambda & callback)
    {
        if constexpr (index < introspection :: count <type :: template __tag__, tag, -1> ())
        {
            callback(introspection :: get <tag, index, type> (instance));
            introspection :: visitloop <tag, type, lambda, index + 1> (instance, callback);
        }
    }

    template <typename tag, typename type, typename lambda, size_t index> void inline introspection :: visitloop(const type & instance, const lambda & callback)
    {
        if constexpr (index < introspection :: count <type :: template __tag__, tag, -1> ())
        {
            callback(introspection :: get <tag, index, type> (instance));
            introspection :: visitloop <tag, type, lambda, index + 1> (instance, callback);
        }
    }

    // Static methods

    template <template <typename, size_t, std :: nullptr_t> typename progressive, typename tag, size_t index, ssize_t shuffle> constexpr bool introspection :: exists()
    {
        return sfinae :: template exists <progressive, tag, index, shuffle> :: value;
    }

    template <template <typename, size_t, std :: nullptr_t> typename progressive, typename tag, ssize_t shuffle> constexpr size_t introspection :: count()
    {
        return countloop <progressive, tag, shuffle, 0> ();
    }

    template <typename tag, size_t index, typename type, std :: enable_if_t <introspection :: exists <type :: template __tag__, tag, index, -1> ()> *> inline auto & introspection :: get(type & instance)
    {
        return type :: template __tag__ <tag, index, nullptr> :: get(instance);
    }

    template <typename tag, size_t index, typename type, std :: enable_if_t <introspection :: exists <type :: template __tag__, tag, index, -1> ()> *> inline const auto & introspection :: get(const type & instance)
    {
        return type :: template __tag__ <tag, index, nullptr> :: get(instance);
    }

    template <typename tag, typename type, typename lambda> inline void introspection :: visit(type & instance, const lambda & callback)
    {
        introspection :: visitloop <tag, type, lambda, 0> (instance, callback);
    }

    template <typename tag, typename type, typename lambda> inline void introspection :: visit(const type & instance, const lambda & callback)
    {
        introspection :: visitloop <tag, type, lambda, 0> (instance, callback);
    }
};

#endif
