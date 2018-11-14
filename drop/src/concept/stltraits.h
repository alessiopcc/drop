// Forward declarations

namespace drop
{
    namespace stltraits
    {
        template <typename> struct array;
        template <typename> struct vector;
        template <typename> struct tuple;
    };
};

#if !defined(__forward__) && !defined(__src__concept__stltraits__h)
#define __src__concept__stltraits__h

#include <type_traits>
#include <array>
#include <vector>
#include <tuple>

namespace drop
{
    namespace stltraits
    {
        template <typename atype, size_t asize> struct array <std :: array <atype, asize>>
        {
            // Typedefs

            typedef atype type;

            // Static members

            static constexpr bool value = true;
            static constexpr size_t size = asize;
        };

        template <typename> struct array
        {
            // Static members

            static constexpr bool value = false;
        };

        template <typename vtype> struct vector <std :: vector <vtype>>
        {
            // Typedefs

            typedef vtype type;

            // Static members

            static constexpr bool value = true;
        };

        template <typename> struct vector
        {
            // Static members

            static constexpr bool value = false;
        };

        template <typename... types> struct tuple <std :: tuple <types...>>
        {
            // Typedefs

            template <size_t index> using type = std :: remove_reference_t <decltype(std :: get <index> (std :: declval <std :: tuple <types...> &> ()))>;

            // Static members

            static constexpr bool value = true;
            static constexpr size_t size = sizeof...(types);
        };

        template <typename> struct tuple
        {
            // Static members

            static constexpr bool value = false;
        };
    };
};

#endif
