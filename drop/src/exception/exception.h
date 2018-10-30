// Forward declarations

namespace drop
{
    template <typename...> class exception;
};

#if !defined(__forward__) && !defined(__src__exception__exception__h)
#define __src__exception__exception__h

// Libraries

#include <exception>
#include <iostream>

// Includes

#include "utils/parameters.h"

namespace drop
{
    template <> class exception <> : public std :: exception
    {
    public:

        // Interface

        virtual void source() const;
        virtual void details() const;
    };

    template <typename tag, typename... tags> class exception <tag, tags...> : public parameters :: backstrip_t <exception <tag, tags...>>
    {
        // Service nested classes

        class name;
        class null;

        template <typename, typename> class holder;

    public:

        // Static methods

        static void raise();

        template <typename stype> static void raise(stype *);
        static void raise(const char *);
        template <typename dtype> static void raise(const dtype &);

        template <typename stype> static void raise(stype *, const char *);
        template <typename dtype> static void raise(const char *, const dtype &);
        template <typename stype, typename dtype> static void raise(stype *, const dtype &);

        template <typename stype, typename dtype> static void raise(stype *, const char *, const dtype &);
    };

    template <typename tag, typename... tags> class exception <tag, tags...> :: name
    {
        // Members

        char * _name;

    public:

        // Constructors

        name();

        // Destructors

        ~name();

        // Casting

        operator const char * () const noexcept;
    };

    template <typename tag, typename... tags> class exception <tag, tags...> :: null
    {
    };

    template <typename tag, typename... tags> template <typename stype, typename dtype> class exception <tag, tags...> :: holder : public exception <tag, tags...>
    {
        // Members

        const stype * _source;
        const char * _what;
        dtype _details;

    public:

        // Constructors

        holder(const stype *, const char *, const dtype &);

        // Methods

        void source() const;
        void details() const;

        const char * what() const noexcept;
    };
};

#endif
