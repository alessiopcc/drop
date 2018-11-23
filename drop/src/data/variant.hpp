#ifndef __src__data__variant__hpp
#define __src__data__variant__hpp

// Includes

#include "variant.h"
#include "exception/exception.hpp"

namespace drop
{
    // base <variant>

    // Constraint helpers

    template <typename... types> template <typename type, bool constvar, typename lambda> constexpr bool base <variant <types...>> :: constraints :: callable()
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

    template <typename... types> template <typename type, bool constvar, typename lambda> constexpr bool base <variant <types...>> :: constraints :: specific()
    {
        return callable <type, constvar, lambda> () && ((... + (callable <types, constvar, lambda> () ? 1 : 0)) == 1);
    }

    template <typename... types> template <bool constvar, typename lambda> constexpr bool base <variant <types...>> :: constraints :: matchable()
    {
        return (:: drop :: callable <lambda> :: value ||  (... || callable <types, constvar, lambda> ()));
    }

    // Constraints

    template <typename... types> template <typename needle, typename... haystack> constexpr bool base <variant <types...>> :: constraints :: distinct()
    {
        if constexpr (sizeof...(haystack) == 0)
            return true;
        else
            return !(... || std :: is_same <needle, haystack> :: value) && distinct <haystack...> ();
    }

    template <typename... types> template <typename type> constexpr bool base <variant <types...>> :: constraints :: defined()
    {
        return (... || std :: is_same <type, types> :: value);
    }

    template <typename... types> template <typename type> constexpr bool base <variant <types...>> :: constraints :: variant()
    {
        return (std :: is_same <type, undefined> :: value) || defined <type> ();
    }

    template <typename... types> template <typename type> constexpr bool base <variant <types...>> :: constraints :: copyconstructible()
    {
        if constexpr (defined <type> ())
            return std :: is_copy_constructible <type> :: value;
        else
            return false;
    }

    template <typename... types> template <typename type> constexpr bool base <variant <types...>> :: constraints :: moveconstructible()
    {
        if constexpr (defined <type> ())
            return std :: is_move_constructible <type> :: value;
        else
            return false;
    }

    template <typename... types> template <typename type> constexpr bool base <variant <types...>> :: constraints :: copyassignable()
    {
        if constexpr (defined <type> ())
            return (std :: is_copy_constructible <type> :: value) && (std :: is_copy_assignable <type> :: value);
        else
            return false;
    }

    template <typename... types> template <typename type> constexpr bool base <variant <types...>> :: constraints :: moveassignable()
    {
        if constexpr (defined <type> ())
            return (std :: is_move_constructible <type> :: value) && (std :: is_move_assignable <type> :: value);
        else
            return false;
    }

    template <typename... types> template <bool constvar, typename... lambdas> constexpr bool base <variant <types...>> :: constraints :: match()
    {
        return (... && matchable <constvar, lambdas> ());
    }

    // Protected constructors

    template <typename... types> base <variant <types...>> :: base() : _typeid(0)
    {
    }

    template <typename... types> template <typename type> base <variant <types...>> :: base(const type & value) : _typeid(index <type, types...> ())
    {
        new (&(this->_value)) type(value);
    }

    template <typename... types> template <typename type> base <variant <types...>> :: base(type && value) : _typeid(index <type, types...> ())
    {
        new (&(this->_value)) type(std :: move(value));
    }

    template <typename... types> base <variant <types...>> :: base(const base & that)
    {
        this->_typeid = that._typeid;

        that.unwrap <types...> ([&](auto && value)
        {
            typedef std :: remove_const_t <std :: remove_reference_t <decltype(value)>> vtype;
            new (&(this->_value)) vtype(value);
        });
    }

    template <typename... types> base <variant <types...>> :: base(base && that)
    {
        this->_typeid = that._typeid;

        that.unwrap <types...> ([&](auto && value)
        {
            typedef std :: remove_const_t <std :: remove_reference_t <decltype(value)>> vtype;
            new (&(this->_value)) vtype(std :: move(value));
        });
    }

    // Protected destructor

    template <typename... types> base <variant <types...>> :: ~base()
    {
       this->unwrap <types...> ([](auto && value)
       {
           typedef std :: remove_reference_t <decltype(value)> vtype;
           value.~vtype();
       });
    }

    // Bytewise

    template <typename... types> template <typename atype, std :: enable_if_t <(sizeof(atype) >= 0) && (... && bytewise :: constraints :: readable <types, atype> ())> *> void base <variant <types...>> :: accept(bytewise :: reader <atype> & reader) const
    {
        reader.visit(this->_typeid);

        this->unwrap <types...> ([&](const auto & value)
        {
            reader.visit(value);
        });
    }

