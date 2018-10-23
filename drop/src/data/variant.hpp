#ifndef __src__data__variant__hpp
#define __src__data__variant__hpp

// Includes

#include "variant.h"

namespace drop
{
    // Constraint helpers

    template <typename... types> template <typename type, bool constvar, typename lambda> constexpr bool variant <types...> :: constraints :: callable()
    {
        if constexpr (constvar)
            return :: drop :: callable <lambda, type> :: value ||
                   :: drop :: callable <lambda, const type> :: value ||
                   :: drop :: callable <lambda, const type &> :: value;
        else
            return :: drop :: callable <lambda, type> :: value ||
                   :: drop :: callable <lambda, const type> :: value ||
                   :: drop :: callable <lambda, type &> :: value ||
                   :: drop :: callable <lambda, const type &> :: value;
    }

    template <typename... types> template <typename type, bool constvar, typename lambda> constexpr bool variant <types...> :: constraints :: specific()
    {
        return callable <type, constvar, lambda> () && ((... + (callable <types, constvar, lambda> () ? 1 : 0)) == 1);
    }

    template <typename... types> template <bool constvar, typename lambda> constexpr bool variant <types...> :: constraints :: matchable()
    {
        return (:: drop :: callable <lambda> :: value ||  (... || callable <types, constvar, lambda> ()));
    }

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

    template <typename... types> template <bool constvar, typename... lambdas> constexpr bool variant <types...> :: constraints :: match()
    {
        return (... && matchable <constvar, lambdas> ());
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

    template <typename... types> template <typename... lambdas, std :: enable_if_t <variant <types...> :: constraints :: template match <false, lambdas...> ()> *> void variant <types...> :: match(lambdas && ... matchcases)
    {
        if(!(*this))
            this->matchloop(matchcases...);
        else
            this->unwrap <types...> ([&](auto & value)
            {
                typedef std :: decay_t <decltype(value)> type;
                constexpr bool specific = (... || (constraints :: template specific <type, false, lambdas> ()));

                this->matchloop <type, specific> (value, matchcases...);
            });
    }

    template <typename... types> template <typename... lambdas, std :: enable_if_t <variant <types...> :: constraints :: template match <true, lambdas...> ()> *> void variant <types...> :: match(lambdas && ... matchcases) const
    {
        if(!(*this))
            this->matchloop(matchcases...);
        else
            this->unwrap <types...> ([&](auto & value)
            {
                typedef std :: decay_t <decltype(value)> type;
                constexpr bool specific = (... || (constraints :: template specific <type, true, lambdas> ()));

                this->matchloop <type, specific> (value, matchcases...);
            });
    }

    // Private methods

    template <typename... types> template <typename type, typename... tail, typename lambda> void variant <types...> :: unwrap(lambda && callback)
    {
        if(index <type, types...> () == this->_typeid)
        {
            callback(this->reinterpret <type> ());
            return;
        }

        if constexpr (sizeof...(tail) > 0)
            this->unwrap <tail...> (callback);
    }

    template <typename... types> template <typename type, typename... tail, typename lambda> void variant <types...> :: unwrap(lambda && callback) const
    {
        if(index <type, types...> () == this->_typeid)
        {
            callback(this->reinterpret <type> ());
            return;
        }

        if constexpr (sizeof...(tail) > 0)
            this->unwrap <tail...> (callback);
    }

    template <typename... types> template <typename type, bool specific, typename lambda, typename... tail> void variant <types...> :: matchloop(type & value, lambda && matchcase, tail && ... matchtail)
    {
        if constexpr ((specific && constraints :: template specific <type, false, lambda> ()) || (!specific && constraints :: template callable <type, false, lambda> ()))
            matchcase(value);
        else
        {
            if constexpr (sizeof...(tail) > 0)
                this->matchloop <type, specific> (value, matchtail...);
        }
    }

    template <typename... types> template <typename type, bool specific, typename lambda, typename... tail> void variant <types...> :: matchloop(const type & value, lambda && matchcase, tail && ... matchtail) const
    {
        if constexpr ((specific && constraints :: template specific <type, true, lambda> ()) || (!specific && constraints :: template callable <type, true, lambda> ()))
            matchcase(value);
        else
        {
            if constexpr (sizeof...(tail) > 0)
                this->matchloop <type, specific> (value, matchtail...);
        }
    }

    template <typename... types> template <typename lambda, typename... tail> void variant <types...> :: matchloop(lambda && matchcase, tail && ... matchtail) const
    {
        if constexpr (:: drop :: callable <lambda> :: value)
            matchcase();
        else
        {
            if constexpr (sizeof...(tail) > 0)
                this->matchloop(matchtail...);
        }
    }

    // Static private methods

    template <typename... types> template <typename needle, typename haystack, typename... tail> constexpr uint8_t variant <types...> :: index()
    {
        if constexpr (std :: is_same <needle, haystack> :: value)
            return 1;
        else
            return 1 + index <needle, tail...> ();
    }

    // Casting

    template <typename... types> variant <types...> :: operator bool () const
    {
        return this->_typeid;
    }
};

#endif
