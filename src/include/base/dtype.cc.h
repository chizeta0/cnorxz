// -*- C++ -*-
/**
   
   @file include/base/dtype.cc.h
   @brief DType template member function definitions

   Definitions of the template member functions of the DType class.
   
   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

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
	_mkComp<T>();
    }

    template <typename T>
    DType& DType::operator=(const T& d)
    {
	mD = d;
	_mkToStr<T>();
	_mkComp<T>();
	return *this;
    }

    template <typename T>
    void DType::_mkToStr()
    {
	mToStr = [](const std::any& d){
	    return toString(std::any_cast<T>(d));
	};
    }

    template <typename T>
    void DType::_mkComp()
    {
	mComp = [](const std::any& d, const std::any& a){
	    if(d.type() != a.type()){
		return 2;
	    }
	    else {
		auto& at = std::any_cast<const T&>(a);
		auto& dt = std::any_cast<const T&>(d);
		if(std::equal_to<T>{}(dt,at)){
		    return 0;
		}
		else if(std::less<T>{}(dt,at)){
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
