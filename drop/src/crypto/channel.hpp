#ifndef __src__crypto__channel__hpp
#define __src__crypto__channel__hpp

// Includes

#include "channel.h"
#include "exception/exception.hpp"

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

    template <size_t size, std :: enable_if_t <(size >= crypto_secretbox_MACBYTES)> *> std :: array <uint8_t, size - crypto_secretbox_MACBYTES> channel :: decrypt(const std :: array <uint8_t, size> & ciphertext)
    {
        (this->_nonce++);

        std :: array <uint8_t, size - crypto_secretbox_MACBYTES> plaintext;

        if(crypto_secretbox_open_easy(plaintext.data(), ciphertext.data(), size, this->_nonce.data(), this->_key.data()))
            exception <sodium, decryption_failed> :: raise(this);

        return plaintext;
    }
};

#endif
