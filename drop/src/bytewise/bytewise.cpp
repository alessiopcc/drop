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

// Ostream integration

std :: ostream & operator << (std :: ostream & out, const std :: vector <uint8_t> & data)
{
    out << "[";

    for(size_t i = 0; i < data.size(); i++)
    {
        if(data[i] >= 32 && data[i] < 127)
            out << (char)(data[i]);
        else
            out << "(" << std :: setfill('0') << std :: setw(2) << std :: hex << (unsigned int)(data[i]) << ")" << std :: dec;
    }

    return out << "]";
}
