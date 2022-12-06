
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
	Vector<SizeT> mBlockSizes;
	SizeT mOff = 0;
	
    public:
	DEFAULT_MEMBERS(CSlice);
	CSlice(const RangePtr& range, const CArrayBase<T>* parent,
	       const Vector<SizeT>& blockSizes, SizeT off);
	
	virtual const T* data() const override;
	virtual const_iterator cbegin() const override;
	virtual const_iterator cend() const override;
	virtual bool isView() const override final;
    };

    template <typename T>
    class Slice : public virtual ArrayBase<T>,
		  public virtual CSlice<T>
    {
    public:
	typedef CArrayBase<T> AB;
	typedef typename AB::const_iterator const_iterator;

    private:
	ArrayBase<T>* mParent = nullptr;

    public:
	DEFAULT_MEMBERS(Slice);
	Slice(const RangePtr& range, ArrayBase<T>* parent,
	      const Vector<SizeT>& blockSizes, SizeT off);

	virtual T* data() override;
    };
}

#endif
