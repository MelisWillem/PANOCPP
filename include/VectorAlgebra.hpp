#pragma once
#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <stdlib.h>
#include <utility>

namespace pnc {

template <unsigned int TSize, typename TData = double>
class VectorUnit final {
private:
    TData constant;

public:
    static constexpr unsigned int size = TSize;
    using data_type = TData;

    explicit VectorUnit(TData constant): constant(constant)
    {}

    constexpr auto operator[](unsigned int index) const
    {
        return constant;
    }
};

template <unsigned int TSize, typename TData = double>
class Vector final{
private:
    TData* data;
    bool is_moved = false;

public:
    static constexpr unsigned int size = TSize;
    using data_type = TData;

    Vector()
        : data(new TData[size] {})
    {
    }

    template<typename ...TArgs,typename = typename std::enable_if_t<size==sizeof...(TArgs)>>
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

    constexpr TData operator[](unsigned int index) const
    {
        assert(!IsMoved());
        return data[index];
    }

    constexpr TData& operator[](unsigned int index)
    {
        assert(!IsMoved());
        return data[index];
    }

    Vector(Vector&& other)
        : data(other.data)
    {
        other.data = {};
        other.is_moved = true;
    }

    template <typename TVecLike>
    constexpr Vector<size, TData>& operator=(const TVecLike& other)
    {
        static_assert(
                size == TVecLike::size,
                "Trying to assign vector expression to a vector of a different dimension");
        assert(!IsMoved());
        for (unsigned int i = 0; i < size; i++) {
            data[i] = other[i];
        }

        return *this;
    }

    bool IsMoved() const 
    {
        return is_moved;
    }
};

template <typename TVector, typename TMaybeData>
struct IsDataOfVectorKind {
    static constexpr bool value = std::is_same<typename TVector::data_type, TMaybeData>::value;
};

template <
    typename TVec,
    unsigned int size = TVec::size
    >
constexpr auto SUM(const TVec& vec)
{
    static_assert(size> 0,"Sum of a vector is only defined if the dimension of the vector is larger then 0");
    auto cache = vec[0];
    for (unsigned int i = 1; i < vec.size; i++) {
        cache += vec[i];
    }
    return cache;
}

template <
    typename TVec,
    unsigned int size = TVec::size
    >
constexpr auto MAX(const TVec& vec)
{
    static_assert(size> 0,"Max of a vector is only defined if the dimension of the vector is larger then 0");
    auto cache = vec[0];
    for (unsigned int i = 1; i < vec.size; i++) {
        if (cache < vec[i]) {
            cache = vec[i];
        }
    }
    return cache;
}

template <
    typename TLeft,
    typename TRight,
    typename typeL = typename TLeft::data_type,
    typename typeR = typename TRight::data_type,
    typename = typename std::enable_if_t<std::is_same<typeL, typeR>::value>,
    unsigned int sizeL = TLeft::size,
    unsigned int sizeR = TRight::size,
    typename = typename std::enable_if_t<sizeL==sizeR>
    >
constexpr auto MAX(const TLeft left, const TRight right)
{
    return VectorMax(left, right);
}

template < 
    typename TVectorRef,
    typename TVector = typename std::remove_reference_t<TVectorRef>
    >
class VectorNegative final {
private:
    TVectorRef v;

public:
    static constexpr unsigned int size = TVector::size;
    using data_type = typename TVector::data_type;

    explicit VectorNegative(TVectorRef&& v)
        : v(std::forward<TVectorRef>(v))
    {
    }

    constexpr auto operator[](unsigned int index) const
    {
        return -v[index];
    }
};

template <
    typename TVectorRef,
    typename TConstant,
    typename TOperator,
    typename TVector = std::remove_reference_t<TVectorRef>
    >
class VectorConstantOperation final{
private:
    TVectorRef v;
    TConstant c;

public:
    static constexpr unsigned int size = TVector::size;
    using data_type = typename TVector::data_type;
    TOperator op;

    explicit VectorConstantOperation(
        TVectorRef&& v,
        TConstant c,
        TOperator op)
        : v(std::forward<TVectorRef>(v))
        , c(c)
        , op(op)
    {
    }

