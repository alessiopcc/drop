// Forward declarations

namespace drop
{
    // Tags

    class signature_failed;
    class verification_failed;
    class malformed_key;

    // Classes

    class signature;
    class signer;
    class verifier;
};

#if !defined(__forward__) && !defined(__src__crypto__signature__h)
#define __src__crypto__signature__h

// Libraries

#include <sodium.h>

// Includes

#include "bytewise/bytewise.hpp"

namespace drop
{
    class signature : public std :: array <uint8_t, crypto_sign_BYTES>
    {
    };

    class signer
    {
    public:

        // Nested classes

        class publickey : public std :: array <uint8_t, crypto_sign_PUBLICKEYBYTES>
        {
        };

        class secretkey : public std :: array <uint8_t, crypto_sign_SECRETKEYBYTES>
        {
        };

    private:

        // Members

        publickey _publickey;
        secretkey _secretkey;

    public:

        // Constructors

        signer();
        signer(const publickey &, const secretkey &);

        // Getters

        const publickey & publickey() const;
        const secretkey & secretkey() const;

        // Methods

        template <size_t size> signature sign(const std :: array <uint8_t, size> &) const;
        signature sign(const std :: vector <uint8_t> &) const;
        template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: serializable <types> ())> * = nullptr> signature sign(const types & ...) const;
    };

    class verifier
    {
        // Members

        class signer :: publickey _publickey;

    public:

        // Constructors

        verifier(const class signer :: publickey &);

        // Getters

        const class signer :: publickey & publickey() const;

        // Methods

        template <size_t size> void verify(const signature &, const std :: array <uint8_t, size> &) const;
        void verify(const signature &, const std :: vector <uint8_t> &) const;
        template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: serializable <types> ())> * = nullptr> void verify(const signature &, const types & ...) const;
    };
};

#endif
