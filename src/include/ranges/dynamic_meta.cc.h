
#include <sstream>
#include "dynamic_meta.h"

namespace CNORXZ
{
    template <typename T>
    std::string toString(const T& a)
    {
	std::stringstream ss;
	ss << a;
	return ss.str();
    }

    template <typename T>
    std::string toString<vector<T>>(const vector& a)
    {
	std::stringstream ss;
	ss << "[";
	auto it = a.begin();
	for(; it != a.end()-1; ++it){
	    ss << *it << ",";
	}
	ss << *it << "]";
	return ss.str();
    }

    template <typename T, size_t N>
    std::string toString<std::array<T,N>>(const std::array<T,N>& a)
    {
	std::stringstream ss;
	ss << "(";
	auto it = a.begin();
	for(; it != a.end()-1; ++it){
	    ss << *it << ",";
	}
	ss << *it << ")";
	return ss.str();
    }

    // for tuple use vector<DType> !!!
    // (yes DType is slow, thats why one should only use it for debugging)
    
    template <>
    std::string toString<DType>(const DType& a)
    {
	return a.str();
    }
    
    template <typename T>
    DType::DType(const T& d) : mD(d)
    {
	_mkToStr();
    }

    template <typename T>
    DType& DType::operator=(const T& d)
    {
	mD = d;
	_mkToStr();
	return *this;
    }

    template <typename T>
    void DType::_mkToStr()
    {
	mToStr = [&](){
	    return toString(std::any_cast<T>(mD));
	};
    }

    template <typename T>
    void DType::_mkComp()
    {
	mComp = [&](const std::any& a){
	    if(mD.type() != a.type()){
		return 2;
	    }
	    else {
		auto& at = std::any_cast<const T&>(a);
		auto& dt = std::any_cast<const T&>(mD);
		if(std::equal_to(dt,at)){
		    return 0;
		}
		else if(std::less(dt,at)){
		    return -1;
		}
		else {
		    return 1;
		}
	    }
	};
    }
}
