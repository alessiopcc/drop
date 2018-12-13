// Includes

#include "streamers.hpp"

namespace drop
{
    // Constructors

    streamers :: receive :: receive(std :: vector <uint8_t> & buffer) : _stage(unallocated{.data = &buffer}), _cursor(0)
    {
    }
};
