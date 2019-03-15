#ifndef __src__crypto__channel__hpp
#define __src__crypto__channel__hpp

// Includes

#include "channel.h"
#include "drop/exception/exception.hpp"

namespace drop
{
    // channel

    // Methods

    template <size_t size> std :: array <uint8_t, size + crypto_secretbox_MACBYTES> channel :: encrypt(const std :: array <uint8_t, size> & plaintext)
    {
        (this->_nonce)++;

        std :: array <uint8_t, size + crypto_secretbox_MACBYTES> ciphertext;
        crypto_secretbox_easy(ciphertext.data(), plaintext.data(), size, this->_nonce.data(), this->_key.data());

        return ciphertext;
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: serializable <types> ())> *> auto channel :: encrypt(const types & ... items)
    {
        return this->encrypt(bytewise :: serialize(items...));
    }

    template <size_t size, std :: enable_if_t <(size >= crypto_secretbox_MACBYTES)> *> std :: array <uint8_t, size - crypto_secretbox_MACBYTES> channel :: decrypt(const std :: array <uint8_t, size> & ciphertext)
    {
        (this->_nonce++);

        std :: array <uint8_t, size - crypto_secretbox_MACBYTES> plaintext;

        if(crypto_secretbox_open_easy(plaintext.data(), ciphertext.data(), size, this->_nonce.data(), this->_key.data()))
            exception <decryption_failed, mac_mismatch> :: raise(this);

        return plaintext;
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: fixed <types> ())> *> auto channel :: decrypt(const std :: array <uint8_t, (... + bytewise :: traits :: size <types> ()) + crypto_secretbox_MACBYTES> & ciphertext)
    {
        return bytewise :: deserialize <types...> (this->decrypt(ciphertext));
    }

    template <typename... types, std :: enable_if_t <(sizeof...(types) > 0) && (... && bytewise :: constraints :: deserializable <types> ()) && !(... && bytewise :: constraints :: fixed <types> ())> *> auto channel :: decrypt(const std :: vector <uint8_t> & ciphertext)
    {
        return bytewise :: deserialize <types...> (this->decrypt(ciphertext));
    }
};

#endif
