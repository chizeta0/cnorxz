// -*- C++ -*-

#ifndef __index_base_h__
#define __index_base_h__

#include <cstdlib>
#include <string>
#include <vector>

#include "base_def.h"
#include "range_base.h"

namespace MultiArrayTools
{
    size_t indexId()
    {
	static size_t id = 0;
	++id;
	return id;
    }

    enum class IndexType{
	SINGLE = 0,
	MULTI = 1,
	CONT = 2	
    };    

    class VirtualIndexWrapperBase
    {
    public:
	DEFAULT_MEMBERS(VirtualIndexWrapperBase);
	
	virtual IndexType type() const = 0;
	virtual size_t dim() const = 0;
	virtual size_t pos() const = 0;
	virtual size_t max() const = 0;
	virtual std::shared_ptr<RangeBase> rangePtr() const = 0;
	virtual std::shared_ptr<VirtualIndexWrapperBase> getPtr(size_t n) const = 0;
	virtual std::intptr_t getPtrNum() const = 0;
	virtual size_t getStepSize(size_t n) const = 0;
    };

    template <class I>
    std::shared_ptr<IndexWrapper<I> > make_viwb(std::shared_ptr<I> idxPtr)
    {
	return std::make_shared<IndexWrapper<I> >(idxPtr);
    }

    template <class I>
    std::shared_ptr<IndexWrapper<I> > make_viwb(const I& idxPtr)
    {
	return make_viwb( std::make_shared<I>(idxPtr) );
    }
    
    template <class I>
    class IndexWrapper : public VirtualIndexWrapperBase
    {
    public:
	
	DEFAULT_MEMBERS(IndexWrapper);

	IndexWrapper(std::shared_ptr<I> idxPtr) : mIdxPtr(idxPtr) {}

	virtual IndexType type() const override { return mIdxPtr->type(); }
	virtual size_t dim() const override { return mIdxPtr->dim(); }
	virtual size_t pos() const override { return mIdxPtr->pos(); }
	virtual size_t max() const override { return mIdxPtr->max(); }
	virtual std::shared_ptr<RangeBase> rangePtr() const override { return mIdxPtr->vrange(); }
	virtual std::shared_ptr<VirtualIndexWrapperBase> getPtr(size_t n) const override { return mIdxPtr->getVPtr(n); }
	virtual std::intptr_t getPtrNum() const override { return reinterpret_cast<std::intptr_t>( mIdxPtr.get() ); }
	virtual size_t getStepSize(size_t n) const override { return mIdxPtr->getStepSize(n); }

	std::shared_ptr<I> get() const { return mIdxPtr; } // unwrap
	
    private:
	std::shared_ptr<I> mIdxPtr;
    };
    
    template <class I, typename MetaType>
    class IndexInterface
    {
    public:
	//typedef typename I::RangeType RangeType;
	
	//DEFAULT_MEMBERS(IndexInterface);

	I* THIS() { return static_cast<I*>(this); }
	I const* THIS() const { return static_cast<I const*>(this); }
	
	~IndexInterface() = default; 

	IndexType type() const { return I::S_type(THIS()); }
	
	I& operator=(size_t pos) { return I::S_ass_op(THIS(), pos); }
	I& operator++() { return I::S_pp_op(THIS()); }
	I& operator--() { return I::S_mm_op(THIS()); }

	int pp(std::intptr_t idxPtrNum) { return I::S_pp(THIS(), idxPtrNum); }
	int mm(std::intptr_t idxPtrNum) { return I::S_mm(THIS(), idxPtrNum); }
	
	bool operator==(const IndexInterface& in) const;
	bool operator!=(const IndexInterface& in) const;
	
	size_t dim() const { return I::S_dim(THIS()); }
	size_t pos() const;
	size_t max() const;

	bool last() const { return I::S_last(THIS()); }
	bool first() const { return I::S_first(THIS()); }

	
	std::shared_ptr<RangeBase> vrange() const { return mRangePtr; }
	/*auto range() const -> decltype( I::S_range(THIS()) ) { return I::S_range(THIS()); }
	
	template <size_t N>
	auto getPtr() const -> decltype(I::template S_get<N>(THIS()))
	{ return I::template S_get<N>(THIS()); }
	*/
	std::shared_ptr<VIWB> getVPtr(size_t n) const { return I::S_getVPtr(THIS(),n); }
	
	size_t getStepSize(size_t n) const { return I::S_getStepSize(THIS(),n); }
	
	operator size_t() const;

	std::string id() const { return I::S_id(THIS()); }

	MetaType meta() const { return I::S_meta(THIS()); }
	I& at(const MetaType& meta) { return I::S_at(THIS(), meta); }

	
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
