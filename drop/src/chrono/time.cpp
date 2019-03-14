// Includes

#include "time.hpp"

namespace drop
{
    // timestamp

    // Constructors

    timestamp :: timestamp()
    {
    }

    timestamp :: timestamp(const timeval & timeval) : _value(timeval.tv_sec * 1000000 + timeval.tv_usec)
    {
    }

    // Operators

    timestamp timestamp :: operator + (const interval & rho) const
    {
        timestamp timestamp = (*this);
        timestamp._value += ((const uint64_t &) rho);
        return timestamp;
    }

    timestamp timestamp :: operator - (const interval & rho) const
    {
        timestamp timestamp = (*this);
        timestamp._value -= ((const uint64_t &) rho);
        return timestamp;
    }

    uint64_t timestamp :: operator / (const interval & rho) const
    {
        return this->_value / ((const uint64_t &) rho);
    }

    interval timestamp :: operator - (const timestamp & rho) const
    {
        return interval(this->_value - rho._value);
    }

    bool timestamp :: operator < (const timestamp & rho) const
    {
        return (this->_value < rho._value);
    }

    bool timestamp :: operator <= (const timestamp & rho) const
    {
        return (this->_value <= rho._value);
    }

    bool timestamp :: operator > (const timestamp & rho) const
    {
        return (this->_value > rho._value);
    }

    bool timestamp :: operator >= (const timestamp & rho) const
    {
        return (this->_value >= rho._value);
    }

    bool timestamp :: operator == (const timestamp & rho) const
    {
        return (this->_value == rho._value);
    }

    bool timestamp :: operator != (const timestamp & rho) const
    {
        return (this->_value != rho._value);
    }

    // Casting

    timestamp :: operator interval () const
    {
        return interval(this->_value);
    }

    timestamp :: operator const uint64_t & () const
    {
        return this->_value;
    }

    timestamp :: operator timeval () const
    {
        timeval timeval;

        timeval.tv_sec = this->_value / 1000000;
        timeval.tv_usec = this->_value % 1000000;

        return timeval;
    }

    // interval

    // Constructors

    interval :: interval()
    {
    }

    interval :: interval(const timeval & timeval) : _value(timeval.tv_sec * 1000000 + timeval.tv_usec)
    {
    }

    // Operators

    interval interval :: operator + (const interval & rho) const
    {
        return interval(this->_value + rho._value);
    }

    interval interval :: operator - (const interval & rho) const
    {
        return (this->_value > rho._value) ? (this->_value - rho._value) : 0;
    }

    interval interval :: operator * (const double & rho) const
    {
        return interval(this->_value * rho);
    }

    interval interval :: operator / (const double & rho) const
    {
        return interval(this->_value / rho);
    }

    bool interval :: operator < (const interval & rho) const
    {
        return (this->_value < rho._value);
    }

    bool interval :: operator <= (const interval & rho) const
    {
        return (this->_value <= rho._value);
    }

    bool interval :: operator > (const interval & rho) const
    {
        return (this->_value > rho._value);
    }

    bool interval :: operator >= (const interval & rho) const
    {
        return (this->_value >= rho._value);
    }

    bool interval :: operator == (const interval & rho) const
    {
        return (this->_value == rho._value);
    }

    bool interval :: operator != (const interval & rho) const
    {
        return (this->_value != rho._value);
    }

    // Casting

    interval :: operator timestamp () const
    {
        return timestamp(this->_value);
    }

    interval :: operator const uint64_t & () const
    {
        return this->_value;
    }

    interval :: operator timeval () const
    {
        timeval timeval;

        timeval.tv_sec = this->_value / 1000000;
        timeval.tv_usec = this->_value % 1000000;

        return timeval;
    }

    // Static methods

    interval interval :: random(const interval & max)
    {
        interval interval;
        randombytes_buf(&(interval._value), sizeof(uint64_t));
        interval._value %= max._value;

        return interval;
    }

    // Functions

    timestamp now()
    {
        timeval time;
        gettimeofday(&time, nullptr);
        return timestamp(time.tv_sec * 1000000 + time.tv_usec);
    }

    void sleep(const timestamp & timestamp)
    {
        timestamp offset = now();

        if(timestamp > offset)
            usleep((const uint64_t &) (timestamp - offset));
    }

    void sleep(const interval & interval)
    {
        usleep((const uint64_t &) interval);
    }

    // Ostream integration

    std :: ostream & operator << (std :: ostream & out, const timestamp & timestamp)
    {
        time_t seconds = ((const uint64_t &) timestamp) / 1000000;
        tm * timeinfo = localtime(&seconds);

        char buffer[80];
        strftime(buffer, 80, "[%H:%M:%S (%b %d, %Y)]", timeinfo);

        return out << buffer;
    }

    std :: ostream & operator << (std :: ostream & out, const interval & interval)
    {
        if(((const uint64_t &) interval) < 1000ull)
            out << "(" << ((const uint64_t &) interval) << "us)";
        else if(((const uint64_t &) interval) < 1000000ull)
            out << "(" << std :: setprecision(4) << (((double) ((const uint64_t &) interval)) / 1000ull) << "ms)";
        else if(((const uint64_t &) interval) < 60000000ull)
            out << "(" << std :: setprecision(4) << (((double) ((const uint64_t &) interval)) / 1000000ull) << "s)";
        else if(((const uint64_t &) interval) < 3600000000ull)
            out << "(" << (((const uint64_t &) interval) / 60000000ull) << "m " << std :: setprecision(4) << (((double) (((const uint64_t &) interval) % 60000000ull)) / 1000000ull) << "s)";
        else
            out << "(" << (((const uint64_t &) interval) / 3600000000ull) << "h " << ((((const uint64_t &) interval) / 60000000ull) % 60) << "m " << std :: setprecision(4) << (((double) (((const uint64_t &) interval) % 60000000ull)) / 1000000ull) << "s)";
        return out;
    }
};
