// Forward declarations

namespace drop
{
    class expression;
};

#if !defined(__forward__) && !defined(__src__concept__expression__h)
#define __src__concept__expression__h

// Libraries

#include <type_traits>

// Macros

#define $expression(expr) :: drop :: expression :: evaluate([](auto && identity) -> decltype(expr) {})
#define $type(type) identity(std :: declval <type> ())

namespace drop
{
    // Classes

    class expression
    {
        // SFINAE

        struct sfinae
        {
            template <typename> class valid;
        };

        // Service nested classes

        class identity;
        template <typename> class evaluator;

    public:

        // Static methods

        template <typename expr> static constexpr evaluator <expr> evaluate(const expr &);
    };

    template <typename expr> class expression :: sfinae :: valid
    {
        // SFINAE

        template <typename itype> static constexpr auto test(std :: nullptr_t) -> decltype(std :: declval <expr> ()(std :: declval <itype> ()), std :: true_type());
        template <typename itype> static constexpr std :: false_type test(...);

    public:

        // Static members

        static constexpr bool value = std :: is_same <decltype(test <identity> (nullptr)), std :: true_type> :: value;
    };

    class expression :: identity
    {
    public:

        // Operators

        template <typename type> type operator () (type &&);
    };

    template <typename expr> class expression :: evaluator
    {
    public:

        // Methods

        template <typename type> constexpr bool is() const;
        template <typename type> constexpr bool casts() const;

        // Casting

        constexpr operator bool () const;
    };
};

#endif
