// Includes

#include "streamer.hpp"

namespace drop
{
    // send

    // Constructors

    streamer <send> :: streamer(const std :: vector <uint8_t> & buffer) : _data(buffer.data()), _size(buffer.size()), _cursor(0)
    {
        varint(this->_size).visit([&](const auto & header)
        {
            this->_header = header;
        });
    }

    // receive

    // Constructors

    streamer <receive> :: streamer(std :: vector <uint8_t> & buffer) : _stage(unallocated{.data = &buffer}), _cursor(0)
    {
    }
};
