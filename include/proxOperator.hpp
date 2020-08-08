#include"VectorAlgebra.hpp"
#include<type_traits>

// Proximal operator should look like:
// x_new = prox_op(x_old)
// This should use the concept of a vector and
// not the vector itself.
namespace pnc{


// The box operator always returns a vector with
// elements inside the box borders.
// If the value is outside the borders, the value of 
// the closest border is returned.
// precondition: low < high
template<
    typename data_type,
    data_type low,
    data_type high,
    typename TVector,
    typename = typename std::enable_if_t<(high>low)>
    >
class BoxOperator final{
    private:
        const TVector& input;
    public:
        BoxOperator(const TVector& input): input(input)
        {}

        constexpr auto operator[](const int index)
        {
            if(input[index]>high) { return high; }
            if(input[index]<low) { return low; }
            return input[index];
        }
};

// The box operator always returns a vector with
// elements inside the box borders.
// If the value is outside the borders, the value of 
// the closest border is returned.
// precondition: data_type == TVector::data_type
template<
    typename data_type,
    data_type low,
    data_type high,
    typename TVector,
    typename = typename std::enable_if_t<
        std::is_same_v<data_type,typename TVector::data_type>>>
constexpr auto Box(const TVector& input)
{
    // Sadly enough data_type needs to be defined by the user,
    // there must be a way to get rid of this !
    return BoxOperator<data_type,low,high,decltype(input)>(input);
}

}
