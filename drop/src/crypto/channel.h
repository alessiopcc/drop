// Forward declarations

namespace drop
{
    // Tags

    class decryption_failed;
    class message_too_short;
    class mac_mismatch;

    // Classes

    class channel;
};

#if !defined(__forward__) && !defined(__src__crypto__channel__h)
#define __src__crypto__channel__h

// Library

#include <sodium.h>

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

            // Methods

            nonce specular() const;

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
        std :: vector <uint8_t> encrypt(const std :: vector <uint8_t> &);

        template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: serializable <types> ())> * = nullptr> auto encrypt(const types & ...);

        template <size_t size, std :: enable_if_t <(size >= crypto_secretbox_MACBYTES)> * = nullptr> std :: array <uint8_t, size - crypto_secretbox_MACBYTES> decrypt(const std :: array <uint8_t, size> &);
        std :: vector <uint8_t> decrypt(const std :: vector <uint8_t> &);
        
        template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: fixed <types> ())> * = nullptr> auto decrypt(const std :: array <uint8_t, (... + bytewise :: traits :: size <types> ()) + crypto_secretbox_MACBYTES> &);
        template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: deserializable <types> ()) && !(... && bytewise :: constraints :: fixed <types> ())> * = nullptr> auto decrypt(const std :: vector <uint8_t> &);

        // Operators

        channel & operator = (const channel &) = delete;
        channel & operator = (channel &&) = delete;
    };
};

#endif
