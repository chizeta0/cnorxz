
#ifndef __x_to_string_h__
#define __x_to_string_h__

#include <string>
#include <vector>
#include <array>
#include <tuple>

namespace MultiArrayHelper
{
    template <typename T>
    inline std::string xToString(const T& x);


    template <>
    inline std::string xToString<char>(const char& x);

    template <>
    inline std::string xToString<std::string>(const std::string& x);

    template <typename T>
    inline std::string xToString(const std::vector<T>& x);

    template <typename T, size_t N>
    inline std::string xToString(const std::array<T,N>& x);

    template <typename... Ts>
    inline std::string xToString(const std::tuple<Ts...>& tp);

    // TEMPLATE CODE

    template <typename T>
    inline std::string xToString(const T& x)
    {
	return std::to_string(x);
    }

    template <size_t N>
    struct TupleToString
    {
	template <typename... Ts>
	static inline std::string mk(const std::tuple<Ts...>& tp)
	{
	    return TupleToString<N-1>::mk(tp) + " , " + xToString(std::get<N>(tp));
	}
    };

    template <>
    struct TupleToString<0>
    {
	template <typename... Ts>
	static inline std::string mk(const std::tuple<Ts...>& tp)
	{
	    return xToString(std::get<0>(tp));
	}
    };

    template <>
    inline std::string xToString<char>(const char& x)
    {
	std::string out = "";
	return out += x;
    }

    template <>
    inline std::string xToString<std::string>(const std::string& x)
    {
	return x;
    }

    template <typename T>
    inline std::string xToString(const std::vector<T>& x)
    {
	std::string out = "[ ";
	for(auto& y: x){
	    out += xToString(y) + " , ";
	}
	out.pop_back();
	out.back() = ']';
	return out;
    }

    template <typename T, size_t N>
    inline std::string xToString(const std::array<T,N>& x)
    {
	std::string out = "[ ";
	for(auto& y: x){
	    out += xToString(y) + " , ";
	}
	out.pop_back();
	out.back() = ']';
	return out;
    }

    template <typename... Ts>
    inline std::string xToString(const std::tuple<Ts...>& tp)
    {
	return "{ " + TupleToString<sizeof...(Ts)-1>::mk(tp) + " }";
    }

}

#endif
