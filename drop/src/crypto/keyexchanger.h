// Forward declarations

namespace drop
{
    // Tags

    class keyexchange_failed;
    class malformed_key;

    // Classes

    class keyexchanger;
};

#if !defined(__forward__) && !defined(__src__crypto__keyexchanger__h)
#define __src__crypto__keyexchanger__h

// Libraries

#include <sodium.h>
#include <array>

// Includes

#include "crypto/channel.h"

namespace drop
{
    class keyexchanger
    {
    public:

        // Nested classes

        class publickey : public std :: array <uint8_t, crypto_kx_PUBLICKEYBYTES>
        {
        };

        class secretkey : public std :: array <uint8_t, crypto_kx_SECRETKEYBYTES>
        {
        };

        struct keypair
        {
            // Asserts

            static_assert(crypto_kx_SESSIONKEYBYTES == crypto_secretbox_KEYBYTES, "The keys produced by sodium key-exchanger do not fit a secretbox key.");

            // Public members

            class channel :: key receive;
            class channel :: key transmit;
        };

    private:

        // Members

        publickey _publickey;
        secretkey _secretkey;

    public:

        // Constructors

        keyexchanger();
        keyexchanger(const publickey &, const secretkey &);

        // Getters

        const publickey & publickey() const;
        const secretkey & secretkey() const;

        // Methods

        keypair exchange(const class publickey &) const;
    };
};

#endif
