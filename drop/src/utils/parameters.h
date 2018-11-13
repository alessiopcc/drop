// Forward declarations

namespace drop
{
    namespace parameters
    {
        template <typename, typename> struct concat;
        template <typename left, typename right> using concat_t = typename concat <left, right> :: type;

        template <typename> struct reverse;
        template <typename pack> using reverse_t = typename reverse <pack> :: type;

        template <typename> struct frontstrip;
        template <typename pack> using frontstrip_t = typename frontstrip <pack> :: type;

        template <typename> struct backstrip;
        template <typename pack> using backstrip_t = typename backstrip <pack> :: type;
    };
};

#if !defined(__forward__) && !defined(__src__utils__parameters__h)
#define __src__utils__parameters__h

namespace drop
{
    namespace parameters
    {
        // Classes

        template <template <typename...> typename pack, typename... left, typename... right> struct concat <pack <left...>, pack <right...>>
        {
            typedef pack <left..., right...> type;
        };

        template <template <typename...> typename pack> struct reverse <pack <>>
        {
            typedef pack <> type;
        };

        template <template <typename...> typename pack, typename arg, typename... args> struct reverse <pack <arg, args...>>
        {
            typedef concat_t <reverse_t <pack <args...>>, pack <arg>> type;
        };

        template <template <typename...> typename pack, typename arg, typename... args> struct frontstrip <pack <arg, args...>>
        {
            typedef pack <args...> type;
        };

        template <template <typename...> typename pack, typename arg, typename... args> struct backstrip <pack <arg, args...>>
        {
            typedef reverse_t <frontstrip_t <reverse_t <pack <arg, args...>>>> type;
        };

        // Functions

        template <size_t times, typename type, typename lambda> void repeat(type &&, lambda &&);
    };
};

#endif
