#ifndef __src__exception__exception__hpp
#define __src__exception__exception__hpp

// Includes

#include "exception.h"

namespace drop
{
    // exception <tag, tags...>

    // Static methods

    template <typename tag, typename... tags> auto exception <tag, tags...> :: make()
    {
        return holder <null, null> (nullptr, nullptr, null());
    }

    template <typename tag, typename... tags> template <typename stype> auto exception <tag, tags...> :: make(stype * source)
    {
        return holder <stype, null> (source, nullptr, null());
    }

    template <typename tag, typename... tags> auto exception <tag, tags...> :: make(const char * what)
    {
        return holder <null, null> (nullptr, what, null());
    }

    template <typename tag, typename... tags> template <typename dtype> auto exception <tag, tags...> :: make(const dtype & details)
    {
        return holder <null, dtype> (nullptr, nullptr, details);
    }

    template <typename tag, typename... tags> template <typename stype> auto exception <tag, tags...> :: make(stype * source, const char * what)
    {
        return holder <stype, null> (source, what, null());
    }

    template <typename tag, typename... tags> template <typename dtype> auto exception <tag, tags...> :: make(const char * what, const dtype & details)
    {
        return holder <null, dtype> (nullptr, what, details);
    }

    template <typename tag, typename... tags> template <typename stype, typename dtype> auto exception <tag, tags...> :: make(stype * source, const dtype & details)
    {
        return holder <stype, dtype> (source, nullptr, details);
    }

    template <typename tag, typename... tags> template <typename stype, typename dtype> auto exception <tag, tags...> :: make(stype * source, const char * what, const dtype & details)
    {
        return holder <stype, dtype> (source, what, details);
    }

    template <typename tag, typename... tags> void exception <tag, tags...> :: raise()
    {
        throw holder <null, null> (nullptr, nullptr, null());
    }

    template <typename tag, typename... tags> template <typename stype> void exception <tag, tags...> :: raise(stype * source)
    {
        throw holder <stype, null> (source, nullptr, null());
    }

    template <typename tag, typename... tags> void exception <tag, tags...> :: raise(const char * what)
    {
        throw holder <null, null> (nullptr, what, null());
    }

    template <typename tag, typename... tags> template <typename dtype> void exception <tag, tags...> :: raise(const dtype & details)
    {
        throw holder <null, dtype> (nullptr, nullptr, details);
    }

    template <typename tag, typename... tags> template <typename stype> void exception <tag, tags...> :: raise(stype * source, const char * what)
    {
        throw holder <stype, null> (source, what, null());
    }

    template <typename tag, typename... tags> template <typename dtype> void exception <tag, tags...> :: raise(const char * what, const dtype & details)
    {
        throw holder <null, dtype> (nullptr, what, details);
    }

    template <typename tag, typename... tags> template <typename stype, typename dtype> void exception <tag, tags...> :: raise(stype * source, const dtype & details)
    {
        throw holder <stype, dtype> (source, nullptr, details);
    }

    template <typename tag, typename... tags> template <typename stype, typename dtype> void exception <tag, tags...> :: raise(stype * source, const char * what, const dtype & details)
    {
        throw holder <stype, dtype> (source, what, details);
    }

    // exception <tag, tags...> :: name

    // Constructors

    template <typename tag, typename... tags> exception <tag, tags...> :: name :: name() : _name(new char[sizeof(__PRETTY_FUNCTION__)])
    {
        size_t beg = 0;
        while(__PRETTY_FUNCTION__[beg++] != '<');

        size_t end = sizeof(__PRETTY_FUNCTION__) - 1;
        while(__PRETTY_FUNCTION__[--end] != '>');

        sprintf(this->_name, "[exception: %.*s]", (int) (end - beg), __PRETTY_FUNCTION__ + beg);
    }

    // Destructors

    template <typename tag, typename... tags> exception <tag, tags...> :: name :: ~name()
    {
        delete [] this->_name;
    }

    // Casting

    template <typename tag, typename... tags> exception <tag, tags...> :: name :: operator const char * () const noexcept
    {
        return this->_name;
    }

    // exception <tag, tags...> :: holder

    // Constructors

    template <typename tag, typename... tags> template <typename stype, typename dtype> exception <tag, tags...> :: holder <stype, dtype> :: holder(const stype * source, const char * what, const dtype & details) : _source(source), _what(what), _details(details)
    {
    }

    // Methods

    template <typename tag, typename... tags> template <typename stype, typename dtype> void exception <tag, tags...> :: holder <stype, dtype> :: source() const
    {
        if constexpr (!(std :: is_same <stype, null> :: value))
            throw this->_source;
    }

    template <typename tag, typename... tags> template <typename stype, typename dtype> void exception <tag, tags...> :: holder <stype, dtype> :: details() const
    {
        if constexpr (!(std :: is_same <dtype, null> :: value))
            throw this->_details;
    }

    template <typename tag, typename... tags> template <typename stype, typename dtype> const char * exception <tag, tags...> :: holder <stype, dtype> :: what() const noexcept
    {
        if(this->_what)
            return this->_what;
        else
        {
            static name name;
            return name;
        }
    };
};

#endif
