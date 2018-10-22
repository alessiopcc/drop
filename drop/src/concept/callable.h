// Forward declarations

namespace drop
{
    template <typename, typename...> class callable;
};

#if !defined(__forward__) && !defined(__src__concept__callable__h)
#define __src__concept__callable__h

// Includes

#include <type_traits>

namespace drop
{
    template <typename functor, typename... arguments> class callable
    {
        // SFINAE

        template <typename ftype, void (ftype :: *)(arguments...)> struct msfinae {};
        template <typename ftype, void (ftype :: *)(arguments...) const> struct csfinae {};

        template <typename ftype> static std :: true_type mtest(msfinae <ftype, &ftype :: operator ()> *);
        template <typename ftype> static std :: false_type mtest(...);

        template <typename ftype> static std :: true_type ctest(csfinae <ftype, &ftype :: operator ()> *);
        template <typename ftype> static std :: false_type ctest(...);

        typedef std :: decay_t <functor> functbase;
        static constexpr bool constfunct = std :: is_const <std :: remove_reference_t <functor>> :: value;

        static constexpr bool mvalue = std :: is_same <decltype(mtest <functbase> (nullptr)), std :: true_type> :: value || std :: is_same <decltype(ctest <functbase> (nullptr)), std :: true_type> :: value;
        static constexpr bool cvalue = std :: is_same <decltype(ctest <functbase> (nullptr)), std :: true_type> :: value;

    public:

        // Static members

        static constexpr bool value = constfunct ? cvalue : mvalue;
    };
};

#endif
