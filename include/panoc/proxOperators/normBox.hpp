#pragma once

#include<panoc/VectorAlgebra.hpp>
#include<type_traits>
#include<algorithm>

namespace pnc
{
    template<
        typename TVecRef,
        typename TVec = typename std::remove_reference_t<TVecRef>,
        typename TData = typename TVec::data_type,
        typename TSize = typename TVec::size_type
            >
    class NormBoxOperator{
		public:
            using data_type = TData;
            using size_type = TSize;
        private: 
            enum State{
                low,
                mid,
                high
            };

            TVecRef _input;
            State _state;
            data_type _cost;
            data_type offset;

            constexpr auto sign(const data_type x) const
            {
                return x < data_type{0} 
                ? data_type{-1} 
                : data_type{1};
            }

        public:
            NormBoxOperator(TVecRef&& vec,data_type offset)
                : _input(std::forward<TVecRef>(vec)),offset(offset)
            {
                auto norml1 = NormL1(vec);
                if(norml1<offset)
                {
                    _state = State::low;
                }
                else if( norml1 > 2*offset)
                {
                    _state = State::high;
                }
                else
                {
                    _state = State::mid;
                }
                _cost = std::max(data_type{0},norml1-offset);
            }

            constexpr auto operator[](const unsigned int index) const
            {
                switch(_state)
                {
                    case State::low:
                        return _input[index];
                    case State::mid:
                        return sign(_input[index])*offset;
                    default: // case State::high:
                        return sign(_input[index])*(abs(_input[index])-offset);
                }
            }

            constexpr auto cost() const
            {
                return _cost;
            }

            constexpr auto size() const
            {
                return _input.size();
            }
    };

    template< typename data_type >
    struct NormBox{
        const data_type offset;
        NormBox(data_type offset) : offset(offset)
        {
        }

        template<
            typename TVecRef,
            typename TVec = std::remove_reference_t<TVecRef>,
            typename = std::enable_if_t<std::is_same_v<typename TVec::data_type,data_type>>
                >
        auto operator()(TVecRef&& vec) const
        {
            return NormBoxOperator<TVecRef>(std::forward<TVecRef>(vec),offset);
        }
    };
}
