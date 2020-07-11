#pragma once
#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>

// simple vector algebra
// two operations(applied element wise) are defined:
//    -> v = v1 * v2
//    -> v = v1 + v2
//
// extra:
// SUM -> calculates sum of all elements on the vector
namespace pnc {

template <unsigned int TSize, typename TData = double>
class Vector {
private:
    TData* data;

public:
    static constexpr unsigned int size = TSize;
    using data_type = TData;

    Vector()
        : data(new TData[size] {})
    {
    }

    template<typename ...TArgs>
    Vector(TArgs... args)
        : data(new TData[TSize]{args...})
    {
    }

    Vector(TData init[size])
        : data(init)
    {
    }

    ~Vector()
    {
        delete[] data;
    }

    TData operator[](unsigned int index) const
    {
        return data[index];
    }

    TData& operator[](unsigned int index)
    {
        return data[index];
    }

    Vector(Vector&& other)
        : data(other.data)
    {
        other.data = {};
    }

    template <typename TVecLike>
    Vector<size, TData>& operator=(const TVecLike& other)
    {
        for (unsigned int i = 0; i < size; i++) {
            data[i] = other[i];
        }

        return *this;
    }
};

template <typename TVec>
auto SUM(const TVec& vec)
{
    auto cache = vec[0];
    for (unsigned int i = 1; i < vec.size; i++) {
        cache += vec[i];
    }
    return cache;
}

template <typename TVec>
auto MAX(const TVec& vec)
{
    auto cache = vec[0];
    for (unsigned int i = 1; i < vec.size; i++) {
        if (cache < vec[i]) {
            cache = vec[i];
        }
    }
    return cache;
}

template <typename TVec>
auto MAX(const TVec left, const TVec right)
{
    return VectorMax(left, right);
}

template <typename TVector>
class VectorNegative {
private:
    const TVector& v;

public:
    static constexpr unsigned int size = TVector::size;
    using data_type = decltype(v[0]);

    VectorNegative(const TVector& v)
        : v(v)
    {
    }

    auto operator[](unsigned int index) const
    {
        return -v[index];
    }
};

template <typename TVector, typename TConstant,typename TOperator>
class VectorConstantOperation {
private:
    const TVector& v;
    const TConstant c;

public:
    static constexpr unsigned int size = TVector::size;
    using data_type = typename TVector::data_type;
    TOperator op;

    VectorConstantOperation(
        const TVector& v,
        const TConstant c,
        TOperator op)
        : v(v)
        , c(c)
        , op(op)
    {
    }

    auto operator[](unsigned int index) const
    {
        return op(v[index] , c);
    }
};

template <typename TLeft, typename TRight>
class VectorSum {
private:
    const TLeft& v1;
    const TRight& v2;

public:
    static constexpr unsigned int size = TLeft::size;
    using data_type = decltype(v1[0]);

    VectorSum(
        const TLeft& v1,
        const TRight& v2)
        : v1(v1)
        , v2(v2)
    {
    }

    auto operator[](unsigned int index) const
    {
        return v1[index] + v2[index];
    }
};

template <typename TLeft, typename TRight>
class VectorProd {
private:
    const TLeft& v1;
    const TRight& v2;

public:
    static constexpr unsigned int size = TLeft::size;
    using data_type = decltype(v1[0]);

    VectorProd(
        const TLeft& v1,
        const TRight& v2)
        : v1(v1)
        , v2(v2)
    {
    }

    auto operator[](unsigned int index) const
    {
        return v1[index] * v2[index];
    }
};

template <typename TLeft, typename TRight>
class VectorMax {
private:
    const TLeft& v1;
    const TRight& v2;

public:
    static constexpr unsigned int size = TLeft::size;
    using data_type = decltype(v1[0]);

    VectorMax(
        const TLeft& v1,
        const TRight& v2)
        : v1(v1)
        , v2(v2)
    {
    }

