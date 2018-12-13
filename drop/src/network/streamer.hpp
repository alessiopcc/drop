#ifndef __src__network__streamer__hpp
#define __src__network__streamer__hpp

// Includes

#include "streamer.h"

namespace drop
{
    // send

    // Constraints

    template <typename type> constexpr bool streamer <send> :: constraints :: stream()
    {
        return $expression($type(type).send($type(const uint8_t*), $type(size_t))).template casts <size_t> ();
    }

    // Constructors

    template <size_t size> streamer <send> :: streamer(const std :: array <uint8_t, size> & buffer) : _data(buffer.data()), _size(size), _cursor(0)
    {
    }

    // Methods

    template <typename type, std :: enable_if_t <streamer <send> :: constraints :: stream <type> ()> *> bool streamer <send> :: stream(type & stream)
    {
        bool completed = false;

        this->_header.match([&](const auto & header)
        {
            size_t sent;

            do
            {
                sent = stream.send(header.data() + this->_cursor, header.size() - this->_cursor);
                this->_cursor += sent;
            }
            while(sent && (this->_cursor < header.size()));

            if(this->_cursor == header.size())
            {
                this->_header.erase();
                this->_cursor = 0;

                completed = this->stream(stream);
            }
        }, [&]()
        {
            size_t sent;

            do
            {
                sent = stream.send(this->_data + this->_cursor, this->_size - this->_cursor);
                this->_cursor += sent;
            }
            while(sent && (this->_cursor < this->_size));

            if(this->_cursor == this->_size)
                completed = true;
        });

        return completed;
    }

    // receive

    // Constraints

    template <typename type> constexpr bool streamer <receive> :: constraints :: stream()
    {
        return $expression($type(type).receive($type(uint8_t *), $type(size_t))).template casts <size_t> ();
    }

    // Constructors

    template <size_t size> streamer <receive> :: streamer(std :: array <uint8_t, size> & buffer) : _stage(allocated{.data = buffer.data(), .size = size}), _cursor(0)
    {
    }

    // Methods

    template <typename type, std :: enable_if_t <streamer <receive> :: constraints :: stream <type> ()> *> bool streamer <receive> :: stream(type & stream)
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
