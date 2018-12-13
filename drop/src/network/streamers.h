// Forward declarations

namespace drop
{
    class streamers;
};

#if !defined(__forward__) && !defined(__src__network__streamers__h)
#define __src__network__streamers__h

// Libraries

#include <vector>
#include <array>

// Includes

#include "data/variant.hpp"
#include "data/varint.hpp"
#include "concept/expression.hpp"

namespace drop
{
    class streamers
    {
    public:

        // Nested classes

        class send;
        class receive;
    };

    class streamers :: send
    {
        // Constraints

        struct constraints
        {
            template <typename> static constexpr bool stream();
        };

        // Members

        variant <std :: array <uint8_t, 1>, std :: array <uint8_t, 2>, std :: array <uint8_t, 4>> _header;

        const uint8_t * _data;
        size_t _size;

        size_t _cursor;

    public:

        // Constructors

        template <size_t size> send(const std :: array <uint8_t, size> &);
        send(const std :: vector <uint8_t> &);

        // Methods

        template <typename type, std :: enable_if_t <constraints :: stream <type> ()> * = nullptr> bool stream(type &);
    };

    class streamers :: receive
    {
        // Constraints

        struct constraints
        {
            template <typename> static constexpr bool stream();
        };

        // Service nested classes

        struct unallocated
        {
            std :: vector <uint8_t> * data;
            std :: array <uint8_t, 4> size;
        };

        struct allocated
        {
            uint8_t * data;
            size_t size;
        };

        // Members

        variant <unallocated, allocated> _stage;
        size_t _cursor;

    public:

        // Constructors

        template <size_t size> receive(std :: array <uint8_t, size> &);
        receive(std :: vector <uint8_t> &);

        // Methods

        template <typename type, std :: enable_if_t <constraints :: stream <type> ()> * = nullptr> bool stream(type &);
    };
};

#endif
