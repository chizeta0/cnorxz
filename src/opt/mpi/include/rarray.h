// -*- C++ -*-
/**
   
   @file opt/mpi/include/rarray.h
   @brief RArray declarations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_mpi_rarray_h__
#define __cxz_mpi_rarray_h__

#include "cnorxz.h"

namespace CNORXZ
{
    /** ****
	Multi-dimensional multi-rank array.
	@tparam T data type.
     */
    template <typename T>
    class RArray : public ArrayBase<T>
    {
    public:
	typedef CArrayBase<T> AB;
	typedef typename RAIndex const_iterator;

	using CArrayBase<T>::operator[];
	using ArrayBase<T>::operator[];

	DEFAULT_MEMBERS(RArray);
	
	virtual const T* data() const override;
	virtual T* data() override;
	virtual const_iterator cbegin() const override;
	virtual const_iterator cend() const override;
	virtual bool isView() const override;

    protected:
	virtual bool formatIsTrivial() const override final;
	
    private:
	Vector<T> mCont;
	RangePtr mRRange;
    };
}

#endif
