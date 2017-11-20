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
    
    class IndexBase
    {
    public:
	//DEFAULT_MEMBERS(IndexBase);

	IndexBase() { mId = indexId(); }
	IndexBase(IndexBase&& in) = default;
	IndexBase& operator=(IndexBase&& in) = default;
	
	IndexBase(const std::shared_ptr<RangeBase>& range, size_t pos);
	virtual ~IndexBase() = default; 

	virtual IndexType type() const = 0;
	
	virtual IndexBase& operator=(size_t pos) = 0;
	virtual IndexBase& operator++() = 0;
	virtual IndexBase& operator--() = 0;

	virtual int pp(std::shared_ptr<IndexBase>& idxPtr) = 0;
	virtual int mm(std::shared_ptr<IndexBase>& idxPtr) = 0;
	
	bool operator==(const IndexBase& in) const;
	bool operator!=(const IndexBase& in) const;
	
	virtual size_t dim() const = 0;
	virtual size_t pos() const;
	virtual size_t max() const;

	virtual bool last() const = 0;
	virtual bool first() const = 0;

	//virtual bool locked() const;
	//virtual IndexBase& lock(std::shared_ptr<IndexBase>& idx);

	virtual std::shared_ptr<RangeBase> rangePtr() const;
	virtual std::shared_ptr<IndexBase> getPtr(size_t n) const = 0;

	virtual size_t getStepSize(size_t n) const = 0;
	
	virtual operator size_t() const;

	virtual std::string id() const { return std::to_string( mId ); }
    protected:

	std::shared_ptr<RangeBase> mRangePtr;
	size_t mPos;
	size_t mId;
	//bool mLocked = false;
    };

    template <typename MetaType>
    class IndexInterface : public IndexBase
    {
    public:

	DEFAULT_MEMBERS(IndexInterface);
	
	IndexInterface(const std::shared_ptr<RangeBase>& rangePtr, size_t pos);
	virtual MetaType meta() const = 0;
	virtual IndexInterface& at(const MetaType& meta) = 0;
    };
    
}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
    /*****************
     *   IndexBase   *
     *****************/

    IndexBase::IndexBase(const std::shared_ptr<RangeBase>& range,
			 size_t pos) : mRangePtr(range),
				       mPos(pos)
    {
	mId = indexId();
    }
    
    bool IndexBase::operator==(const IndexBase& in) const
    {
	return in.mPos == mPos and in.mRangePtr.get() == mRangePtr.get();
    }
    
    bool IndexBase::operator!=(const IndexBase& in) const
    {
	return in.mPos != mPos or in.mRangePtr.get() != mRangePtr.get();
    }
    
    size_t IndexBase::pos() const
    {
	return mPos;
    }

    size_t IndexBase::max() const
    {
	return mRangePtr->size();
    }
    /*
    bool IndexBase::locked() const
    {
	return mLocked;
    }
    
    IndexBase& IndexBase::lock(std::shared_ptr<IndexBase>& idx)
    {
	mLocked = (idx.get() == this);
	return *this;
    }
    */

    std::shared_ptr<RangeBase> IndexBase::rangePtr() const
    {
	return mRangePtr;
    }
    
    IndexBase::operator size_t() const
    {
	return pos();
    }
    
    /**********************
     *   IndexInterface   *	     
     **********************/

    template <typename MetaType>
    IndexInterface<MetaType>::IndexInterface(const std::shared_ptr<RangeBase>& rangePtr, size_t pos) :
	IndexBase(rangePtr, pos) {}
    
}

#endif