    constexpr auto operator[](unsigned int index) const
    {
        return op(v[index] , c);
    }
};

template <typename TLeft, typename TRight>
class VectorSum final{
private:
    const TLeft v1;
    const TRight v2;

public:
    static constexpr unsigned int size = std::remove_reference_t<TLeft>::size;
    using data_type = typename std::remove_reference_t<TLeft>::data_type;

    explicit VectorSum(
        TLeft&& left,
        TRight&& right)
        : v1(std::forward<TLeft>(left))
        , v2(std::forward<TRight>(right))
    {
    }

    constexpr auto operator[](unsigned int index) const
    {
        return v1[index] + v2[index];
    }
};

template <
    typename TLeftRef,
    typename TRightRef,
    typename TLeft = typename std::remove_reference_t<TLeftRef>,
    typename TRight = typename std::remove_reference_t<TRightRef>
    >
class VectorProd final {
private:
    TLeftRef v1;
    TRightRef v2;

public:
    static constexpr unsigned int size = TLeft::size;
    using data_type = typename TLeft::data_type;

    explicit VectorProd(
        TLeftRef&& left,
        TRightRef&& right)
        : v1(std::forward<TLeftRef>(left))
        , v2(std::forward<TRightRef>(right))
    {
    }

    constexpr auto operator[](unsigned int index) const
    {
        return v1[index] * v2[index];
    }
};

template <
    typename TLeftRef,
    typename TRightRef,
    typename TLeft = typename std::remove_reference_t<TLeftRef>,
    typename TRight = typename std::remove_reference_t<TRightRef>
    >
class VectorMax final {
private:
    const TLeftRef v1;
    const TRightRef v2;

public:
    static constexpr unsigned int size = TLeft::size;
    using data_type = typename TLeft::data_type;

    explicit VectorMax(
        TLeftRef&& v1,
        TRightRef&& v2)
        : v1(std::forward<TLeftRef>(v1))
        , v2(std::forward<TRightRef>(v2))
    {
    }

