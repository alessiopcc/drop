// Forward declarations

namespace drop
{
    namespace stltraits
    {
        template <typename> struct array;
        template <typename> struct vector;
    };
};

#if !defined(__forward__) && !defined(__src__concept__stltraits__h)
#define __src__concept__stltraits__h

#include <type_traits>
#include <array>
#include <vector>

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
    };
};

#endif
