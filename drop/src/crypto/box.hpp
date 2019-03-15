#ifndef __src__crypto__box__hpp
#define __src__crypto__box__hpp

// Includes

#include "box.h"
#include "drop/exception/exception.hpp"

namespace drop
{
    // Methods

    template <size_t size> std :: array <uint8_t, size + crypto_box_NONCEBYTES + crypto_box_MACBYTES> box :: encrypt(const class publickey & to, const std :: array <uint8_t, size> & plaintext) const
    {
        std :: array <uint8_t, size + crypto_box_NONCEBYTES + crypto_box_MACBYTES> ciphertext;
        randombytes_buf(ciphertext.data(), crypto_box_NONCEBYTES);

        if(crypto_box_easy(ciphertext.data() + crypto_box_NONCEBYTES, plaintext.data(), size, ciphertext.data(), to.data(), this->_secretkey.data()))
            exception <encryption_failed, malformed_key> :: raise(this);

        return ciphertext;
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: serializable <types> ())> *> auto box :: encrypt(const class publickey & to, const types & ... items) const
    {
        return this->encrypt(to, bytewise :: serialize(items...));
    }

    template <size_t size, std :: enable_if_t <(size > crypto_box_NONCEBYTES + crypto_box_MACBYTES)> *> std :: array <uint8_t, size - (crypto_box_NONCEBYTES + crypto_box_MACBYTES)> box :: decrypt(const class publickey & from, const std :: array <uint8_t, size> & ciphertext) const
    {
        std :: array <uint8_t, size - (crypto_box_NONCEBYTES + crypto_box_MACBYTES)> plaintext;

        if(crypto_box_open_easy(plaintext.data(), ciphertext.data() + crypto_box_NONCEBYTES, size - crypto_box_NONCEBYTES, ciphertext.data(), from.data(), this->_secretkey.data()))
            exception <decryption_failed> :: raise(this);

        return plaintext;
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: fixed <types> ())> *> auto box :: decrypt(const class publickey & from, const std :: array <uint8_t, (... + bytewise :: traits :: size <types> ()) + crypto_box_NONCEBYTES + crypto_box_MACBYTES> & ciphertext) const
    {
        return bytewise :: deserialize <types...> (this->decrypt(from, ciphertext));
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: deserializable <types> ()) && !(... && bytewise :: constraints :: fixed <types> ())> *> auto box :: decrypt(const class publickey & from, const std :: vector <uint8_t> & ciphertext) const
    {
        return bytewise :: deserialize <types...> (this->decrypt(from, ciphertext));
    }

    template <size_t size, std :: enable_if_t <(size > crypto_box_SEALBYTES)> *> std :: array <uint8_t, size - crypto_box_SEALBYTES> box :: unseal(const std :: array <uint8_t, size> & ciphertext) const
    {
        std :: array <uint8_t, size - crypto_box_SEALBYTES> plaintext;

        if(crypto_box_seal_open(plaintext.data(), ciphertext.data(), size, this->_publickey.data(), this->_secretkey.data()))
            exception <decryption_failed> :: raise(this);

        return plaintext;
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: fixed <types> ())> *> auto box :: unseal(const std :: array <uint8_t, (... + bytewise :: traits :: size <types> ()) + crypto_box_SEALBYTES> & ciphertext) const
    {
        return bytewise :: deserialize <types...> (this->unseal(ciphertext));
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: deserializable <types> ()) && !(... && bytewise :: constraints :: fixed <types> ())> *> auto box :: unseal(const std :: vector <uint8_t> & ciphertext) const
    {
        return bytewise :: deserialize <types...> (this->unseal(ciphertext));
    }

    // Static methods

    template <size_t size> std :: array <uint8_t, size + crypto_box_SEALBYTES> box :: seal(const class publickey & to, const std :: array <uint8_t, size> & plaintext)
    {
        std :: array <uint8_t, size + crypto_box_SEALBYTES> ciphertext;

        if(crypto_box_seal(ciphertext.data(), plaintext.data(), size, to.data()))
            exception <encryption_failed, malformed_key> :: raise();

        return ciphertext;
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: serializable <types> ())> *> auto box :: seal(const class publickey & to, const types & ... items)
    {
        return seal(to, bytewise :: serialize(items...));
    }
};

#endif
