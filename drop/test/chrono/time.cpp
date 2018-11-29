#include "framework/test.hpp"

// Libraries

#include <iostream>

// Includes

#include "chrono/time.hpp"

namespace
{
    // Using

    using namespace drop;

    // Tests

    $test("time/literals", []
    {
        // TODO: unsigned long long int vs. long double

        auto us = uint64_t(3600000000_us);
        auto ms = uint64_t(3600000_ms);
        auto m = uint64_t(60_m);
        auto h = uint64_t(1_h);

        if(us != ms || us != m || us != h || ms != m || ms != h || m != h)
            throw "Literals initializing `interval` to the wrong value.";
    });

    $test("time/interval", []
    {
        // TODO: interval()

        auto myinterval = interval(1000);
        auto myotherinterval = 1_ms;

        if(uint64_t(myinterval) != uint64_t(myotherinterval))
            throw "`interval` initialized to the wrong value by constructor.";

        myotherinterval = 1_us;

        if(uint64_t(myinterval - myotherinterval) != 999)
            throw "`operator -` does not work correctly.";
        if(uint64_t(myinterval + myotherinterval) != 1001)
            throw "`operator +` does not work correctly.";

        if(uint64_t(myinterval / 5) != 200)
            throw "`operator /` does not work correctly.";
        if(uint64_t(myinterval * 2.0) != 2000)
            throw "`operator *` does not work correctly.";

        if(uint64_t(60_m + 1_h) != uint64_t((1_s * 60 + 239_m) / 2))
            throw "`interval` operators and literal does not work correctly togheter.";

        myinterval = interval :: random(1_h);
        myotherinterval = interval :: random(myinterval);

        if(uint64_t(myinterval) == uint64_t(interval :: random(1_h)))
            throw "`intervall :: random` returns same value given the same paramter.";
        if(uint64_t(myinterval) > uint64_t(interval(1_h)) || uint64_t(myotherinterval) > uint64_t(myinterval))
            throw "`interval :: random` does not respect given max.";
    });


    $test("time/timestamp", []
    {
        // TODO: timestamp()

        auto mytimestamp = timestamp(1111);
        auto myothertimestamp = timestamp(111);

        if(uint64_t(mytimestamp) != 1111 || myothertimestamp != 111)
            throw "`timestamp` construction followed by cast, does not return the correct value.";
        if(uint64_t(mytimestamp - myothertimestamp) != 1000)
            throw "`operator -` between `timestamp` does not return the correct value.";

        if(!(mytimestamp != myothertimestamp))
            throw "`operator !=` does not work correctly.";
        if(!(mytimestamp == timestamp(1111)))
            throw "`operator ==` does not work correctly.";

        if(!(mytimestamp > myothertimestamp) || myothertimestamp > mytimestamp)
            throw "`operator >` does not work correctly.";
        if(mytimestamp < myothertimestamp || !(myothertimestamp < mytimestamp))
            throw "`operator <` does not work correctly.";
        if(!(mytimestamp >= timestamp(1111)) || !(mytimestamp > myothertimestamp) || myothertimestamp > mytimestamp)
            throw "`operator >=` does not work correctly.";
        if(!(mytimestamp <= timestamp(1111)) || mytimestamp < myothertimestamp || !(myothertimestamp < mytimestamp))
            throw "`operator <=` does not work correctly.";

        if(myothertimestamp + 1_ms != mytimestamp)
            throw "`operator +` with `interval` does not work correctly.";
        if(mytimestamp - 1_ms != myothertimestamp)
            throw "`operator -` with `interval` does not work correctly.";

        if(timestamp(3333) / 3_us != 1111)
            throw "`operator /` does not work correctly.";
    });

    $test("time/miscellaneous", []
    {
        // TODO
        /*
        // Functions

        timestamp now();

        void sleep(const timestamp &);
        void sleep(const interval &);

        // Ostream integration

        std :: ostream & operator << (std :: ostream &, const timestamp &);
        std :: ostream & operator << (std :: ostream &, const interval &);
        */
    });
};
