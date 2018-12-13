#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "network/streamer.hpp"

namespace
{
    // Using

    using namespace drop;

    // Classes

    class wstream
    {
        // Members

        std :: vector <size_t> _steps;
        size_t _step;

    public:

        // Constructors

        wstream(const std :: vector <size_t> & steps) : _steps(steps), _step(0)
        {
        }

        size_t send(const uint8_t * chunk, size_t size)
        {
            size = std :: min(size, this->_steps[this->_step]);

            for(size_t i = 0; i < size; i++)
                std :: cout << (uint32_t) *(chunk + i) << std :: endl;

            this->_step++;

            return size;
        }

    };

    class rstream
    {
        // Members

        std :: vector <uint8_t> _data;
        std :: vector <size_t> _steps;

        size_t _step;
        size_t _cursor;

    public:

        // Constructors

        rstream(const std :: vector <uint8_t> & data, const std :: vector <size_t> & steps) : _data(data), _steps(steps), _step(0), _cursor(0)
        {
        }

        // Methods

        size_t receive(uint8_t * chunk, size_t size)
        {
            size = std :: min(size, (this->_steps[this->_step] - this->_cursor));
            memcpy(chunk, this->_data.data() + this->_cursor, size);

            this->_cursor += size;
            this->_step++;

            return size;
        }
    };

    // Tests

    $test("streamers/send", []
    {
        {
            std :: array <uint8_t, 8> mybuffer = {1, 2, 3, 4, 5, 6, 7, 8};
            wstream mystream({0, 1, 0, 3, 2, 100});

            streamer <send> mystreamer(mybuffer);
            while(!mystreamer.stream(mystream));
        }

        {
            size_t size = 192;

            std :: vector <uint8_t> mybuffer;
            mybuffer.resize(size);

            for(size_t i = 0; i < size; i++)
                mybuffer[i] = i;

            wstream mystream({0, 0, 0, 1, 1, 9, 8, 9, 9, 9, 9, 9, 9, 9, 9, size});

            streamer <send> mystreamer(mybuffer);
            while(!mystreamer.stream(mystream));
        }
    });

    $test("streamers/receive", []
    {
        {
            std :: array <uint8_t, 8> mybuffer;
            rstream mystream({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, {1, 2, 3, 3, 3, 3, 4, 12});

            streamer <receive> mystreamer(mybuffer);
            while(!mystreamer.stream(mystream));
        }

        {
            std :: vector <uint8_t> mybuffer;
            rstream mystream({192, 0, 0, 5, 4, 5, 6, 7, 8}, {0, 0, 0, 1, 1, 9, 8, 9, 9, 9, 9, 9, 9, 9, 9});

            streamer <receive> mystreamer(mybuffer);
            while(!mystreamer.stream(mystream));
        }
    });
};
