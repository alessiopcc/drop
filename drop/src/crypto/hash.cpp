// Includes

#include "hash.hpp"

namespace drop
{
    // hash

    // Constructors

    hash :: hash(bytewise)
    {
    }

    // Private constructors

    hash :: hash(crypto_generichash_state & state)
    {
        crypto_generichash_final(&state, this->data(), crypto_generichash_BYTES);
    }

    // key

    // Static methods

    hash :: key hash :: key :: random()
    {
        key key;
        randombytes_buf(key.data(), crypto_generichash_KEYBYTES);
        return key;
    }

    // state

    // Constructors

    hash :: state :: state()
    {
        crypto_generichash_init(&(this->_state), nullptr, 0, crypto_generichash_BYTES);
    }

    hash :: state :: state(const key & key)
    {
        crypto_generichash_init(&(this->_state), key.data(), crypto_generichash_KEYBYTES, crypto_generichash_BYTES);
    }

    // Methods

    hash :: state & hash :: state :: update(const uint8_t * chunk, const size_t & size)
    {
        crypto_generichash_update(&(this->_state), chunk, size);
        return (*this);
    }

    hash hash :: state :: finalize()
    {
        return hash(this->_state);
    }
};
