#ifndef __src__chrono__time__hpp
#define __src__chrono__time__hpp

// Includes

#include "time.h"

namespace drop
{
    // timestamp

    // Constexpr constructors

    constexpr timestamp :: timestamp(const uint64_t & value) : _value(value)
    {
    }

    // interval

    // Constexpr constructors

    constexpr interval :: interval(const uint64_t & value) : _value(value)
    {
    }

    // Literals

    constexpr interval operator "" _us(unsigned long long int value)
    {
        return interval(value);
    }

    constexpr interval operator "" _ms(unsigned long long int value)
    {
        value *= 1000ull;
        return interval(value);
    }

    constexpr interval operator "" _ms(long double value)
    {
        value *= 1000ull;
        return interval(value);
    }

    constexpr interval operator "" _s(unsigned long long int value)
    {
        value *= 1000000ull;
        return interval(value);
    }

    constexpr interval operator "" _s(long double value)
    {
        value *= 1000000ull;
        return interval(value);
    }

    constexpr interval operator "" _m(unsigned long long int value)
    {
        value *= 60000000ull;
        return interval(value);
    }

    constexpr interval operator "" _m(long double value)
    {
        value *= 60000000ull;
        return interval(value);
    }

    constexpr interval operator "" _h(unsigned long long int value)
    {
        value *= 3600000000ull;
        return interval(value);
    }

    constexpr interval operator "" _h(long double value)
    {
        value *= 3600000000ull;
        return interval(value);
    }
};

#endif
