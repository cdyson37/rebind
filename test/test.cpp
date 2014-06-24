#include "rebind.h"

#include <type_traits>
#include <tuple>

using namespace rebind;

template <typename... Args> struct TestArgs { };

////////////////////////////////////////////////////////////////////////////////
// Test Rebind
////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same<Rebind<std::tuple<int, double>, TestArgs>,
                           TestArgs<int, double>>(), "");

static_assert(std::is_same<Rebind<std::pair<int, double>, TestArgs>,
                           TestArgs<int, double>>(), "");

////////////////////////////////////////////////////////////////////////////////
// Test RebindArgs
////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same< RebindArgs<int, double, TestArgs<char>>,
                            TestArgs<int, double> >(), "");

////////////////////////////////////////////////////////////////////////////////
// Test Append
////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same<Append<TestArgs<>, int>,
                           TestArgs<int>>(), "");

static_assert(std::is_same<Append<TestArgs<int>, double>,
                           TestArgs<int, double>>(), "");

static_assert(std::is_same<Append<std::tuple<int>, double>,
                           std::tuple<int, double>>(), "");

////////////////////////////////////////////////////////////////////////////////
// Test First and Last
////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same<First<TestArgs<int, double>>, int>(), "");
static_assert(std::is_same<Last<TestArgs<int, double>>, double>(), "");

static_assert(std::is_same<First<std::tuple<int, double>>, int>(), "");
static_assert(std::is_same<Last<std::tuple<int, double>>, double>(), "");

////////////////////////////////////////////////////////////////////////////////
// Test Reverse
////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same<Reverse<TestArgs<>>,
                           TestArgs<>>(), "");

static_assert(std::is_same<Reverse<TestArgs<int>>,
                           TestArgs<int>>(), "");

static_assert(std::is_same<Reverse<TestArgs<int, double>>,
                           TestArgs<double, int>>(), "");

static_assert(std::is_same<Reverse<TestArgs<int, double, float>>,
                           TestArgs<float, double, int>>(), "");

static_assert(std::is_same<Reverse<std::tuple<int, double, float>>,
                           std::tuple<float, double, int>>(), "");

////////////////////////////////////////////////////////////////////////////////
// Test Join
////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same< Join<TestArgs<int, double>, TestArgs<float>>,
                            TestArgs<int, double, float> >(), "");

static_assert(std::is_same< Join<std::tuple<int, double>, std::tuple<float>>,
                            std::tuple<int, double, float> >(), "");

////////////////////////////////////////////////////////////////////////////////
// Test TransformAll
////////////////////////////////////////////////////////////////////////////////

namespace {

template <typename... T>
using Transformer = Reverse<std::tuple<T...>>;

using X = TransformAll<TestArgs<int, double>, Transformer>;

static_assert(std::is_same<X, TestArgs<double, int>>(), "");

}

////////////////////////////////////////////////////////////////////////////////
// Test TransformEach
////////////////////////////////////////////////////////////////////////////////

template <typename T>
using Ident = T;

template <typename T>
struct BoolToCharImpl { using type = T; };

template <>
struct BoolToCharImpl<bool> { using type = char; };

template <typename T>
using BoolToChar = typename BoolToCharImpl<T>::type;

static_assert(std::is_same<TransformEach<TestArgs<int, double>, Ident>,
                           TestArgs<int, double>>(), "");

static_assert(std::is_same<TransformEach<TestArgs<int, bool>, BoolToChar>,
                           TestArgs<int, char>>(), "");

////////////////////////////////////////////////////////////////////////////////
// Test DropFirst
////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same<DropFirst<TestArgs<int, double, bool>>,
                           TestArgs<double, bool>>(), "");

static_assert(std::is_same<DropFirst<TestArgs<double, bool>>,
                           TestArgs<bool>>(), "");

static_assert(std::is_same<DropFirst<TestArgs<bool>>,
                           TestArgs<>>(), "");

