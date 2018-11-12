// Includes

#include "bytewise.hpp"

namespace drop
{
    // sizer

    // Private constructors

    bytewise :: sizer :: sizer() : _size(0)
    {
    }

    // Methods

    void bytewise :: sizer :: update(const uint8_t *, const size_t & size)
    {
        this->_size += size;
    }

    // Operators

    bytewise :: sizer :: operator const size_t & () const
    {
        return this->_size;
    }
};
