// -*- C++ -*-
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
    /** ****
	Type erasing class wrapping std::any
     */
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
	DEFAULT_MEMBERS(DType); /**< default constructors and assignments */

	/** Generic constructor template

	    Constructs a DType from the given argument
	    @tparam T input data type 
	    @param d input
	 */
	template <typename T>
	DType(const T& d);
	
	/** Generic assignment template

	    Assigns the given argument to the DType
	    @tparam T input data type 
	    @param d input
	 */
	template <typename T>
	DType& operator=(const T& d);

	/** convert contained data so string */
	String str() const { return mToStr(mD); }

	/** return reference to type-erased data */
	const std::any& get() const { return mD; }

	/** check for equality
	    @param a variable to compare with
	 */
	bool operator==(const DType& a) const { return mComp(mD,a.mD) == 0; }

	/** check for inequality
	    @param a variable to compare with
	 */
	bool operator!=(const DType& a) const { return mComp(mD,a.mD) != 0; }

	/** check smaller
	    @param a variable to compare with
	 */
	bool operator<(const DType& a) const { return mComp(mD,a.mD) == -1; }

	/** check greater
	    @param a variable to compare with
	 */
	bool operator>(const DType& a) const { return mComp(mD,a.mD) == 1; }

	/** check not greater
	    @param a variable to compare with
	 */
	bool operator<=(const DType& a) const { auto c = mComp(mD,a.mD); return c <= 0; }

	/** check not smaller
	    @param a variable to compare with
	 */
	bool operator>=(const DType& a) const { auto c = mComp(mD,a.mD); return c == 1 or c == 0; }
    };
    
} // namespace CNORXZ

#endif
