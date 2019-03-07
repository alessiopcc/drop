// Forward declarations

namespace drop
{
    class introspection;
};

#if !defined(__forward__) && !defined(__src__introspection__introspection__h)
#define __src__introspection__introspection__h

// Libraries

#include <type_traits>

// Macros

#define $$tagmember(tag, item, counter)                                         \
    /* Friends */                                                               \
                                                                                \
    friend class :: drop :: introspection;                                      \
                                                                                \
    /* Forward declarations */                                                  \
                                                                                \
    template <typename, size_t, std :: nullptr_t> class __tag__;                \
                                                                                \
    template <std :: nullptr_t __tagdummy__> class __tag__ <tag, :: drop ::     \
        introspection :: next <__tag__, tag, counter> (), __tagdummy__>         \
    {                                                                           \
        /* Friends */                                                           \
                                                                                \
        friend class :: drop :: introspection;                                  \
                                                                                \
        /* Exists */                                                            \
                                                                                \
        typedef void exists;                                                    \
                                                                                \
        /* Static methods */                                                    \
                                                                                \
        template <typename __tagtype__> static inline auto & get(__tagtype__ && \
            instance)                                                           \
        {                                                                       \
            return instance.item;                                               \
        }                                                                       \
    };

#define $$tagbase(tag, item, counter)                                           \
    /* Friends */                                                               \
                                                                                \
    friend class :: drop :: introspection;                                      \
                                                                                \
    /* Forward declarations */                                                  \
                                                                                \
    template <typename, size_t, std :: nullptr_t> class __tag__;                \
                                                                                \
    template <std :: nullptr_t __tagdummy__> class __tag__ <tag, :: drop ::     \
        introspection :: next <__tag__, tag, counter> (), __tagdummy__>         \
    {                                                                           \
        /* Friends */                                                           \
                                                                                \
        friend class :: drop :: introspection;                                  \
                                                                                \
        /* Exists */                                                            \
                                                                                \
        typedef void exists;                                                    \
                                                                                \
        /* Static methods */                                                    \
                                                                                \
        template <typename __tagtype__> static inline auto & get(__tagtype__ && \
            instance)                                                           \
        {                                                                       \
            if constexpr (std :: is_const <std :: remove_reference_t            \
                <__tagtype__>> :: value)                                        \
                return static_cast <const typename :: introspection :: traits   \
                :: argument <void (item)> :: type &> (instance);                \
            else                                                                \
                return static_cast <typename :: introspection :: traits         \
                :: argument <void (item)> :: type &> (instance);                \
        }                                                                       \
    };

#define $$tag(tag, type, item, counter) $$tag ## type(tag, item, counter)

#define $$tag2args(tag, item) $$tag(tag, member, item, __COUNTER__)
#define $$tag3args(tag, type, item) $$tag(tag, type, item, __COUNTER__)

#define $$tagget4tharg(arg1, arg2, arg3, arg4, ...) arg4
#define $$tagmacrochooser(...) $$tagget4tharg(__VA_ARGS__, $$tag3args, $$tag2args, $$tag1args,)
#define $tag(...) $$tagmacrochooser(__VA_ARGS__)(__VA_ARGS__)

namespace drop
{
    // Classes

    class introspection
    {
        // SFINAE

        struct sfinae
        {
            template <typename, typename> class tagged;
            template <template <typename, size_t, std :: nullptr_t> typename, typename, size_t, ssize_t> class exists;
        };

    public:

        // Constraints

        class constraints
        {
            // Helpers

            template <typename, typename, typename, size_t> static constexpr bool visitorloop();

        public:

            // Constraints

            template <typename, typename, typename> static constexpr bool visitor();
        };

        // Traits

        struct traits
        {
            template <typename> struct argument;
        };

    private:

        // Private static methods

        template <template <typename, size_t, std :: nullptr_t> typename, typename, ssize_t, size_t> static constexpr size_t nextloop();
        template <typename tag, size_t index, typename type, typename lambda> static inline void visitloop(type &&, lambda &&);

    public:

        // Static methods

        template <template <typename, size_t, std :: nullptr_t> typename, typename, size_t, ssize_t> static constexpr bool exists();
        template <template <typename, size_t, std :: nullptr_t> typename, typename, ssize_t> static constexpr size_t next();

        template <typename, typename> static constexpr size_t count();

        template <typename tag, size_t index, typename type, std :: enable_if_t <introspection :: exists <std :: decay_t <type> :: template __tag__, tag, index, -1> ()> * = nullptr> static inline auto & get(type &&);
        template <typename tag, typename type, typename lambda, std :: enable_if_t <constraints :: visitor <lambda, type, tag> ()> * = nullptr> static inline void visit(type &&, lambda &&);
    };

    template <typename type, typename tag> class introspection :: sfinae :: tagged
    {
        // SFINAE

        template <typename stype> static std :: true_type sfinae(typename stype :: template __tag__ <tag, 0, nullptr> *);
        template <typename stype> static std :: false_type sfinae(...);

    public:

        // Static members

        static constexpr bool value = std :: is_same <decltype(sfinae <type> (nullptr)), std :: true_type> :: value;
    };

    template <template <typename, size_t, std :: nullptr_t> typename progressive, typename tag, size_t index, ssize_t shuffle> class introspection :: sfinae :: exists
    {
        // SFINAE

        template <template <typename, size_t, std :: nullptr_t> typename sprogressive> static std :: true_type sfinae(typename sprogressive <tag, index, nullptr> :: exists *);
        template <template <typename, size_t, std :: nullptr_t> typename sprogressive> static std :: false_type sfinae(...);

    public:

        // Static members

        static constexpr bool value = std :: is_same <decltype(sfinae <progressive> (nullptr)), std :: true_type> :: value;
    };

    template <typename atype> struct introspection :: traits :: argument <void(atype)>
    {
        typedef atype type;
    };
}

#endif
