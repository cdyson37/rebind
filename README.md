Rebind
======

Rebind is a C++ library for manipulating containers of types, rather like `boost::mpl`. It requires C++11 support. Unlike traditional meta-programming libraries, it aims to be agnostic about type containers. Rather than have an equivalent of `boost::mpl::vector`, it instead allows any template to act as a container of zero or more types. It aims to be template container agnostic in the same way that the STL is value container agnostic.

In rebind, you can store types in any class you want e.g. `std::tuple` or an empty struct:

```cpp
template <typename... Args>
struct Container { };
```

`rebind` itself is a takes the template arguments from one instantiation and applies them to a different template - for example:

```cpp
#include "rebind.h"
using namespace rebind;
using Pair = std::pair<int, double>;
using C = Rebind<Pair, Container>; // = Container<int, double>
```

Similarly, you could rebind the template arguments of a `std::tuple<>` to a `boost::mpl::vector<>` or any arbitrary type. This can be useful for storing template arguments and later applying them to an ordinary class:

```cpp
template <typename A, typename B> struct MyStruct;
using X = std::tuple<int, double>;
using Y = Rebind<X, MyStruct>; // = MyStruct<int, double>
```

From `rebind` itself, it's possible to build a wide variety of useful algorithms that operate on templates as containers-of-types. For example, one can concatenate the arguments of two different templates:

```cpp
using T1 = std::tuple<int, double>;
using T2 = std::tuple<char>;
using J = Join<T1, T2>; // = std::tuple<int, double, char>
```

Here are a few others:

 - ``Join``: concatenate the arguments of two templates
 - ``Append``: add a type to the end of a template's argument list
 - ``Reverse``: reverse the order of a template's arguments
 - ``First`` / ``Last``: get the first and last template arguments
 - ``DropFirst`` / ``DropLast``: remove the first and last template arguments
 - ``TransformAll`` / ``TransformEach``: apply a transformation to the template arguments, either all at once or individually
 - ``Accumulate``: as per STL: use an operation to `sum' the template arguments, using a given initial value and operation
 - ``Contains``: test if a type is within the argument list
 - ``Repeat``: repeat the same _action_ on a list of template arguments, where action is a template
 - ``Nth``: get the Nth parameter in an argument list
 - ``Size``: get the number of arguments in a template's argument list
 - ``LogicalOr``: _OR_ together two ``std::integral_constant``s
 - ``Any``: establish if a given predicate is true of any type in a template's argument list

Naming
------

All STL allocators have a nested type called `rebind` such that `typename SomeAllocator::template rebind<int>::other` is the same as `SomeAllocator<int>`. This was the inspiration for `rebind`ing the arguments of one template onto another.

Examples
--------

See `examples/reverse_string` for a party-trick example of string reversal at compile time.

Building
--------

Rebind is header-only. Tests and examples can be built by running `./build.sh`, which currently requires CMake. This may change to SCons or another build system at some point to eliminate this dependency (contributions welcome!)

Reference
---------

The following algorithms are provided. Here `A` and `B` are templates, `T` and `U` are types. `O` is a `using` operation.

```cpp
Rebind<A<T...>, B> -> B<T...>

RebindConcrete<A<T...>, B<U...>> -> B<T...>

RebindArgs<T..., A<U...>> -> A<T...>

Join<A<T...>, B<U...>> -> A<T..., U...>

Append<A<T...>, U> -> A<T..., U>

Reverse<A<T...>> -> Reverse<A<U...>> // where U... are T... backwards

First<A<T...>> = U // Where U is the first of T...

Last<A<T...>> = U // Where U is the last of T...

TransformAll<A<T...>, B> -> Rebind<B<T...>, A>

TransformEach<A<T...>, B> -> A<B<T>...>

Accumulate<A<T...>, U, O> -> O(O(O(U, T1), T2), T3) // etc, where T... = T1, T2, T3

Contains<A<T...>, U> -> std::true_type or std::false_type depending on whether U is in T...

Repeat<N, A<T...>, O> -> A<U...> // where U = O<O<O<T...>>> (N types)

DropFirst<A<T, U...>> -> T<U...>

DropLast<A<T..., U>> -> A<T...>

Nth<N, A<T...>> -> U // where U is the Nth element in T...

Size<A<T...>> = N // where N is the size of T...

LogicalOr<C1, C2> = std::true_type or std::false_type depending on C1() || C2()

Any<A<T...>, P> = std::true_type or std::false_type depending on whether P<T> is true for at least one T in T...

```

Legal
-----

MIT licence. Please see `LICENCE` file.
