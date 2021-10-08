#pragma once
#include<string>
#include<functional>
#include<panoc/VectorAlgebra.hpp>
#include<sstream>

template<typename T>
std::string ToString(const pnc::Vector<T>& vec)
{
    if (vec.size() == 0)
    {
		return "[]";
    }
    if (vec.size() == 1) {
		return "[" + std::to_string(vec[0]) + "]";
    }

    std::stringstream ss;
    ss << "[";
    const auto dimension = vec.size();
    for(typename T::size_type i=0; i < dimension-1; ++i)
    {
        ss << std::to_string(vec[i]) << ", ";
    }
    ss << std::to_string(vec[dimension-1]);
    ss << "]";


    return ss.str();
}
