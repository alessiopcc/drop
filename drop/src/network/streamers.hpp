#ifndef __src__network__streamers__hpp
#define __src__network__streamers__hpp

// Includes

#include "streamers.h"

namespace drop
{
    // receive

    // Constraints

    template <typename type> constexpr bool streamers :: receive :: constraints :: stream()
    {
        return $expression($type(type).receive($type(uint8_t *), $type(size_t))).template casts <size_t> ();
    }

    // Constructors

    template <size_t size> streamers :: receive :: receive(std :: array <uint8_t, size> & buffer) : _stage(allocated{.data = buffer.data(), .size = size}), _cursor(0)
    {
    }

    // Methods

    template <typename type, std :: enable_if_t <streamers :: receive :: constraints :: stream <type> ()> *> bool streamers :: receive :: stream(type & stream)
    {
        bool completed = false;

        this->_stage.match([&](unallocated & stage)
        {
            size_t received;

            do
            {
                received = stream.receive(stage.size.data() + this->_cursor, this->_cursor ? (varint :: size(stage.size[0]) - this->_cursor) : 1);
                this->_cursor += received;
            }
            while(received && this->_cursor < varint :: size(stage.size[0]));

            if(this->_cursor == varint :: size(stage.size[0]))
            {
                size_t size = varint(stage.size.data());
                stage.data->resize(size);

                uint8_t * data = stage.data->data();
                this->_stage = allocated{.data = data, .size = size};
                this->_cursor = 0;

                completed = this->stream(stream);
            }
        }, [&](allocated & stage)
        {
            size_t received;

            do
            {
                received = stream.receive(stage.data + this->_cursor, stage.size - this->_cursor);
                this->_cursor += received;
            }
            while(received && (this->_cursor < stage.size));

            if(this->_cursor == stage.size)
                completed = true;
        });

        return completed;
    }
};

#endif
