#ifndef REBIND_REBIND_H
#define REBIND_REBIND_H

#include "rebind_detail_fwd.h"

namespace rebind {

/// Apply the template arguments of the first arguemnt to the primary template
/// given by the second argument.
/// E.g. Rebind<boost::tuple<int, double>, std::tuple>
///        -> std::tuple<int, double>
template <typename OldTemplate, template <class...> class NewTemplate>
using Rebind = typename detail::RebindImpl<OldTemplate>
                 ::template Rebind<NewTemplate>;

/// Apply the template arguments of the first argument to the template instance
/// given by the second argument.
/// E.g. RebindConcrete<boost::tuple<int, double>, std::tuple<float>>
///        -> std::tuple<int, double>
template <typename OldTemplate, class NewTemplate>
using RebindConcrete = typename detail::RebindImpl<OldTemplate>
                         ::template RebindConcrete<NewTemplate>::type;

/// Rebind explicitly listed arguments onto an existing template instantiation.
/// E.g. RebindArgs<int, double, std::tuple<char>>
///        -> std::tuple<int, double>
template <typename... Args>
using RebindArgs
  = typename detail::RebindArgsImpl<detail::Arguments<Args...>>::type;

/// E.g. Join<std::tuple<int>, boost::tuple<double>>
///        -> std::tuple<int, double>
template <typename Arguments1, typename Arguments2>
using Join = typename detail::JoinImpl<Arguments1, Arguments2>::type;

/// Append a new type to the given concrete argument vector.
/// E.g. Append<std::tuple<int>, double>
///        -> std::tuple<int, double>
// TODO: Support appending more than one argument?
template <typename ConcreteArguments, typename Arg>
using Append = Join<ConcreteArguments, detail::Arguments<Arg>>;

/// E.g. Reverse<std::tuple<int, double>>
///        -> std::tuple<double, int>
template <typename Arguments>
using Reverse = typename detail::ReverseImpl<Arguments>::type;

/// E.g. First<std::tuple<int, double>> -> int
template <typename Arguments>
using First = Rebind<Arguments, detail::FirstImpl>;

/// E.g. Last<std::tuple<int, double>> -> double
template <typename Arguments>
using Last = First<Reverse<Arguments>>;

/// Rebind all types in ConcreteArguments onto Transformer (which is expected to
/// be a "using") then rebind them back onto ConcreteArguments' primary template
template <typename ConcreteArguments, template <class...> class Transformer>
using TransformAll
  = RebindConcrete<Rebind<ConcreteArguments, Transformer>, ConcreteArguments>;

/// Apply Transformer to each element in Arguments
/// E.g. TransformEach<std::tuple<int, bool>, BoolToChar>
///        -> std::tuple<int, char>
template <typename Arguments, template <class> class Transformer>
using TransformEach
  = typename detail::TransformImpl<Arguments, Transformer>::type;

/// Accumulate Arguments as (((Initial x FirstArg) x SecondArg) x ...) where 'x'
/// represents BinaryOperation and Initial is a starting type, as per
/// std::accumulate.
template <typename Arguments,
          typename Initial,
          template <class, class> class BinaryOperation>
using Accumulate
  = typename detail::AccumulateImpl<Arguments, Initial, BinaryOperation>::type;

/// Evaluates to std::true_type iff Needle is found in the template arguments of
/// ConcreteArgs.
/// E.g. Contains<std::tuple<int, double>, int> -> std::true_type
template <typename ConcreteArgs, typename Needle>
using Contains = typename detail::ContainsImpl<ConcreteArgs, Needle>::type;

/// Apply Action to ConcreteArguments N times.
/// E.g. Repeat<2, std::tuple<int, char, double>, DropFirst>
///        -> std::tuple<double>
template <std::size_t N,
          typename ConcreteArguments,
          template <class ...> class Action>
using Repeat = typename detail::RepeatImpl<N, ConcreteArguments, Action>::Impl;

/// Remove the first item from the sequence.
/// E.g. DropFirst<std::tuple<int, bool>> -> std::tuple<bool>
template <typename Arguments>
using DropFirst = typename detail::DropFirstImpl<Arguments>::type;

/// Remove the last item from the sequence.
/// E.g. DropLast<std::tuple<int, bool>> -> std::tuple<int>
template <typename Arguments>
using DropLast = Reverse<DropFirst<Reverse<Arguments>>>;

/// Get the Nth type from the sequence
/// E.g. Nth<1, std::tuple<char, int, double>>
///        -> int
template <std::size_t N, typename ConcreteArguments>
using Nth = typename Rebind<ConcreteArguments,
                            detail::NthImpl<N>::template Impl>::type;

/// Get the number of elements in the sequence, as
/// std::integral_constant<std::size_t, N> for some N
/// E.g. Size<std::tuple<char, int, double>>() == 3
template <typename ConcreteArguments>
using Size = std::integral_constant<std::size_t,
               Rebind<ConcreteArguments, detail::SizeImpl>::value>;

/// Return as an integral constant the logical OR of two integral constants
template <typename IC1, typename IC2>
using LogicalOr = std::integral_constant<typename IC1::value_type,
                                         IC1::value || IC2::value>;

/// Return std::true_type or std::false_type depending on whether any of the
/// Arguments satisfy Pred.
template <typename Arguments, template <class> class Pred>
using Any = Accumulate<TransformEach<Arguments, Pred>,
                       std::false_type,
                       LogicalOr>;

} // namespace rebind

#include "rebind_detail.h"
#endif
