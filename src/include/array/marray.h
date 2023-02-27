
#ifndef __cxz_marray_h__
#define __cxz_marray_h__

#include "array_base.h"

namespace CNORXZ
{

    template <typename T>
    class MArray : public ArrayBase<T>
    {
    public:
	typedef CArrayBase<T> AB;
	typedef typename AB::const_iterator const_iterator;

	using CArrayBase<T>::operator[];
	using ArrayBase<T>::operator[];

	DEFAULT_MEMBERS(MArray);
	MArray(const RangePtr& range);
	MArray(const RangePtr& range, const Vector<T>& vec);
	MArray(const RangePtr& range, Vector<T>&& vec);
	MArray& init(const RangePtr& range);
	MArray& extend(const RangePtr& range);

	virtual const T* data() const override;
	virtual T* data() override;
	virtual const_iterator cbegin() const override;
	virtual const_iterator cend() const override;
	virtual bool isView() const override;

	SERIALIZATION_FUNCTIONS;
	
    private:
	Vector<T> mCont;
    };
    
}

#endif
