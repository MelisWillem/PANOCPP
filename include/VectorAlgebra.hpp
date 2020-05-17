#include <cassert>

// simple vector algebra
// two operations(applied element wise) are defined:
//    - v = v1 * v2
//    - v = v1 + v2
//
// extra:
// SUM -> calculates sum of all elements on the vector
namespace pnc {

template <unsigned int size, typename TData = double>
class VectorUnit {
private:
    const TData _amplitude;

public:
    constexpr unsigned int size() { return size; }
    //using data_type = TData;

    VectorUnit(TData amplitude)
    {
        this->amplitude = amplitude;
    }

    TData operator[](unsigned int index) const
    {
        return _amplitude;
    }
};

template <unsigned int size, typename TData = double>
class Vector {
private:
    const TData* data;

public:
    constexpr unsigned int get_size()  const { return size; } 

    Vector(const TData init[size]): data(init) { }

    ~Vector()
    {
        delete data;
    }

    TData operator[](unsigned int index) const
    {
        return data[index];
    }

    Vector(Vector&& other)
        : data(other.data){}
};

template <
    typename TLeft,
    typename TRight>
auto operator+(
    const TLeft& left,
    const TRight& right)
{
    return VectorSum<TLeft, TRight>(left, right);
}

template <
    typename TLeft,
    typename TRight>
auto operator*(
    const TLeft& left,
    const TRight& right)
{
    return VectorProd<TLeft, TRight>(left, right);
}

template <typename TVec>
auto SUM(const TVec& vec)
{
    auto cache = vec[0];
    for (unsigned int i = 1; i < vec.get_size(); i++) {
        cache += vec[i];
    }
    return cache;
}

template <typename TVec>
auto MAX(const TVec& vec)
{
    auto cache = vec[0];
    for (unsigned int i = 1; i < vec.get_size(); i++) {
        if(cache<vec[i])
        {
            cache = vec[i];
        }
    }
    return cache;
}

template <typename TVec>
auto MAX(const TVec left,const TVec right)
{
    return VectorMax(left,right);
}


template <typename TVector>
class VectorNegative {
private:
    const TVector& v;

public:
    constexpr unsigned int get_size() { return v.get_size(); }
    //using data_type=v::data_type;

    VectorNegative(TVector& v)
        : v(v) {}

    auto operator[](unsigned int index) const
    {
        return -v[index];
    }
};

template <typename TLeft, typename TRight>
class VectorSum {
private:
    const TLeft& v1;
    const TRight& v2;

public:
    constexpr unsigned int get_size() const { return v1.get_size(); }

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
    constexpr unsigned int get_size() const { return v1.get_size(); }

    VectorProd(
        const TLeft& v1,
        const TRight& v2)
        : v1(v1)
        , v2(v2) { }

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
    constexpr unsigned int get_size() const { return v1.get_size(); }

    VectorMax(
        const TLeft& v1,
        const TRight& v2)
        : v1(v1)
        , v2(v2) { }

    auto operator[](unsigned int index) const
    {
        return v1[index] < v2[index] 
            ? v2[index] 
            : v1[index];
    }
};

}
