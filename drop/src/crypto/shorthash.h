// Forward declarations

namespace drop
{
    class shorthash;
};

#if !defined(__forward__) && !defined(__src__crypto__shorthash__h)
#define __src__crypto__shorthash__h

// Libraries

#include <sodium.h>

// Includes

#include "drop/bytewise/bytewise.hpp"

namespace drop
{
    class shorthash
    {
        // Service nested classes

        class oneshot;
        class buffer;

        // Members

        oneshot * _oneshot;
        buffer * _buffer;

    public:

        // Constructors

        shorthash();

        // Destructors

        ~shorthash();

        // Operators

        template <typename type, std :: enable_if_t <bytewise :: constraints :: oneshot <type> ()> * = nullptr> size_t operator () (const type &) const noexcept;
        template <typename type, std :: enable_if_t <bytewise :: constraints :: readable <type, buffer> () && !(bytewise :: constraints :: oneshot <type> ())> * = nullptr> size_t operator () (const type &) const noexcept;
    };

    class shorthash :: oneshot
    {
        // Members

        std :: array <uint8_t, crypto_shorthash_KEYBYTES> _key;
        uint64_t _value;

    public:

        // Constructors

        oneshot(const std :: array <uint8_t, crypto_shorthash_KEYBYTES> &);

        // Getters

        const uint64_t & value() const;

        // Methods

        void update(const uint8_t *, const size_t &);
    };

    class shorthash :: buffer
    {
        // Settings

        struct settings
        {
            static constexpr size_t base_capacity = 32;
        };

        // Members

        size_t _capacity;
        size_t _size;
        uint8_t * _buffer;

        std :: array <uint8_t, crypto_shorthash_KEYBYTES> _key;

    public:

        // Constructors

        buffer(const std :: array <uint8_t, crypto_shorthash_KEYBYTES> &);

        // Destructor

        ~buffer();

        // Methods

        void update(const uint8_t *, const size_t &);
        uint64_t finalize();
    };
};

#endif
