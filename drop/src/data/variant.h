// Forward declarations

namespace drop
{
    // Tags

    class bad_access;
    class write_failed;
    class type_mismatch;
    class malformed_type;

    // Classes

    template <typename> class base;

    template <typename...> class variant;
    class undefined;

    // Using

    template <typename type> using optional = variant <type>;
};

#if !defined(__forward__) && !defined(__src__data__variant__h)
#define __src__data__variant__h

// Libraries

#include <stdint.h>
#include <type_traits>

// Includes

#include "drop/utils/math.hpp"
#include "drop/concept/callable.h"
#include "drop/utils/enablers.h"
#include "drop/bytewise/bytewise.hpp"

namespace drop
{
    // Classes

    template <typename... types> class base <variant <types...>>
    {
    public:

        // Constraints

        class constraints
        {
            // Friends

            friend class base <variant <types...>>;

            // Helpers

            template <typename, bool, typename> static constexpr bool callable();
            template <typename, bool, typename> static constexpr bool specific();

            template <bool, typename> static constexpr bool matchable();

        public:

            // Constraints

            template <typename, typename...> static constexpr bool distinct();

            template <typename> static constexpr bool defined();
            template <typename> static constexpr bool variant();

            template <typename> static constexpr bool copyconstructible();
            template <typename> static constexpr bool moveconstructible();

            template <typename> static constexpr bool copyassignable();
            template <typename> static constexpr bool moveassignable();

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

    protected:

        // Protected constructors

        base();

        template <typename type> base(const type &);
        template <typename type> base(type &&);

        base(const base &);
        base(base &&);

        // Protected destructor

        ~base();

    public:

        // Bytewise

        template <typename atype, std :: enable_if_t <(sizeof(atype) >= 0) && (... && bytewise :: constraints :: readable <types, atype> ())> * = nullptr> void accept(bytewise :: reader <atype> &) const;
        template <typename atype, std :: enable_if_t <(sizeof(atype) >= 0) && (... && bytewise :: constraints :: constructible <types> ()) && (... && bytewise :: constraints :: writable <types, atype> ())> * = nullptr> void accept(bytewise :: writer <atype> &);

        // Getters

        template <typename type, std :: enable_if_t <constraints :: template defined <type> ()> * = nullptr> type & get();
        template <typename type, std :: enable_if_t <constraints :: template defined <type> ()> * = nullptr> const type & get() const;

        template <typename type, std :: enable_if_t <constraints :: template defined <type> ()> * = nullptr> type & reinterpret();
        template <typename type, std :: enable_if_t <constraints :: template defined <type> ()> * = nullptr> const type & reinterpret() const;

        // Methods

        template <typename type, std :: enable_if_t <constraints :: template variant <type> ()> * = nullptr> bool is() const;

        template <typename type, typename... atypes, std :: enable_if_t <std :: is_constructible <type, atypes...> :: value> * = nullptr> void emplace(atypes && ...);

        template <typename... lambdas, std :: enable_if_t <constraints :: template match <false, lambdas...> ()> * = nullptr> void match(lambdas && ...);
        template <typename... lambdas, std :: enable_if_t <constraints :: template match <true, lambdas...> ()> * = nullptr> void match(lambdas && ...) const;

        void erase();

    private:

        // Private methods

        template <typename type, typename... tail, typename lambda> void scaffold(const uint8_t &, lambda &&);

        template <typename type, typename... tail, typename lambda> void unwrap(lambda &&);
        template <typename type, typename... tail, typename lambda> void unwrap(lambda &&) const;

        template <typename type, bool specific, typename lambda, typename... tail> void matchloop(type &, lambda &&, tail && ...);
        template <typename type, bool specific, typename lambda, typename... tail> void matchloop(const type &, lambda &&, tail && ...) const;

        template <typename lambda, typename... tail> void matchloop(lambda &&, tail && ...) const;

        // Static private methods

        template <typename, typename, typename...> static constexpr uint8_t index();

    protected:

        // Protected operators

        template <typename type> base & operator = (const type &);
        template <typename type> base & operator = (type &&);

    public:

        // Operators

        template <typename... dummy, std :: enable_if_t <(sizeof...(dummy) == 0) && (sizeof...(types) == 1)> * = nullptr> auto & operator * (dummy...);
        template <typename... dummy, std :: enable_if_t <(sizeof...(dummy) == 0) && (sizeof...(types) == 1)> * = nullptr> const auto & operator * (dummy...) const;

        base & operator = (const base &);
        base & operator = (base &&);

        // Casting

        operator bool () const;
    };

    template <typename... types> class variant : public base <variant <types...>>,
                                                 public enablers :: copy_constructible <(... && (std :: is_copy_constructible <types> :: value))>,
                                                 public enablers :: move_constructible <(... && (std :: is_move_constructible <types> :: value))>,
                                                 public enablers :: copy_assignable <(... && (std :: is_copy_assignable <types> :: value)) && (... && (std :: is_copy_constructible <types> :: value))>,
                                                 public enablers :: move_assignable <(... && (std :: is_move_assignable <types> :: value)) && (... && (std :: is_move_constructible <types> :: value))>
    {
    public:

        // Constraints

        typedef typename base <variant <types...>> :: constraints constraints;

        // Constructors

        variant();

        template <typename type, std :: enable_if_t <constraints :: template copyconstructible <type> ()> * = nullptr> variant(const type &);
        template <typename type, std :: enable_if_t <constraints :: template moveconstructible <type> ()> * = nullptr> variant(type &&);

        // Operators

        template <typename type, typename std :: enable_if_t <constraints :: template copyassignable <type> ()> * = nullptr> variant & operator = (const type &);
        template <typename type, typename std :: enable_if_t <!(std :: is_lvalue_reference <type> :: value) && (constraints :: template moveassignable <std :: remove_const_t <std :: remove_reference_t <type>>> ())> * = nullptr> variant & operator = (type &&);

        // Static methods

        template <typename type, typename... atypes, std :: enable_if_t <std :: is_constructible <type, atypes...> :: value> * = nullptr> static variant <types...> construct(atypes && ...);
    };
};

#endif