////////////////////////////////////////////////////////////////////////////////
// Test DropLast
////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same<DropLast<TestArgs<int, double, bool>>,
                           TestArgs<int, double>>(), "");

static_assert(std::is_same<DropLast<TestArgs<int, double>>,
                           TestArgs<int>>(), "");

static_assert(std::is_same<DropLast<TestArgs<int>>,
                           TestArgs<>>(), "");

////////////////////////////////////////////////////////////////////////////////
// Test Nth
////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same< Nth<0, TestArgs<char>>,
                            char >(), "");

static_assert(std::is_same< Nth<0, TestArgs<char, int>>,
                            char >(), "");

static_assert(std::is_same< Nth<1, TestArgs<char, int>>,
                            int >(), "");

static_assert(std::is_same< Nth<0, TestArgs<char, int, double>>,
                            char >(), "");

static_assert(std::is_same< Nth<1, TestArgs<char, int, double>>,
                            int >(), "");

static_assert(std::is_same< Nth<2, TestArgs<char, int, double>>,
                            double >(), "");

////////////////////////////////////////////////////////////////////////////////
// Test Size
////////////////////////////////////////////////////////////////////////////////

static_assert(Size< TestArgs<> >() == 0, "");
static_assert(Size< TestArgs<int> >() == 1, "");
static_assert(Size< TestArgs<int, double> >() == 2, "");

////////////////////////////////////////////////////////////////////////////////
// Test Accumulate
////////////////////////////////////////////////////////////////////////////////

template <typename IC1, typename IC2>
using Add = std::integral_constant<typename IC1::value_type,
                                   IC1::value + IC2::value>;

static_assert( Accumulate< TestArgs<>,
                           std::integral_constant<int, 1>,
                           Add>::value == 1, "");

static_assert( Accumulate< TestArgs< std::integral_constant<int, 0> >,
                           std::integral_constant<int, 1>,
                           Add>::value == 1, "");

static_assert( Accumulate< TestArgs< std::integral_constant<int, 0>,
                                     std::integral_constant<int, 1> >,
                           std::integral_constant<int, 0>,
                           Add>::value == 1, "");

static_assert( Accumulate< TestArgs< std::integral_constant<int, 0>,
                                     std::integral_constant<int, 1>,
                                     std::integral_constant<int, 2> >,
                           std::integral_constant<int, 0>,
                           Add>::value == 3, "");

////////////////////////////////////////////////////////////////////////////////
// Test Contains
////////////////////////////////////////////////////////////////////////////////

static_assert(!Contains<TestArgs<>, int>(), "");
static_assert(!Contains<TestArgs<double>, int>(), "");
static_assert(!Contains<TestArgs<double, float>, int>(), "");
static_assert(Contains<TestArgs<int>, int>(), "");
static_assert(Contains<TestArgs<int, float>, int>(), "");

////////////////////////////////////////////////////////////////////////////////
// Test Repeat
////////////////////////////////////////////////////////////////////////////////

static_assert(std::is_same<Repeat<0, TestArgs<int, double>, DropFirst>,
                           TestArgs<int, double>>(), "");
static_assert(std::is_same<Repeat<1, TestArgs<int, double>, DropFirst>,
                           TestArgs<double>>(), "");
static_assert(std::is_same<Repeat<2, TestArgs<int, double>, DropFirst>,
                           TestArgs<>>(), "");

////////////////////////////////////////////////////////////////////////////////
// Test Any
////////////////////////////////////////////////////////////////////////////////

template <typename T>
using IsIntegral = typename std::is_integral<T>::type;

static_assert(!Any<TestArgs<>, IsIntegral>(), "");
static_assert(Any<TestArgs<int, double>, IsIntegral>(), "");
static_assert(!Any<TestArgs<float, double>, IsIntegral>(), "");

////////////////////////////////////////////////////////////////////////////////
// Main function
////////////////////////////////////////////////////////////////////////////////

int main() { }
