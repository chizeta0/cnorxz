// -*- C++ -*-
/**
   
   @file include/array/slice.h
   @brief Slice declarations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_slice_h__
#define __cxz_slice_h__

#include "base/base.h"
#include "array_base.h"

namespace CNORXZ
{
    /** ****
	constant and possibly partial view on the data
	of another container

	@tparam T data type
     */
    template <typename T>
    class CSlice : public CArrayBase<T>
    {
    public:
	typedef CArrayBase<T> AB;
	typedef typename AB::const_iterator const_iterator;

	DEFAULT_MEMBERS(CSlice); /**< default constructors and assignments */

	/** create slice from an array

	    @param range the slice's container range
	    @param parent the original container
	    @param blockSizes the format of the slice
	    @param off the initial pointer position w.r.t. the original initial position
	 */
	CSlice(const RangePtr& range, const CArrayBase<T>* parent,
	       const YFormat& blockSizes, SizeT off);
	
	virtual const T* data() const override;
	virtual const_iterator cbegin() const override;
	virtual const_iterator cend() const override;
	virtual bool isView() const override final;

    protected:

	virtual bool formatIsTrivial() const override final;
	
	const CArrayBase<T>* mCParent = nullptr; /**< pointer to the original container */
	YFormat mBlockSizes; /**< the format */
	SizeT mOff = 0; /** pointer offset w.r.t. the original pointer */
	
    };

    /** ****
	possibly partial view on the data
	of another container

	@tparam T data type
     */
    template <typename T>
    class Slice : public ArrayBase<T>
    {
    public:
	typedef CArrayBase<T> AB;
	typedef typename AB::const_iterator const_iterator;

	DEFAULT_MEMBERS(Slice); /**< default constructors and assignments */

	/** create slice from an array

	    @param range the slice's container range
	    @param parent the original container
	    @param blockSizes the format of the slice
	    @param off the initial pointer position w.r.t. the original initial position
	 */
	Slice(const RangePtr& range, ArrayBase<T>* parent,
	      const YFormat& blockSizes, SizeT off);

	virtual const T* data() const override;
	virtual T* data() override;
	virtual const_iterator cbegin() const override;
	virtual const_iterator cend() const override;
	virtual bool isView() const override final;

    protected:
	virtual bool formatIsTrivial() const override final;

    private:
	ArrayBase<T>* mParent = nullptr;
	YFormat mBlockSizes;
	SizeT mOff = 0;

    };
}

#endif
