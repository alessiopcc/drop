// Includes

#include "channel.hpp"

namespace drop
{
    // channel

    // Constructors

    channel :: channel() : _key(key :: random()), _nonce(nonce :: random())
    {
    }

    channel :: channel(const class key & key) : _key(key), _nonce(nonce :: random())
    {
    }

    channel :: channel(const class key & key, const class nonce & nonce) : _key(key), _nonce(nonce)
    {
    }

    // Getters

    const class channel :: key & channel :: key() const
    {
        return this->_key;
    }

    const class channel :: nonce & channel :: nonce() const
    {
        return this->_nonce;
    }

    // Methods

    std :: vector <uint8_t> channel :: encrypt(const std :: vector <uint8_t> & plaintext)
    {
        (this->_nonce)++;

        std :: vector <uint8_t> ciphertext(plaintext.size() + crypto_secretbox_MACBYTES);
        crypto_secretbox_easy(ciphertext.data(), plaintext.data(), plaintext.size(), this->_nonce.data(), this->_key.data());

        return ciphertext;
    }

    std :: vector <uint8_t> channel :: decrypt(const std :: vector <uint8_t> & ciphertext)
    {
        (this->_nonce++);

        if(ciphertext.size() < crypto_secretbox_MACBYTES)
            exception <decryption_failed, message_too_short> :: raise(this);

        std :: vector <uint8_t> plaintext(ciphertext.size() - crypto_secretbox_MACBYTES);

        if(crypto_secretbox_open_easy(plaintext.data(), ciphertext.data(), ciphertext.size(), this->_nonce.data(), this->_key.data()))
            exception <decryption_failed, mac_mismatch> :: raise(this);

        return plaintext;
    }

    // key

    // Static methods

    class channel :: key channel :: key :: random()
    {
        key key;
        randombytes_buf(key.data(), crypto_secretbox_KEYBYTES);
        return key;
    }

    // nonce

    // Operators

    class channel :: nonce & channel :: nonce :: operator ++ ()
    {
        sodium_increment(this->data(), crypto_secretbox_NONCEBYTES);
        return (*this);
    }

    class channel :: nonce & channel :: nonce :: operator ++ (int)
    {
        return ++(*this);
    }

    // Static methods

    class channel :: nonce channel :: nonce :: random()
    {
        nonce nonce;
        randombytes_buf(nonce.data(), crypto_secretbox_NONCEBYTES);
        return nonce;
    }
};
