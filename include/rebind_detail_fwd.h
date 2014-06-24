#ifndef REBIND_REBIND_DETAIL_FWD_H
#define REBIND_REBIND_DETAIL_FWD_H

#include <type_traits>

namespace rebind { namespace detail {

// Helper struct to carry arguments around when we don't have a preferred
// container
template <typename... Args> struct Arguments;

template <typename T>
struct RebindImpl;

template <typename ConcreteArguments, typename Arg>
struct AppendImpl;

template <typename ConcreteArguments1, typename ConcreteArguments2>
struct JoinImpl;

template <typename T>
struct ReverseImpl;

template <typename ConcreteArguments,
          template <class> class Transformer>
struct TransformImpl;

template <typename Arguments,
          typename Initial,
          template <class, class> class BinaryOperation>
struct AccumulateImpl;

template <typename ConcreteArgs, typename Needle>
struct ContainsImpl;

template <std::size_t N,
          typename ConcreteArguments,
          template <class ...> class Action>
struct RepeatImpl;

template <typename Arg, typename... Args>
using FirstImpl = Arg;

template <typename T>
struct DropFirstImpl;

template <typename ConcreteArguments>
struct RebindArgsImpl;

template <std::size_t N>
struct NthImpl;

template <typename... Args>
struct SizeImpl;

} } // namespace rebind::detail

#endif
