#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "network/streamers.hpp"

namespace
{
    // Using

    using namespace drop;

    // Classes

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

    $test("streamers/develop", []
    {
        {
            std :: array <uint8_t, 8> mybuffer;
            rstream mystream({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, {1, 2, 3, 3, 3, 3, 4, 12});

            streamers :: receive mystreamer(mybuffer);
            while(!mystreamer.stream(mystream));
        }

        {
            std :: vector <uint8_t> mybuffer;
            rstream mystream({192, 0, 0, 5, 4, 5, 6, 7, 8}, {0, 0, 0, 1, 1, 9, 8, 9, 9, 9, 9, 9, 9, 9, 9});

            streamers :: receive mystreamer(mybuffer);
            while(!mystreamer.stream(mystream));    
        }
    });
};
