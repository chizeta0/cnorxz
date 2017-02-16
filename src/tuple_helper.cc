
#include "tuple_helper.h"

namespace MultiArrayTools
{

    template <class Tuple, size_t N>
    auto make_left_x(const Tuple& tp) -> decltype(std::tuple_cat(make_left<Tuple,N-1>(tp),
							       std::make_tuple(get<N>(tp))))
    {
	return std::tuple_cat(make_left<Tuple,N-1>(tp), std::make_tuple(get<N>(tp)));
    }

    template <class Tuple>
    auto make_left_x<Tuple, 0>(const Tuple& tp) -> decltype(std::make_tuple(get<N>(tp)))
    {
	return std::make_tuple(get<N>(tp));
    }

    template <class Tuple, size_t N>
    auto make_right_x(const Tuple& tp) -> decltype(std::tuple_cat(std::make_tuple(get<std::tuple_size(tp)-N>(tp)),
								make_left<Tuple,std::tuple_size(tp)-N+1>(tp)))
    {
	const size_t M = std::tuple_size(tp) - N;
	return std::tuple_cat(std::make_tuple(get<M>(tp)), make_left<Tuple,M+1>(tp));
    }

    template <class Tuple>
    auto make_right_x<Tuple, 0>(const Tuple& tp) -> decltype(std::make_tuple(get<std::tuple_size(tp)>(tp)))
    {
	const size_t M = std::tuple_size(tp);
	return std::make_tuple(get<M>(tp));
    }

    template <class Tuple, size_t N>
    auto make_left(const Tuple& tp) -> decltype(make_left_x<Tuple, N>(tp))
    {
	return make_left_x<Tuple, N>(tp);
    }

    template <class Tuple, size_t N>
    auto make_right(const Tuple& tp) -> decltype(make_right_x<Tuple, std::tuple_size(Tuple)-N>(tp))
    {
	return make_right_x<Tuple, std::tuple_size(Tuple)-N>(tp);
    }
}
