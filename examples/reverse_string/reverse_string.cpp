#include "rebind.h"

#include <iostream>
#include <tuple>

using namespace rebind;

// Integral constant wrapper for char
template <char C>
using Char = std::integral_constant<char, C>;

// Implementation of ToCString, below
template <typename... Args>
struct ToCStringImpl
{
    static constexpr const char value[] = {Args::value..., '\0'};

    constexpr /* implicit */ operator const char* () const
    {
        return value;
    }
};

// Storage for ToCStringImpl
template <typename... Args>
constexpr const char ToCStringImpl<Args...>::value[];

// Convert a sequence of integral constants to a C-string
template <typename Arguments>
using ToCString = Rebind<Arguments, ToCStringImpl>;

// Implementation of StringToType, below
template <const char *S, int offset=0, typename=void>
struct StringToTypeImpl
{
    using C = Char<S[offset]>;

    using type = Join<std::tuple<C>,
                      typename StringToTypeImpl<S, offset+1>::type>;
};

// Terminator for StringToTypeImpl
template <const char *S,
          int offset>
struct StringToTypeImpl<S,
                        offset,
                        typename std::enable_if<S[offset] == 0>::type>
{
    using type = std::tuple<>;
};

// Convert a C-string to a std::tuple of integral constants.
template <const char *S>
using StringToType = typename StringToTypeImpl<S>::type;

// Demo

constexpr char hello[] = "Hello, world!";

int main()
{
    std::cout << ToCString<Reverse<StringToType<hello>>>() << std::endl;
}
