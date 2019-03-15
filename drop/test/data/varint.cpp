#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "drop/data/varint.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("varint/develop", []
    {
        auto buffer = std :: array <uint8_t, 4> ();

        buffer[0] = 1;

        auto one = varint(buffer.data());
        if(one != 1)
            throw "TODO 1";

        buffer[0] = 0 | 0x80;
        buffer[1] = 2;

        auto two = varint(buffer.data());
        if(two != 2)
            throw "TODO 2";

        buffer[0] = 0 | 0x80 | 0x40;
        buffer[1] = 0;
        buffer[2] = 0;
        buffer[3] = 4;

        auto four = varint(buffer.data());
        if(four != 4)
            throw "TODO 4";
    });

    $test("varint/size", []
    {
        uint8_t a = 5;
        uint8_t b = a | 0x80;
        uint8_t c = b | 0x40;

        if(varint :: size(a) != 1)
            throw "TODO 1";
        if(varint :: size(b) != 2)
            throw "TODO 2";
        if(varint :: size(c) != 4)
            throw "TODO 4";
    });
}
