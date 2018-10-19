#ifndef __src__data__variant__hpp
#define __src__data__variant__hpp

// Includes

#include "variant.h"

namespace drop
{
    // Constraints

    template <typename... types> template <typename needle, typename... haystack> constexpr bool variant <types...> :: constraints :: distinct()
    {
        if constexpr (sizeof...(haystack) == 0)
            return true;
        else
            return !(... || std :: is_same <needle, haystack> :: value) && distinct <haystack...> ();
    }

    template <typename... types> template <typename type> constexpr bool variant <types...> :: constraints :: defined()
    {
        return (... || std :: is_same <type, types> :: value);
    }

    template <typename... types> template <typename type> constexpr bool variant <types...> :: constraints :: variant()
    {
        return (std :: is_same <type, undefined> :: value) || defined <type> ();
    }

    template <typename... types> template <typename type> constexpr bool variant <types...> :: constraints :: copyable()
    {
        return defined <type> () && std :: is_copy_constructible <type> :: value;
    }

    template <typename... types> template <typename type> constexpr bool variant <types...> :: constraints :: movable()
    {
        return defined <type> () && std :: is_move_constructible <type> :: value;
    }

    template <typename... types> template <typename lambda> constexpr bool variant <types...> :: constraints :: visitor()
    {
        return (... && $expression($type(lambda)($type(types))));
    }

    // Constructors

    template <typename... types> variant <types...> :: variant() : _typeid(0)
    {
    }

    template <typename... types> template <typename type, std :: enable_if_t <variant <types...> :: constraints :: template copyable <type> ()> *> variant <types...> :: variant(const type & value) : _typeid(index <type, types...> ())
    {
        new (&(this->_value)) type(value);
    }

    template <typename... types> template <typename type, std :: enable_if_t <variant <types...> :: constraints :: template movable <type> ()> *> variant <types...> :: variant(type && value) : _typeid(index <type, types...> ())
    {
        new (&(this->_value)) type(std :: move(value));
    }

    // Getters

    template <typename... types> template <typename type, std :: enable_if_t <variant <types...> :: constraints :: template defined <type> ()> *> type & variant <types...> :: reinterpret()
    {
        return reinterpret_cast <type &> (this->_value);
    }

    template <typename... types> template <typename type, std :: enable_if_t <variant <types...> :: constraints :: template defined <type> ()> *> const type & variant <types...> :: reinterpret() const
    {
        return reinterpret_cast <const type &> (this->_value);
    }

    // Methods

    template <typename... types> template <typename type, std :: enable_if_t <variant <types...> :: constraints :: template variant <type> ()> *> bool variant <types...> :: is() const
    {
        if constexpr (std :: is_same <type, undefined> :: value)
            return (this->_typeid == 0);
        else
            return (this->_typeid == index <type, types...> ());
    }

    template <typename... types> template <typename lambda, std :: enable_if_t <variant <types...> :: constraints :: template visitor <lambda> ()> *> void variant <types...> :: visit(lambda && callback)
    {
        this->visitloop <types...> (callback);
    }

    template <typename... types> template <typename lambda, std :: enable_if_t <variant <types...> :: constraints :: template visitor <lambda> ()> *> void variant <types...> :: visit(lambda && callback) const
    {
        this->visitloop <types...> (callback);
    }

    // Private methods

    template <typename... types> template <typename type, typename... tail, typename lambda> void variant <types...> :: visitloop(lambda && callback)
    {
        if(index <type, types...> () == this->_typeid)
        {
            callback(this->reinterpret <type> ());
            return;
        }

        if constexpr (sizeof...(tail) > 0)
            this->visitloop <tail...> (callback);
    }

    template <typename... types> template <typename type, typename... tail, typename lambda> void variant <types...> :: visitloop(lambda && callback) const
    {
        if(index <type, types...> () == this->_typeid)
        {
            callback(this->reinterpret <type> ());
            return;
        }

        if constexpr (sizeof...(tail) > 0)
            this->visitloop <tail...> (callback);
    }

    // Static private methods

    template <typename... types> template <typename needle, typename haystack, typename... tail> constexpr uint8_t variant <types...> :: index()
    {
        if constexpr (std :: is_same <needle, haystack> :: value)
            return 1;
        else
            return 1 + index <needle, tail...> ();
    }
};

#endif
