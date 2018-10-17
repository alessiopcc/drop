// Forward declarations

namespace drop
{
    class introspection;
};

#if !defined(__forward__) && !defined(__src__introspection__introspection__h)
#define __src__introspection__introspection__h

// Includes

#include <type_traits>

// Macros

#define $$$tag(tag, item, counter)                                              \
    /* Friends */                                                               \
                                                                                \
    friend class :: drop :: introspection;                                      \
                                                                                \
    /* Forward declarations */                                                  \
                                                                                \
    template <typename, size_t, std :: nullptr_t> class __tag__;                \
                                                                                \
    template <std :: nullptr_t __tagdummy__> class __tag__ <tag, :: drop ::     \
        introspection :: count <__tag__, tag, counter> (), __tagdummy__>        \
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

#define $$tag(tag, item, counter) $$$tag(tag, item, counter)
#define $tag(tag, item) $$tag(tag, item, __COUNTER__)

namespace drop
{
    // Classes

    class introspection
    {
        // SFINAE

        struct sfinae
        {
            template <template <typename, size_t, std :: nullptr_t> typename, typename, size_t, ssize_t> class exists;
        };

        // Private static methods

        template <template <typename, size_t, std :: nullptr_t> typename, typename, ssize_t, size_t> static constexpr size_t countloop();
        template <typename tag, size_t index, typename type, typename lambda> static inline void visitloop(type &&, lambda &&);

    public:

        // Static methods

        template <template <typename, size_t, std :: nullptr_t> typename, typename, size_t, ssize_t> static constexpr bool exists();
        template <template <typename, size_t, std :: nullptr_t> typename, typename, ssize_t> static constexpr size_t count();

        template <typename tag, size_t index, typename type, std :: enable_if_t <introspection :: exists <std :: decay_t <type> :: template __tag__, tag, index, -1> ()> * = nullptr> static inline auto & get(type &&);
        template <typename tag, typename type, typename lambda> static inline void visit(type &&, lambda &&);
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
}

#endif
