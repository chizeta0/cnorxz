// -*- C++ -*-

#ifndef __index_base_h__
#define __index_base_h__

#include <cstdlib>
#include <string>
#include <vector>

#include "rbase_def.h"
#include "range_base.h"
#include "index_type.h"
#include "vindex_wrapper.h"

namespace MultiArrayTools
{
    
    template <class I, typename MetaType>
    class IndexInterface
    {
    public:
	//typedef typename I::RangeType RangeType;
	
	//DEFAULT_MEMBERS(IndexInterface);

	I& THIS() { return static_cast<I&>(*this); }
	I const& THIS() const { return static_cast<I const&>(*this); }
	
	~IndexInterface() = default; 

	IndexType type() const { return THIS().type(); }
	
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
	/*auto range() const -> decltype( I::S_range(THIS()) ) { return I::S_range(THIS()); }
	
	template <size_t N>
	auto getPtr() const -> decltype(I::template S_get<N>(THIS()))
	{ return I::template S_get<N>(THIS()); }
	*/
	std::shared_ptr<VIWB> getVPtr(size_t n) const { return THIS().getVPtr(n); }
	
	size_t getStepSize(size_t n) const { return THIS().getStepSize(n); }
	
	operator size_t() const;

	std::string id() const { return THIS().id(); }

	MetaType meta() const { return THIS().meta(); }
	I& at(const MetaType& meta) { return THIS().at(meta); }

	void print(size_t offset = 0) const { THIS().print(offset); }
	
    private:

	friend I;
	
	IndexInterface() { mId = indexId(); }
	IndexInterface(const IndexInterface& in) = default;
	IndexInterface& operator=(const IndexInterface& in) = default;
	IndexInterface(IndexInterface&& in) = default;
	IndexInterface& operator=(IndexInterface&& in) = default;
	
	IndexInterface(const std::shared_ptr<RangeBase>& range, size_t pos);
	
	std::shared_ptr<RangeBase> mRangePtr;
	size_t mPos;
	size_t mId;
	size_t mMax;
    };
   
}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
    /**********************
     *   IndexInterface   *
     **********************/

    template <class I, typename MetaType>
    IndexInterface<I,MetaType>::IndexInterface(const std::shared_ptr<RangeBase>& range,
					       size_t pos) : mRangePtr(range),
							     mPos(pos),
							     mMax(mRangePtr->size())
    {
	mId = indexId();
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
}

#endif
