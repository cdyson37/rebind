#include "rebind.h"

#include <tuple>
#include <type_traits>
#include <type_traits>

using namespace rebind;

namespace {

using Pair = std::pair<int, double>;
namespace {
using Tuple = std::tuple<typename Pair::first_type,
                              typename Pair::second_type>;
static_assert(std::is_same<Tuple, std::tuple<int, double>>(), "");
}
namespace {
using Tuple = Rebind<Pair, std::tuple>;
static_assert(std::is_same<Tuple, std::tuple<int, double>>(), "");
}

}

namespace {

template <typename A, typename B> struct MyStruct;
using X = std::tuple<int, double>;
using Y = Rebind<X, MyStruct>;
static_assert(std::is_same<Y, MyStruct<int, double>>(), "");

}

namespace {

using T1 = std::tuple<int, double>;
using T2 = std::tuple<char>;
using J = Join<T1, T2>;
static_assert(std::is_same<J, std::tuple<int, double, char>>(), "");

}

namespace {
using A = std::allocator<char>;
using A2 = A::rebind<int>::other;
using A3 = RebindArgs<int, A>;
static_assert(std::is_same<A2, std::allocator<int>>(), "");
static_assert(std::is_same<A3, std::allocator<int>>(), "");
}

int main()
{

}
