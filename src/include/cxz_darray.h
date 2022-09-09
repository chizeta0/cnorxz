
#ifndef __cxz_darray_h__
#define __cxz_darray_h__

#include "cxz_darray_base.h"

namespace CNORXZ
{

    template <typename T>
    class DArray : public MDArrayBase<T>
    {
    public:

	typedef DArrayBase<T> AB;
	typedef typename AB::const_iterator const_iterator;

	using DArrayBase<T>::operator[];
	using MDArrayBase<T>::operator[];

    private:
	vector<T> mCont;
	
    public:

	DEFAULT_MEMBERS(DArray);
	DArray(const RangePtr& range);
	DArray(const RangePtr& range, const vector<T>& vec);
	DArray(const RangePtr& range, vector<T>&& vec);

	virtual const T* data() const override;
	virtual T* data() override;
	virtual const_iterator cbegin() const override;
	virtual const_iterator cend() const override;
	virtual bool isView() const override;
	
    };
    
}

#endif
