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
