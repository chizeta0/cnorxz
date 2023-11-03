/**
   
   @file include/base/dtype.h
   @brief DType declaration

   DType is a generic type-erasing class that wrapps std::any.
   
   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_dynamic_meta_h__
#define __cxz_dynamic_meta_h__

#include <cstdlib>
#include <utility>
#include <memory>
#include <any>
#include <functional>

#include "macros.h"
#include "types.h"

namespace CNORXZ
{
    class DType
    {
    private:
	std::any mD;
	std::function<String(const std::any&)> mToStr;
	std::function<Int(const std::any&,const std::any&)> mComp;

	template <typename T>
	void _mkToStr();

	template <typename T>
	void _mkComp();
	
    public:
	DEFAULT_MEMBERS(DType);
	
	template <typename T>
	DType(const T& d);
	
	template <typename T>
	DType& operator=(const T& d);

	String str() const { return mToStr(mD); }
	const std::any& get() const { return mD; }

	bool operator==(const DType& a) const { return mComp(mD,a.mD) == 0; }
	bool operator!=(const DType& a) const { return mComp(mD,a.mD) != 0; }
	bool operator<(const DType& a) const { return mComp(mD,a.mD) == -1; }
	bool operator>(const DType& a) const { return mComp(mD,a.mD) == 1; }
	bool operator<=(const DType& a) const { auto c = mComp(mD,a.mD); return c <= 0; }
	bool operator>=(const DType& a) const { auto c = mComp(mD,a.mD); return c == 1 or c == 0; }
    };
    
} // namespace CNORXZ

#endif