    constexpr auto operator[](unsigned int index) const
    {
        return v1[index] < v2[index]
            ? v2[index]
            : v1[index];
    }
};

template <typename TVecRef, typename TVec = std::remove_reference_t<TVecRef>>
constexpr auto operator-(TVecRef&& vec)
{
    return VectorNegative<TVecRef>(std::forward<TVecRef>(vec));
}

template<typename TData>
struct MulOperation{
    constexpr auto operator()(TData x,TData y) const
    {
        return x * y;
    }
};

template<typename TData>
struct AddOperation{
    constexpr auto operator()(TData x,TData y) const
    {
        return x + y;
    }
};

// vector + constant
template <
    typename TLeftRef,
    typename TRight,
    typename TLeft = typename std::remove_reference_t<TLeftRef>,
    typename = typename TLeft::data_type,
    typename = typename std::enable_if_t<IsDataOfVectorKind<TLeft, TRight>::value>>
constexpr auto operator+(
    TLeftRef&& left,
    TRight right)
{
    using data_type = typename TLeft::data_type;
    return VectorConstantOperation<TLeftRef,TRight,AddOperation<data_type>>
        (std::forward<TLeftRef>(left),right,AddOperation<data_type>());
}

// constant + vector
template <
    typename TLeft,
    typename TRightRef,
    typename TRight = typename std::remove_reference_t<TRightRef>,
    typename = typename TRight::data_type,
    typename = typename std::enable_if_t<IsDataOfVectorKind<TRight, TLeft>::value>>
constexpr auto operator+(
    TLeft left,
    TRightRef&& right)
{
    using data_type = typename TRight::data_type;
    return VectorConstantOperation<TRightRef,TLeft,AddOperation<data_type>>
        (std::forward<TRightRef>(right),left,AddOperation<data_type>());
}

// vector * constant
template <
    typename TLeftRef,
    typename TRight,
    typename TLeft = std::remove_reference_t<TLeftRef>,
    typename = typename TLeft::data_type,
    typename = typename std::enable_if_t<IsDataOfVectorKind<TLeft, TRight>::value>>
auto operator*(
    TLeftRef&& left,
    TRight right)
{
    using data_type = typename TLeft::data_type;
    return VectorConstantOperation<TLeftRef,TRight,MulOperation<data_type>>
        (std::forward<TLeftRef>(left),right,MulOperation<data_type>());
}

// constant * vector
template <
    typename TLeft,
    typename TRightRef,
    typename TRight = std::remove_reference_t<TRightRef>,
    typename = typename TRight::data_type,
    typename = typename std::enable_if_t<IsDataOfVectorKind<TRight, TLeft>::value>>
constexpr auto operator*(
    TLeft left,
    TRightRef&& right)
{
    using data_type = typename TRight::data_type;
    return VectorConstantOperation<TRightRef,TLeft,MulOperation<data_type>>
        (std::forward<TRightRef>(right),left,MulOperation<data_type>());
}

template <
    typename TLeft,
    typename TRight>
auto operator-(
    TLeft&& left,
    TRight&& right)
{
    return left + (-right);
}

template <
    typename TLeftRef,
    typename TRightRef,
    typename TRight = std::remove_reference_t<TRightRef>,
    typename TLeft = std::remove_reference_t<TLeftRef>,
    typename typeL = typename TLeft::data_type,
    typename typeR = typename TRight::data_type,
    typename = typename std::enable_if_t<std::is_same<typeL, typeR>::value>,
    unsigned int sizeL = TLeft::size,
    unsigned int sizeR = TRight::size,
    typename = typename std::enable_if_t<sizeL==sizeR>
    >
constexpr auto operator+(
    TLeftRef&& left,
    TRightRef&& right)
{
    return VectorSum<TLeftRef,TRightRef>(
        std::forward<TLeftRef>(left),
        std::forward<TRightRef>(right));
}

template <
    typename TLeftRef,
    typename TRightRef,
    typename TLeft = typename std::remove_reference_t<TLeftRef>,
    typename TRight = typename std::remove_reference_t<TRightRef>,
    typename typeL = typename TLeft::data_type,
    typename typeR = typename TRight::data_type,
    typename = typename std::enable_if_t<std::is_same<typeL, typeR>::value>,
    unsigned int sizeL = TLeft::size,
    unsigned int sizeR = TRight::size,
    typename = typename std::enable_if_t<sizeL==sizeR>
    >
constexpr auto operator*(
    TLeftRef&& left,
    TRightRef&& right)
{
    return SUM(
        VectorProd<TLeftRef, TRightRef>(
            std::forward<TLeftRef>(left),
            std::forward<TRightRef>(right)));
}

class ToVector final{};

template <
    typename TLeft,
    typename data_type = typename TLeft::data_type,
    unsigned int size = TLeft::size
    >
constexpr auto operator|(
    const TLeft& left,
    const ToVector& right)
{
    auto vec = Vector<size, data_type>();
    for (unsigned int i = 0; i < size; i++) {
        vec[i] = left[i];
    }

    return vec;
}

template <
    typename TLeftRef,
    typename TRightRef,
    typename TLeft = typename std::remove_reference_t<TLeftRef>,
    typename TRight = typename std::remove_reference_t<TRightRef>
    >
class VectorComponentMax final {
private:
    TLeft v1;
    TRight v2;

public:
    static constexpr unsigned int size = TLeft::size;
    using data_type = typename TLeft::data_type;

    VectorComponentMax(
        TLeftRef&& v1,
        TRightRef&& v2)
        : v1(std::forward<TLeftRef>(v1))
        , v2(std::forward<TRightRef>(v2))
    {
    }

    constexpr auto operator[](unsigned int index) const
    {
        return v1[index] > v2[index] ? v1[index] : v2[index];
    }
};

template <
    typename TLeftRef,
    typename TRightRef,
    typename TLeft = std::remove_reference_t<TLeftRef>,
    typename TRight = std::remove_reference_t<TRightRef>,
    unsigned int left_size = TLeft::size,
    unsigned int right_size = TRight::size,
    typename = typename std::enable_if_t<left_size==right_size>
    >
constexpr auto ComponentWiseMax(
    TLeftRef&& left,
    TRightRef&& right)
{
    return VectorComponentMax<TLeftRef, TRightRef>(
            std::forward<TLeftRef>(left),
            std::forward<TRightRef>(right));
}

template <typename TVector>
constexpr auto Norm(TVector& v)
{
    return SUM(v * v);
}

template <typename TVector>
constexpr auto Norm2(TVector& v)
{
    return sqrt(SUM(v * v));
}

}
