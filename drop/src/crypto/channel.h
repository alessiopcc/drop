// Forward declarations

namespace drop
{
    // Tags

    class decryption_failed;

    // Classes

    class channel;
};

#if !defined(__forward__) && !defined(__src__crypto__channel__h)
#define __src__crypto__channel__h

// Library

#include <sodium.h>

// Forward includes

#define __forward__
#include "crypto/sodium.h"
#undef __forward__

// Includes

#include "bytewise/bytewise.hpp"

namespace drop
{
    class channel
    {
    public:

        // Nested classes

        class key : public std :: array <uint8_t, crypto_secretbox_KEYBYTES>
        {
        public:

            // Static methods

            static key random();
        };

        class nonce : public std :: array <uint8_t, crypto_secretbox_NONCEBYTES>
        {
        public:

            // Operators

            nonce & operator ++ ();
            nonce & operator ++ (int);

            // Static methods

            static nonce random();
        };

    private:

        // Members

        key _key;
        nonce _nonce;

    public:

        // Constructors

        channel();
        channel(const key &);
        channel(const key &, const nonce &);

        channel(const channel &) = delete;
        channel(channel &&) = delete;

        // Getters

        const key & key() const;
        const nonce & nonce() const;

        // Methods

        template <size_t size> std :: array <uint8_t, size + crypto_secretbox_MACBYTES> encrypt(const std :: array <uint8_t, size> &);
        template <size_t size, std :: enable_if_t <(size >= crypto_secretbox_MACBYTES)> * = nullptr> std :: array <uint8_t, size - crypto_secretbox_MACBYTES> decrypt(const std :: array <uint8_t, size> &);

        // Operators

        channel & operator = (const channel &) = delete;
        channel & operator = (channel &&) = delete;
    };
};

#endif