    template <typename... types> template <typename atype, std :: enable_if_t <(sizeof(atype) >= 0) && (... && bytewise :: constraints :: constructible <types> ()) && (... && bytewise :: constraints :: writable <types, atype> ())> *> void base <variant <types...>> :: accept(bytewise :: writer <atype> & writer)
    {
        uint8_t tid;
        writer.visit(tid);

        if(tid > sizeof...(types))
            exception <write_failed, malformed_type> :: raise(this);

        if(this->_typeid != tid)
        {
            this->~base();

            if(tid)
                this->scaffold <types...> (tid, [&](auto & value)
                {
                    writer.visit(value);
                });
            else
                this->_typeid = 0;
        }
        else
            this->unwrap <types...> ([&](auto & value)
            {
                writer.visit(value);
            });
    }

    // Getters

    template <typename... types> template <typename type, std :: enable_if_t <base <variant <types...>> :: constraints :: template defined <type> ()> *> type & base <variant <types...>> :: get()
    {
        if(index <type, types...> () != this->_typeid)
            exception <bad_access, type_mismatch> :: raise(this);

        return reinterpret_cast <type &> (this->_value);
    }

    template <typename... types> template <typename type, std :: enable_if_t <base <variant <types...>> :: constraints :: template defined <type> ()> *> const type & base <variant <types...>> :: get() const
    {
        if(index <type, types...> () != this->_typeid)
            exception <bad_access, type_mismatch> :: raise(this);

        return reinterpret_cast <const type &> (this->_value);
    }

    template <typename... types> template <typename type, std :: enable_if_t <base <variant <types...>> :: constraints :: template defined <type> ()> *> type & base <variant <types...>> :: reinterpret()
    {
        return reinterpret_cast <type &> (this->_value);
    }

    template <typename... types> template <typename type, std :: enable_if_t <base <variant <types...>> :: constraints :: template defined <type> ()> *> const type & base <variant <types...>> :: reinterpret() const
    {
        return reinterpret_cast <const type &> (this->_value);
    }

    // Methods

    template <typename... types> template <typename type, std :: enable_if_t <base <variant <types...>> :: constraints :: template variant <type> ()> *> bool base <variant <types...>> :: is() const
    {
        if constexpr (std :: is_same <type, undefined> :: value)
            return (this->_typeid == 0);
        else
            return (this->_typeid == index <type, types...> ());
    }

    template <typename... types> template <typename type, typename... atypes, std :: enable_if_t <std :: is_constructible <type, atypes...> :: value> *> void base <variant <types...>> :: emplace(atypes && ... arguments)
    {
        this->~base();
        this->_typeid = index <type, types...> ();
        new (&(this->_value)) type(std :: forward <atypes> (arguments)...);
    }

    template <typename... types> template <typename... lambdas, std :: enable_if_t <base <variant <types...>> :: constraints :: template match <false, lambdas...> ()> *> void base <variant <types...>> :: match(lambdas && ... matchcases)
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

    template <typename... types> template <typename... lambdas, std :: enable_if_t <base <variant <types...>> :: constraints :: template match <true, lambdas...> ()> *> void base <variant <types...>> :: match(lambdas && ... matchcases) const
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

    template <typename... types> void base <variant <types...>> :: erase()
    {
        this->~base();
        this->_typeid = 0;
    }

    // Private methods

    template <typename... types> template <typename type, typename... tail, typename lambda> void base <variant <types...>> :: scaffold(const uint8_t & tid, lambda && callback)
    {
        if(tid == 1)
        {
            this->_typeid = index <type, types...> ();

            if constexpr (std :: is_constructible <type, bytewise> :: value)
                new (&(this->_value)) type(bytewise{});
            else
                new (&(this->_value)) type();

            callback(this->reinterpret <type> ());

            return;
        }

        if constexpr (sizeof...(tail) > 0)
            this->scaffold <tail...> (tid - 1, callback);
    }

    template <typename... types> template <typename type, typename... tail, typename lambda> void base <variant <types...>> :: unwrap(lambda && callback)
    {
        if(index <type, types...> () == this->_typeid)
        {
            callback(this->reinterpret <type> ());
            return;
        }

        if constexpr (sizeof...(tail) > 0)
            this->unwrap <tail...> (callback);
    }

    template <typename... types> template <typename type, typename... tail, typename lambda> void base <variant <types...>> :: unwrap(lambda && callback) const
    {
        if(index <type, types...> () == this->_typeid)
        {
            callback(this->reinterpret <type> ());
            return;
        }

        if constexpr (sizeof...(tail) > 0)
            this->unwrap <tail...> (callback);
    }

    template <typename... types> template <typename type, bool specific, typename lambda, typename... tail> void base <variant <types...>> :: matchloop(type & value, lambda && matchcase, tail && ... matchtail)
    {
        if constexpr ((specific && constraints :: template specific <type, false, lambda> ()) || (!specific && constraints :: template callable <type, false, lambda> ()))
            matchcase(value);
        else
        {
            if constexpr (sizeof...(tail) > 0)
                this->matchloop <type, specific> (value, matchtail...);
        }
    }

