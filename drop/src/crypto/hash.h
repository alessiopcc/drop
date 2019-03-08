// Forward declarations

namespace drop
{
    class hash;
};

#if !defined(__forward__) && !defined(__src__crypto__hash__h)
#define __src__crypto__hash__h

// Libraries

#include <sodium.h>

// Includes

#include "bytewise/bytewise.hpp"

namespace drop
{
    class hash : public std :: array <uint8_t, crypto_generichash_BYTES>
    {
    public:

        // Nested classes

        class key;
        class state;

        // Constructors

        template <typename... types, std :: enable_if_t <(... && (bytewise :: constraints :: readable <types, state> ()))> * = nullptr> hash(const types & ...);

    private:

        // Private constructors

        hash(crypto_generichash_state &);

    public:

        // Static methods

        template <typename... types, std :: enable_if_t <(... && (bytewise :: constraints :: readable <types, state> ()))> * = nullptr> static hash keyed(const key &, const types & ...);
    };

    class hash :: key : public std :: array <uint8_t, crypto_generichash_KEYBYTES>
    {
        // Bytewise

        $bytewise(base, (std :: array <uint8_t, crypto_generichash_KEYBYTES>));
        
    public:

        // Static methods

        static key random();
    };

    class hash :: state
    {
        // Members

        crypto_generichash_state _state;

    public:

        // Constructors

        state();
        state(const key &);

        // Methods

        state & update(const uint8_t *, const size_t &);
        template <typename... types, std :: enable_if_t <(... && (bytewise :: constraints :: readable <types, state> ()))> * = nullptr> state & update(const types & ...);
        hash finalize();
    };
};

#endif
