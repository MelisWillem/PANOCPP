#pragma once
#include<string>
#include<functional>

using CostFunction = pnc::test::Poly<5, 2>;

template<
    typename T,
    auto dimension,
    typename = typename std::enable_if_t<std::less<int>()(1, dimension)>>
std::string ToString(const pnc::Vector<dimension, T>& vec)
{
    std::stringstream ss;
    ss << "[";
    for(std::size_t i=0; i < dimension-1; ++i)
    {
        ss << std::to_string(vec[i]) << ", ";
    }
    ss << std::to_string(vec[dimension-1]);
    ss << "]";


    return ss.str();
}

template<typename T>
std::string ToString(const pnc::Vector<1, T>& vec)
{
    return "[" + std::to_string(vec[0]) + "]";
}

template<typename T>
std::string ToString(const pnc::Vector<0, T>& vec)
{
    return "[]";
}
