#include"VectorAlgebra.hpp"
#include<type_traits>
#include<algorithm>

namespace pnc
{
    template<
        typename TVecRef,
        typename TVec = std::remove_reference_t<TVecRef>,
        typename data_type = typename TVec::data_type
        >
    class NormBoxOperator{
        private: 
            enum State{
                low,
                mid,
                high
            };

            TVecRef _input;
            const data_type _offset;
            State _state;
            data_type _cost;

            constexpr auto sign(const data_type x) const
            {
                return x < data_type{0} 
                ? data_type{-1} 
                : data_type{1};
            }

        public:
            NormBoxOperator(TVecRef&& vec, const data_type offset)
                : _input(std::forward<TVecRef>(vec)), _offset(offset)
            {
                auto norml1 = NormL1(vec);
                if(norml1<_offset)
                {
                    _state = State::low;
                }
                else if( norml1 > 2*_offset)
                {
                    _state = State::high;
                }
                else
                {
                    _state = State::mid;
                }
                _cost = std::max(0,norml1-offset);
            }

            constexpr auto operator[](const unsigned int index) const
            {
                switch(_state)
                {
                    case State::low:
                        return _input[index];
                    case State::mid:
                        return sign(_input[index])*_offset;
                    case State::high:
                        return sign(_input[index])*(abs(_input[index])-_offset);
                }
            }

            constexpr auto cost() const
            {
                return _cost;
            }
    };

    template<
        typename TVectorRef,
        typename TVector = std::remove_reference_t<TVectorRef>,
        typename data_type = typename TVector::data_type
            >
    constexpr auto NormBox(TVectorRef&& input, data_type offset)
    {
        return NormBoxOperator<TVectorRef>(std::forward<TVectorRef>(input),offset);
    }
}
