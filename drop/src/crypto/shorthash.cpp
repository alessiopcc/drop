// Includes

#include "shorthash.hpp"

namespace drop
{
    // shorthash

    // Constructors

    shorthash :: shorthash()
    {
        std :: array <uint8_t, crypto_shorthash_KEYBYTES> key;
        crypto_shorthash_keygen(key.data());

        this->_oneshot = new oneshot(key);
        this->_buffer = new buffer(key);
    }

    // Destructors

    shorthash :: ~shorthash()
    {
        delete this->_oneshot;
        delete this->_buffer;
    }

    // oneshot

    // Constructors

    shorthash :: oneshot :: oneshot(const std :: array <uint8_t, crypto_shorthash_KEYBYTES> & key) : _key(key)
    {
    }

    // Getters

    const uint64_t & shorthash :: oneshot :: value() const
    {
        return this->_value;
    }

    // Methods

    void shorthash :: oneshot :: update(const uint8_t * chunk, const size_t & size)
    {
        crypto_shorthash((uint8_t *) &(this->_value), chunk, size, this->_key.data());
    }

    // buffer

    // Constructors

    shorthash :: buffer :: buffer(const std :: array <uint8_t, crypto_shorthash_KEYBYTES> & key) : _capacity(settings :: base_capacity), _size(0), _buffer(new uint8_t [settings :: base_capacity]), _key(key)
    {
    }

    // Destructor

    shorthash :: buffer :: ~buffer()
    {
        delete [] this->_buffer;
    }

    // Methods

    void shorthash :: buffer :: update(const uint8_t * chunk, const size_t & size)
    {
        while(this->_size + size > this->_capacity)
        {
            this->_capacity *= 2;

            uint8_t * buffer = this->_buffer;
            this->_buffer = new uint8_t [this->_capacity];

            memcpy(this->_buffer, buffer, this->_size);
            delete [] buffer;
        }

        memcpy(this->_buffer + this->_size, chunk, size);
        this->_size += size;
    }

    uint64_t shorthash :: buffer :: finalize()
    {
        uint64_t value;
        crypto_shorthash((uint8_t *) &value, this->_buffer, this->_size, this->_key.data());
        this->_size = 0;

        return value;
    }
};
