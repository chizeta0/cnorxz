
#ifndef __cxz_dtype_cc_h__
#define __cxz_dtype_cc_h__

#include <sstream>
#include "dtype.h"
#include "to_string.h"

namespace CNORXZ
{
    // for tuple use vector<DType> !!!
    // (yes DType is slow, thats why one should only use it for debugging)
    
    template <typename T>
    DType::DType(const T& d) : mD(d)
    {
	_mkToStr<T>();
    }

    template <typename T>
    DType& DType::operator=(const T& d)
    {
	mD = d;
	_mkToStr<T>();
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

#endif
