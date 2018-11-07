// Forward declarations

namespace drop
{
    class bytewise;
};

#if !defined(__forward__) && !defined(__src__bytewise__bytewise__h)
#define __src__bytewise__bytewise__h

// Libraries

#include <array>
#include <vector>
#include <stdint.h>

// Includes

#include "bytewise/endianess.h"
#include "introspection/introspection.h"
#include "concept/expression.h"
#include "concept/stltraits.h"

// Macros

#define $bytewise(member) $tag(:: drop :: bytewise, member)

namespace drop
{
    // Classes

    class bytewise
    {
    public:

        // Constraints

        class constraints
        {
            // Constraint helpers

            template <typename, typename, size_t> static constexpr bool readableloop();
            template <typename, typename, size_t> static constexpr bool writableloop();

            template <typename, size_t> static constexpr bool fixedloop();

        public:

            // Constraints

            template <typename> static constexpr bool constructible();

            template <typename, typename> static constexpr bool readable();
            template <typename, typename> static constexpr bool writable();

            template <typename> static constexpr bool fixed();

            template <typename> static constexpr bool reader();
            template <typename> static constexpr bool writer();
        };

        // Traits

        class traits
        {
            // Trait helpers

            template <typename, size_t> static constexpr size_t sizeloop();

        public:

            // Traits

            template <typename> static constexpr size_t size();
        };

        // Nested classes

        template <typename> class reader;
        template <typename> class writer;

        // Friends

        template <typename> friend class reader;
        template <typename> friend class writer;

    public:

        // Static methods

        template <typename vtype, typename type, std :: enable_if_t <constraints :: readable <type, vtype> ()> * = nullptr> static void read(vtype &, const type &);
        template <typename vtype, typename type, std :: enable_if_t <constraints :: writable <type, vtype> ()> * = nullptr> static void write(vtype &, type &);

    private:

        // Private static methods

        template <typename vtype, typename type> static void visit(reader <vtype> &, const type &);
        template <typename vtype, typename type> static void visit(writer <vtype> &, type &);
    };

    template <typename vtype> class bytewise :: reader
    {
        // Asserts

        static_assert(bytewise :: constraints :: reader <vtype> (), "A `reader` must expose a `void update(const char *, const size_t &)` method (or anything compatible).");

        // Friends

        friend class bytewise;

        // Members

        vtype & _visitor;

        // Private constructors

        reader(vtype &);

    public:

        // Methods

        template <typename type, std :: enable_if_t <bytewise :: constraints :: readable <type, vtype> ()> * = nullptr> void visit(const type &);
    };

    template <typename vtype> class bytewise :: writer
    {
        // Asserts

        static_assert(bytewise :: constraints :: writer <vtype> (), "A `writer` must expose a `const char * pop(const size_t &)` method (or anything compatible).");

        // Friends

        friend class bytewise;

        // Members

        vtype & _visitor;

        // Private constructors

        writer(vtype &);

    public:

        // Methods

        template <typename type, std :: enable_if_t <bytewise :: constraints :: writable <type, vtype> ()> * = nullptr> void visit(type &);
    };
};

#endif
