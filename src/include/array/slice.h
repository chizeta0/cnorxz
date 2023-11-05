// -*- C++ -*-
/**
   
   @file include/array/slice.h
   @brief ...


   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_slice_h__
#define __cxz_slice_h__

#include "base/base.h"
#include "array_base.h"

namespace CNORXZ
{
    template <typename T>
    class CSlice : public CArrayBase<T>
    {
    public:
	typedef CArrayBase<T> AB;
	typedef typename AB::const_iterator const_iterator;

    protected:
	const CArrayBase<T>* mCParent = nullptr;
	YFormat mBlockSizes;
	SizeT mOff = 0;
	
    public:
	DEFAULT_MEMBERS(CSlice);
	CSlice(const RangePtr& range, const CArrayBase<T>* parent,
	       const YFormat& blockSizes, SizeT off);
	
	virtual const T* data() const override;
	virtual const_iterator cbegin() const override;
	virtual const_iterator cend() const override;
	virtual bool isView() const override final;
    };

    template <typename T>
    class Slice : public ArrayBase<T>
    {
    public:
	typedef CArrayBase<T> AB;
	//typedef CSlice<T> CS;
	typedef typename AB::const_iterator const_iterator;

    private:
	ArrayBase<T>* mParent = nullptr;
	YFormat mBlockSizes;
	SizeT mOff = 0;

    public:
	DEFAULT_MEMBERS(Slice);
	Slice(const RangePtr& range, ArrayBase<T>* parent,
	      const YFormat& blockSizes, SizeT off);

	virtual const T* data() const override;
	virtual T* data() override;
	virtual const_iterator cbegin() const override;
	virtual const_iterator cend() const override;
	virtual bool isView() const override final;
    };
}

#endif