    template <typename... types> template <typename type, bool specific, typename lambda, typename... tail> void base <variant <types...>> :: matchloop(const type & value, lambda && matchcase, tail && ... matchtail) const
    {
        if constexpr ((specific && constraints :: template specific <type, true, lambda> ()) || (!specific && constraints :: template callable <type, true, lambda> ()))
            matchcase(value);
        else
        {
            if constexpr (sizeof...(tail) > 0)
                this->matchloop <type, specific> (value, matchtail...);
        }
    }

    template <typename... types> template <typename lambda, typename... tail> void base <variant <types...>> :: matchloop(lambda && matchcase, tail && ... matchtail) const
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

    template <typename... types> template <typename needle, typename haystack, typename... tail> constexpr uint8_t base <variant <types...>> :: index()
    {
        if constexpr (std :: is_same <needle, haystack> :: value)
            return 1;
        else
            return 1 + index <needle, tail...> ();
    }

    // Protected operators

    template <typename... types> template <typename type> base <variant <types...>> & base <variant <types...>> :: operator = (const type & value)
    {
        if(this->_typeid == index <type, types...> ())
            this->reinterpret <type> () = value;
        else
        {
            this->~base();
            this->_typeid = index <type, types...> ();
            new (&(this->_value)) type(value);
        }

        return (*this);
    }

    template <typename... types> template <typename type> base <variant <types...>> & base <variant <types...>> :: operator = (type && value)
    {
        if(this->_typeid == index <type, types...> ())
            this->reinterpret <type> () = std :: move(value);
        else
        {
            this->~base();
            this->_typeid = index <type, types...> ();
            new (&(this->_value)) type(std :: move(value));
        }

        return (*this);
    }

    // Operators

    template <typename... types> base <variant <types...>> & base <variant <types...>> :: operator = (const base & rho)
    {
        if(this->_typeid == rho._typeid)
        {
            rho.unwrap <types...> ([&](auto && value)
            {
                typedef std :: remove_const_t <std :: remove_reference_t <decltype(value)>> vtype;
                reinterpret_cast <vtype &> (this->_value) = value;
            });
        }
        else
        {
            this->~base();
            this->_typeid = rho._typeid;

            rho.unwrap <types...> ([&](auto && value)
            {
                typedef std :: remove_const_t <std :: remove_reference_t <decltype(value)>> vtype;
                new (&(this->_value)) vtype(value);
            });
        }

        return (*this);
    }

    template <typename... types> base <variant <types...>> & base <variant <types...>> :: operator = (base && rho)
    {
        if(this->_typeid == rho._typeid)
        {
            rho.unwrap <types...> ([&](auto && value)
            {
                typedef std :: remove_const_t <std :: remove_reference_t <decltype(value)>> vtype;
                reinterpret_cast <vtype &> (this->_value) = std :: move(value);
            });
        }
        else
        {
            this->~base();
            this->_typeid = rho._typeid;

            rho.unwrap <types...> ([&](auto && value)
            {
                typedef std :: remove_const_t <std :: remove_reference_t <decltype(value)>> vtype;
                new (&(this->_value)) vtype(std :: move(value));
            });
        }

        return (*this);
    }

    // Casting

    template <typename... types> base <variant <types...>> :: operator bool () const
    {
        return this->_typeid;
    }

    // variant

    // Constructors

    template <typename... types> variant <types...> :: variant() : base <variant <types...>> ()
    {
    }

    template <typename... types> template <typename type, std :: enable_if_t <variant <types...> :: constraints :: template copyconstructible <type> ()> *> variant <types...> :: variant(const type & value) : base <variant <types...>> (value)
    {
    }

    template <typename... types> template <typename type, std :: enable_if_t <variant <types...> :: constraints :: template moveconstructible <type> ()> *> variant <types...> :: variant(type && value) : base <variant <types...>> (std :: move(value))
    {
    }

    // Operators

    template <typename... types> template <typename type, typename std :: enable_if_t <variant <types...> :: constraints :: template copyassignable <type> ()> *> variant <types...> & variant <types...> :: operator = (const type & rho)
    {
        base <variant <types...>> :: operator = (rho);
        return (*this);
    }

    template <typename... types> template <typename type, typename std :: enable_if_t <!(std :: is_lvalue_reference <type> :: value) && (variant <types...> :: constraints :: template moveassignable <std :: remove_const_t <std :: remove_reference_t <type>>> ())> *> variant <types...> & variant <types...> :: operator = (type && rho)
    {
        base <variant <types...>> :: operator = (std :: move(rho));
        return (*this);
    }

    // Static methods

    template <typename... types> template <typename type, typename... atypes, std :: enable_if_t <std :: is_constructible <type, atypes...> :: value> *> variant <types...> variant <types...> :: construct(atypes && ... arguments)
    {
        variant <types...> variant;
        variant.template emplace <type> (std :: forward <atypes> (arguments)...);
        return variant;
    }
};

#endif
