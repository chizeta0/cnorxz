// -*- C++ -*-

#ifndef __cxz_index_base_h__
#define __cxz_index_base_h__

#include <cstdlib>
#include <string>
#include <vector>

#include "rbase_def.h"
#include "range_base.h"
#include "index_type.h"

#include "xfor/xfor.h"

namespace CNORXZ
{
    
    template <class I, typename MetaType>
    class IndexInterface
    {
    public:
	//typedef typename I::RangeType RangeType;
	
	//DEFAULT_MEMBERS(IndexInterface);

	I& THIS() { return static_cast<I&>(*this); }
	I const& THIS() const { return static_cast<I const&>(*this); }

	static constexpr bool ISINDEX = true;

	~IndexInterface() = default; 

	constexpr IndexType type() const { return THIS().type(); }
	
	I& operator=(size_t pos) { return THIS() = pos; }
	I& operator++() { return THIS()++; }
	I& operator--() { return THIS()--;}

	int pp(std::intptr_t idxPtrNum) { return THIS().pp(idxPtrNum); }
	int mm(std::intptr_t idxPtrNum) { return THIS().mm(idxPtrNum); }
	
	bool operator==(const IndexInterface& in) const;
	bool operator!=(const IndexInterface& in) const;
	
	size_t dim() const { return THIS().dim(); }
	size_t pos() const;
	size_t max() const;

	bool last() const { return THIS().last(); }
	bool first() const { return THIS().first(); }

	
	std::shared_ptr<RangeBase> vrange() const { return mRangePtr; }
	
	size_t getStepSize(size_t n) const { return THIS().getStepSize(n); }
	
	operator size_t() const;

	std::string stringMeta() const { return THIS().stringMeta(); }
	MetaType meta() const { return THIS().meta(); }
	MetaType metaPtr() const { return THIS().metaPtr(); }
	I& at(const MetaType& meta) { return THIS().at(meta); }

	// CHECK / IMPLEMENT !!!!!!
	template <class Expr>
	auto ifor(size_t step, const Expr ex) const
            -> decltype(THIS().template ifor<Expr>(step,ex))
	{ return THIS().template ifor<Expr>(step,ex); }

	template <class Expr>
	auto iforh(size_t step, const Expr ex) const
            -> decltype(THIS().template iforh<Expr>(step,ex))
	{ return THIS().template iforh<Expr>(step,ex); }

	std::intptr_t ptrNum() const;
	
    private:

	friend I;
	
	IndexInterface();
	IndexInterface(const IndexInterface& in);
	IndexInterface& operator=(const IndexInterface& in);
	IndexInterface(IndexInterface&& in);
	IndexInterface& operator=(IndexInterface&& in);
	
	IndexInterface(const std::shared_ptr<RangeBase>& range, size_t pos);
	
	std::shared_ptr<RangeBase> mRangePtr;
	size_t mPos = 0;
	size_t mMax = 0;

	std::intptr_t mPtrNum;
    };

    template <class Index>
    std::shared_ptr<Index> mapResult(const std::shared_ptr<Index>& iptr)
    {
	return iptr;
    }

}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace CNORXZ
{
    /**********************
     *   IndexInterface   *
     **********************/

    template <class I, typename MetaType>
    IndexInterface<I,MetaType>::IndexInterface()
    {
	mPtrNum = reinterpret_cast<std::intptr_t>(this);
    }

    template <class I, typename MetaType>
    IndexInterface<I,MetaType>::IndexInterface(const IndexInterface& in) : mRangePtr(in.mRangePtr),
									   mPos(in.mPos),
									   mMax(in.mMax)
    {
	mPtrNum = reinterpret_cast<std::intptr_t>(this);
    }

    template <class I, typename MetaType>
    IndexInterface<I,MetaType>::IndexInterface(IndexInterface&& in) : mRangePtr(in.mRangePtr),
								      mPos(in.mPos),
								      mMax(in.mMax)
    {
	mPtrNum = reinterpret_cast<std::intptr_t>(this);
    }

    template <class I, typename MetaType>
    IndexInterface<I,MetaType>& IndexInterface<I,MetaType>::operator=(const IndexInterface& in)
    {
	mRangePtr = in.mRangePtr;
	mPos = in.mPos;
	mMax = in.mMax;
	return *this;
    }

    template <class I, typename MetaType>
    IndexInterface<I,MetaType>& IndexInterface<I,MetaType>::operator=(IndexInterface&& in)
    {
	mRangePtr = in.mRangePtr;
	mPos = in.mPos;
	mMax = in.mMax;
	return *this;
    }
    
    template <class I, typename MetaType>
    IndexInterface<I,MetaType>::IndexInterface(const std::shared_ptr<RangeBase>& range,
					       size_t pos) : mRangePtr(range),
							     mPos(pos),
							     mMax(mRangePtr->size())
    {
	mPtrNum = reinterpret_cast<std::intptr_t>(this);
    }

    template <class I, typename MetaType>
    bool IndexInterface<I,MetaType>::operator==(const IndexInterface& in) const
    {
	return in.mPos == mPos and in.mRangePtr.get() == mRangePtr.get();
    }

    template <class I, typename MetaType>
    bool IndexInterface<I,MetaType>::operator!=(const IndexInterface& in) const
    {
	return in.mPos != mPos or in.mRangePtr.get() != mRangePtr.get();
    }

    template <class I, typename MetaType>
    size_t IndexInterface<I,MetaType>::pos() const
    {
	return mPos;
    }

    template <class I, typename MetaType>
    size_t IndexInterface<I,MetaType>::max() const
    {
	return mMax;
    }

    template <class I, typename MetaType>
    IndexInterface<I,MetaType>::operator size_t() const
    {
	return pos();
    }

    template <class I, typename MetaType>
    std::intptr_t IndexInterface<I,MetaType>::ptrNum() const
    {
	return mPtrNum;
    }
}

#endif
