// Includes

#include "varint.hpp"

namespace drop
{
    // Constructors

    varint :: varint(const uint32_t & value) : _value(value)
    {
    }

    varint :: varint(const uint8_t * buffer)
    {
        if(buffer[0] & 0x80)
        {
            if(buffer[0] & 0x40)
                this->_value = ((uint32_t)(buffer[0] & 0x3f) << 24) | ((uint32_t)(buffer[1]) << 16) | ((uint32_t)(buffer[2]) << 8) | (uint32_t)(buffer[3]);
            else
                this->_value = ((uint32_t)(buffer[0] & 0x7f) << 8) | (uint32_t)(buffer[1]);
        }
        else
            this->_value = buffer[0];
    }

    // Getters

    size_t varint :: size() const
    {
        if(this->_value < 128)
            return 1;
        else if(this->_value < 16384)
            return 2;
        else
            return 4;
    }

    // Operators

    varint & varint :: operator = (const uint32_t & value)
    {
        this->_value = value;
        return (*this);
    }

    // Casting

    varint :: operator uint32_t & ()
    {
        return this->_value;
    }

    varint :: operator const uint32_t & () const
    {
        return this->_value;
    }

    // Static methods

    size_t varint :: size(const uint8_t & head)
    {
        return (head & 0x80 ? (head & 0x40 ? 4 : 2) : 1);
    }
};
