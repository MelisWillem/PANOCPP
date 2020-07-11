// Seems like this is not possible:
// https://bugsdb.com/_en/debug/15a3b904c2e060eb02477ceb7535e519 
#include <iostream>

template <typename TData>
class Vector {
public:
    using data_type = TData;
};

template <typename TVector, typename TMaybeData>
struct IsDataOfVectorKind {
    static constexpr bool value = std::is_same<typename TVector::data_type, TMaybeData>::value;
};

template <
    typename TLeft,
    typename TRight,
    typename B = typename TLeft::data_type,
    typename = typename std::enable_if<IsDataOfVectorKind<TLeft,TRight>::value>::type
    >
void test(TLeft v, TRight)
{
    std::cout << "test1" << std::endl;
}

template <
    typename TLeft,
    typename TRight,
    typename = typename std::enable_if<!IsDataOfVectorKind<TLeft,TRight>::value>::type
    >
void test(TLeft v, TRight)
{
    std::cout << "test2" << std::endl;
}

int main()
{
    Vector<int> x;

    test(x, int(2));    // should print test1
    test(x, double(2)); // should print test2
}
