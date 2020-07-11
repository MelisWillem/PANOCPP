// Seems like this is not possible:
#include <iostream>

template <typename TData,int SIZE>
class Vector {
public:
    using data_type = TData;
    static constexpr int size = SIZE;
};

template <typename TVector, typename TMaybeData>
struct IsDataOfVectorKind {
    static constexpr bool value = std::is_same<typename TVector::data_type, TMaybeData>::value;
};

template <
    typename TLeft,
    typename TRight,
    typename = typename TLeft::data_type,
    typename = typename std::enable_if<IsDataOfVectorKind<TLeft,TRight>::value>::type
    >
void test(TLeft v, TRight)
{
    std::cout << "test1" << std::endl;
}

template <
    typename TLeft,
    typename TRight,
    typename = typename TLeft::data_type,
    typename  = typename TRight::data_type,
    int ls = TLeft::size,
    int rs = TRight::size,
    typename = typename std::enable_if<ls==rs>::type
    >
void test(TLeft v, TRight)
{
    std::cout << "test2" << std::endl;
}

int main()
{
    Vector<int,2> x;
    Vector<int,2> y;

    test(x, int(2));    // should print test1

    test(x,y);
}
