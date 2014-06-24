#ifndef REBIND_REBIND_DETAIL_H
#define REBIND_REBIND_DETAIL_H

namespace rebind { namespace detail {

template <template <class...> class Template, typename... Args>
struct RebindImpl<Template<Args...>>
{
    template <template <class...> class Template2>
    using Rebind = Template2<Args...>;

    template <typename T>
    struct RebindConcrete;

    template <template <class...> class Template2, typename... Args2>
    struct RebindConcrete<Template2<Args2...>>
    {
        using type = Template2<Args...>;
    };
};

template <typename ConcreteArguments1, typename ConcreteArguments2>
struct JoinImpl
{
    template <typename T>
    struct S1;

    template <template <class...> class Template, typename... Args1>
    struct S1<Template<Args1...>>
    {
        template <typename... Args2>
        struct S2
        {
          using type = Template<Args1..., Args2...>;
        };
    };

    using tmp = S1<ConcreteArguments1>;
    using type = typename Rebind<ConcreteArguments2, tmp::template S2>::type;
};

template <typename ConcreteArguments>
struct ReverseImpl
{
private:
    // Save the first element of ConcreteArguments
    using FirstArg = First<ConcreteArguments>;

    // Reverse the remaining arguments of ConcreteArguments
    using ReversedTail = Reverse<DropFirst<ConcreteArguments>>;

public:
    // Let type = ReversedTail + FirstArg
    using type = Join<ReversedTail, Arguments<FirstArg>>;
};

template <template <class...> class Template>
struct ReverseImpl<Template<>>
{
    using type = Template<>;
};

template <typename ConcreteArguments,
          template <class> class Transformer>
struct TransformImpl
{
    template <typename... Args>
    using Impl = Arguments<Transformer<Args>...>;

    using type = TransformAll<ConcreteArguments, Impl>;
};

template <typename Arguments,
          typename Initial,
          template <class, class> class BinaryOperation>
struct AccumulateImpl
{
    using tmp = BinaryOperation<Initial, First<Arguments>>;
    using type = typename AccumulateImpl<DropFirst<Arguments>,
                                         tmp,
                                         BinaryOperation>::type;
};

template <template <class, class> class BinaryOperation,
          typename Initial,
          template <class...> class Arguments>
struct AccumulateImpl<Arguments<>, Initial, BinaryOperation>
{
    using type = Initial;
};

template <typename ConcreteArgs, typename Needle>
struct ContainsImpl
{
    template <typename T>
    using Pred = typename std::is_same<Needle, T>::type;

    using type = Any<ConcreteArgs, Pred>;
};

template <std::size_t N,
          typename ConcreteArguments,
          template <class ...> class Action>
struct RepeatImpl
{
private:
    using Applied = Action<ConcreteArguments>;

public:
    using Impl = typename RepeatImpl<N-1, Applied, Action>::Impl;
};

template <typename ConcreteArguments,
          template <class ...> class Action>
struct RepeatImpl<0, ConcreteArguments, Action>
{
    using Impl = ConcreteArguments;
};

template <template <class...> class Template, typename Arg, typename... Args>
struct DropFirstImpl<Template<Arg, Args...>>
{
    using type = Template<Args...>;
};

template <typename ConcreteArguments>
struct RebindArgsImpl
{
    using ArgList = DropLast<ConcreteArguments>;
    using Template = Last<ConcreteArguments>;
    using type = RebindConcrete<ArgList, Template>;
};

template <std::size_t N>
struct NthImpl
{
    template <typename Arg, typename... Args>
    struct Impl
    {
        using type = typename NthImpl<N-1>::template Impl<Args...>::type;
    };
};

template <>
struct NthImpl<0>
{
    template <typename Arg, typename... Args>
    struct Impl
    {
        using type = Arg;
    };
};

template <typename... Args>
struct SizeImpl
{
    enum { value = sizeof...(Args) };
};

} } // namespace rebind::detail

#endif
