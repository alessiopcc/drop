// Forward declarations

namespace drop
{
    template <typename...> class variant;
    class undefined;
};

#if !defined(__forward__) && !defined(__src__data__variant__h)
#define __src__data__variant__h

// Libraries

#include <stdint.h>
#include <type_traits>

// Includes

#include "utils/math.hpp"
#include "concept/callable.h"

namespace drop
{
    // Classes

    template <typename... types> class variant
    {
    public:

        // Constraints

        class constraints
        {
            // Friends

            template <typename...> friend class variant;

            // Helpers

            template <typename, bool, typename> static constexpr bool callable();
            template <typename, bool, typename> static constexpr bool specific();

            template <bool, typename> static constexpr bool matchable();

        public:

            // Constraints

            template <typename, typename...> static constexpr bool distinct();

            template <typename> static constexpr bool defined();
            template <typename> static constexpr bool variant();

            template <typename> static constexpr bool copyable();
            template <typename> static constexpr bool movable();

            template <bool, typename...> static constexpr bool match();
        };

        // Static asserts

        static_assert(sizeof...(types) != 0, "Variants must have at least one type.");
        static_assert((... && (std :: is_same <std :: decay_t <types>, types> :: value)), "The types of a variant cannot be references, constants, or arrays.");
        static_assert(constraints :: template distinct <types...> (), "Variant types must all be distinct.");
        static_assert(!(... || std :: is_same <types, undefined> :: value), "Variant types cannot be undefined.");

    private:

        // Members

        uint8_t _typeid;
        std :: aligned_storage_t <max({sizeof(types)...}), max({alignof(types)...})> _value;

    public:

        // Constructors

        variant();

        template <typename type, std :: enable_if_t <constraints :: template copyable <type> ()> * = nullptr> variant(const type &);
        template <typename type, std :: enable_if_t <constraints :: template movable <type> ()> * = nullptr> variant(type &&);

        // Getters

        template <typename type, std :: enable_if_t <constraints :: template defined <type> ()> * = nullptr> type & reinterpret();
        template <typename type, std :: enable_if_t <constraints :: template defined <type> ()> * = nullptr> const type & reinterpret() const;

        // Methods

        template <typename type, std :: enable_if_t <constraints :: template variant <type> ()> * = nullptr> bool is() const;

        template <typename... lambdas, std :: enable_if_t <constraints :: template match <false, lambdas...> ()> * = nullptr> void match(lambdas && ...);
        template <typename... lambdas, std :: enable_if_t <constraints :: template match <true, lambdas...> ()> * = nullptr> void match(lambdas && ...) const;

    private:

        // Private methods

        template <typename type, typename... tail, typename lambda> void unwrap(lambda &&);
        template <typename type, typename... tail, typename lambda> void unwrap(lambda &&) const;

        template <typename type, bool specific, typename lambda, typename... tail> void matchloop(type &, lambda &&, tail && ...);
        template <typename type, bool specific, typename lambda, typename... tail> void matchloop(const type &, lambda &&, tail && ...) const;

        template <typename lambda, typename... tail> void matchloop(lambda &&, tail && ...) const;

        // Static private methods

        template <typename, typename, typename...> static constexpr uint8_t index();

    public:

        // Casting

        operator bool () const;
    };
};

#endif