    auto operator[](unsigned int index) const
    {
        return v1[index] < v2[index]
            ? v2[index]
            : v1[index];
    }
};

template <typename TVec>
VectorNegative<TVec> operator-(const TVec& vec)
{
    return VectorNegative<TVec>(vec);
}

template <typename TVector, typename TMaybeData>
struct IsDataOfVectorKind {
    static constexpr bool value = std::is_same<typename TVector::data_type, TMaybeData>::value;
};

template<typename TData>
struct MulOperation{
    auto operator()(TData x,TData y) const
    {
        return x * y;
    }
};

template<typename TData>
struct AddOperation{
    auto operator()(TData x,TData y) const
    {
        return x + y;
    }
};

// vector + constant
template <
    typename TLeft,
    typename TRight,
    typename = typename TLeft::data_type,
    typename = typename std::enable_if<IsDataOfVectorKind<TLeft, TRight>::value>::type>
auto operator+(
    const TLeft& left,
    const TRight right)
{
    using data_type = typename TLeft::data_type;
    return VectorConstantOperation<TLeft,TRight,AddOperation<data_type>>
        (left,right,AddOperation<data_type>());
}

// constant + vector
template <
    typename TLeft,
    typename TRight,
    typename = typename TRight::data_type,
    typename = typename std::enable_if<IsDataOfVectorKind<TRight, TLeft>::value>::type>
auto operator+(
    const TLeft left,
    const TRight& right)
{
    using data_type = typename TRight::data_type;
    return VectorConstantOperation<TRight,TLeft,AddOperation<data_type>>
        (right,left,AddOperation<data_type>());
}

// vector * constant
template <
    typename TLeft,
    typename TRight,
    typename = typename TLeft::data_type,
    typename = typename std::enable_if<IsDataOfVectorKind<TLeft, TRight>::value>::type>
auto operator*(
    const TLeft& left,
    const TRight right)
{
    using data_type = typename TLeft::data_type;
    return VectorConstantOperation<TLeft,TRight,MulOperation<data_type>>
        (left,right,MulOperation<data_type>());
}

// constant * vector
template <
    typename TLeft,
    typename TRight,
    typename = typename TRight::data_type,
    typename = typename std::enable_if<IsDataOfVectorKind<TRight, TLeft>::value>::type>
auto operator*(
    const TLeft left,
    const TRight& right)
{
    using data_type = typename TRight::data_type;
    return VectorConstantOperation<TRight,TLeft,MulOperation<data_type>>
        (right,left,MulOperation<data_type>());
}

template <
    typename TLeft,
    typename TRight>
auto operator-(
    const TLeft& left,
    const TRight& right)
{
    return left + (-right);
}

template <
    typename TLeft,
    typename TRight,
    typename typeL = typename TLeft::data_type,
    typename typeR = typename TRight::data_type,
    typename = typename std::enable_if<std::is_same<typeL, typeR>::value>::type,
    unsigned int sizeL = TLeft::size,
    unsigned int sizeR = TRight::size,
    typename = typename std::enable_if<sizeL==sizeR>::type
    >
auto operator+(
    const TLeft& left,
    const TRight& right)
{
    return VectorSum<TLeft, TRight>(left, right);
}

template <
    typename TLeft,
    typename TRight,
    typename typeL = typename TLeft::data_type,
    typename typeR = typename TRight::data_type,
    typename = typename std::enable_if<std::is_same<typeL, typeR>::value>::type,
    unsigned int sizeL = TLeft::size,
    unsigned int sizeR = TRight::size,
    typename = typename std::enable_if<sizeL==sizeR>::type
    >
auto operator*(
    const TLeft& left,
    const TRight& right)
{
    return VectorProd<TLeft, TRight>(left, right);
}

class ToVector {
};

template <typename TLeft>
auto operator|(
    const TLeft& left,
    const ToVector& right)
{
    auto vec = Vector<TLeft::size, decltype(left[0])>();
    for (unsigned int i = 0; i < TLeft::size; i++) {
        vec[i] = left[i];
    }

    return vec;
}

template <typename TLeft, typename TRight>
class VectorComponentMax {
private:
    const TLeft& v1;
    const TRight& v2;

public:
    static constexpr unsigned int size = TLeft::size;
    using data_type = decltype(v1[0]);

    VectorComponentMax(
        const TLeft& v1,
        const TRight& v2)
        : v1(v1)
        , v2(v2)
    {
    }

    auto operator[](unsigned int index) const
    {
        return v1[index] > v2[index] ? v1[index] : v2[index];
    }
};

template <typename TLeft, typename TRight>
auto ComponentWiseMax(
    const TLeft& left,
    const TRight& right)
{
    return VectorComponentMax<TLeft, TRight>(left, right);
}

template <typename TVector>
auto Norm(TVector& v)
{
    return SUM(v * v);
}

}
