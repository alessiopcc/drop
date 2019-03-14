// Forward declarations

namespace drop
{
    class timestamp;
    class interval;
};

#if !defined(__forward__) && !defined(__src__chrono__time__h)
#define __src__chrono__time__h

// Libraries

#include <sodium.h>
#include <stdint.h>
#include <sys/time.h>
#include <iostream>
#include <iomanip>
#include <unistd.h>

// Includes

#include "bytewise/bytewise.hpp"

namespace drop
{
    class timestamp
    {
        // Members

        uint64_t _value;

        // Bytewise

        $bytewise(_value);

    public:

        // Constructors

        timestamp();
        timestamp(const timeval &);

        // Constexpr constructors

        constexpr timestamp(const uint64_t &);

        // Operators

        timestamp operator + (const interval &) const;
        timestamp operator - (const interval &) const;

        uint64_t operator / (const interval &) const;

        interval operator - (const timestamp &) const;

        bool operator < (const timestamp &) const;
        bool operator <= (const timestamp &) const;

        bool operator > (const timestamp &) const;
        bool operator >= (const timestamp &) const;

        bool operator == (const timestamp &) const;
        bool operator != (const timestamp &) const;

        // Casting

        explicit operator interval () const;
        explicit operator const uint64_t & () const;
        explicit operator timeval () const;
    };

    class interval
    {
        // Members

        uint64_t _value;

        // Bytewise

        $bytewise(_value);

    public:

        // Constructors

        interval();
        interval(const timeval &);

        // Constexpr constructors

        constexpr interval(const uint64_t &);

        // Operators

        interval operator + (const interval &) const;
        interval operator - (const interval &) const;
        interval operator * (const double &) const;
        interval operator / (const double &) const;

        bool operator < (const interval &) const;
        bool operator <= (const interval &) const;

        bool operator > (const interval &) const;
        bool operator >= (const interval &) const;

        bool operator == (const interval &) const;
        bool operator != (const interval &) const;

        // Casting

        explicit operator timestamp () const;
        explicit operator const uint64_t & () const;
        explicit operator timeval () const;

        // Static methods

        static interval random(const interval &);
    };

    // Literals

    constexpr interval operator "" _us(unsigned long long int);

    constexpr interval operator "" _ms(unsigned long long int);
    constexpr interval operator "" _ms(long double);

    constexpr interval operator "" _s(unsigned long long int);
    constexpr interval operator "" _s(long double);

    constexpr interval operator "" _m(unsigned long long int);
    constexpr interval operator "" _m(long double);

    constexpr interval operator "" _h(unsigned long long int);
    constexpr interval operator "" _h(long double);

    // Functions

    timestamp now();

    void sleep(const timestamp &);
    void sleep(const interval &);

    // Ostream integration

    std :: ostream & operator << (std :: ostream &, const timestamp &);
    std :: ostream & operator << (std :: ostream &, const interval &);
};

#endif
