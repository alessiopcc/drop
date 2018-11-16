// Forward declarations

namespace drop
{
    // Tags

    class encryption_failed;
    class decryption_failed;
    class malformed_key;
    class message_too_short;

    // Classes

    class box;
};

#if !defined(__forward__) && !defined(__src__crypto__box__h)
#define __src__crypto__box__h

// Library

#include <sodium.h>

// Includes

#include "bytewise/bytewise.hpp"

namespace drop
{
    class box
    {
    public:

        // Nested classes

        class publickey : public std :: array <uint8_t, crypto_box_PUBLICKEYBYTES>
        {
        };

        class secretkey : public std :: array <uint8_t, crypto_box_SECRETKEYBYTES>
        {
        };

    private:

        // Members

        publickey _publickey;
        secretkey _secretkey;

    public:

        // Constructors

        box();
        box(const class publickey &, const class secretkey &);

        // Getters

        const publickey & publickey() const;
        const secretkey & secretkey() const;

        // Methods

        template <size_t size> std :: array <uint8_t, size + crypto_box_NONCEBYTES + crypto_box_MACBYTES> encrypt(const class publickey &, const std :: array <uint8_t, size> &) const;
        std :: vector <uint8_t> encrypt(const class publickey &, const std :: vector <uint8_t> &) const;

        template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: serializable <types> ())> * = nullptr> auto encrypt(const class publickey &, const types & ...) const;

        template <size_t size, std :: enable_if_t <(size > crypto_box_NONCEBYTES + crypto_box_MACBYTES)> * = nullptr> std :: array <uint8_t, size - (crypto_box_NONCEBYTES + crypto_box_MACBYTES)> decrypt(const class publickey &, const std :: array <uint8_t, size> &) const;
        std :: vector <uint8_t> decrypt(const class publickey &, const std :: vector <uint8_t> &) const;

        template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: fixed <types> ())> * = nullptr> auto decrypt(const class publickey &, const std :: array <uint8_t, (... + bytewise :: traits :: size <types> ()) + crypto_box_NONCEBYTES + crypto_box_MACBYTES> &) const;
        template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: deserializable <types> ()) && !(... && bytewise :: constraints :: fixed <types> ())> * = nullptr> auto decrypt(const class publickey &, const std :: vector <uint8_t> &) const;

    };
};

#endif
