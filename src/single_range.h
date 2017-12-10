// -*- C++ -*-

#ifndef __single_range_h__
#define __single_range_h__

#include <cstdlib>
#include <vector>
#include <memory>

#include "base_def.h"
#include "index_base.h"
#include "range_base.h"

namespace MultiArrayTools
{

    template <typename U, RangeType TYPE>
    class SingleIndex : public IndexInterface<SingleIndex<U,TYPE>,U>
    {
    public:

	typedef IndexBase<SingleIndex<U,TYPE>,U> IB;
	typedef U MetaType;
	typedef SingleRange<U,TYPE> RangeType;
	
	//DEFAULT_MEMBERS_X(SingleIndex);
	
	SingleIndex(const std::shared_ptr<SingleRange<U,TYPE> >& range);

	static IndexType S_type(SingleIndex* i) const;
	
	static SingleIndex& S_ass_op(SingleIndex* i, size_t pos);
	static SingleIndex& S_pp_op(SingleIndex* i);
	static SingleIndex& S_mm_op(SingleIndex* i);

	static int S_pp(SingleIndex* i, std::shared_ptr<VIWB>& idxPtr);
	static int S_mm(SingleIndex* i, std::shared_ptr<VIWB>& idxPtr);
	
	static U S_meta(SingleIndex* i) const;
	static SingleIndex& S_at(SingleIndex* i, const U& metaPos);
	
	static size_t S_dim(SingleIndex* i) const; // = 1
	static bool S_last(SingleIndex* i) const;
	static bool S_first(SingleIndex* i) const;

	template <size_t N>
	static void S_getPtr(SingleIndex* i) const;

	std::shared_ptr<VIWB> getVPtr(size_t n) const;
	
	static size_t S_getStepSize(SingleIndex* i, size_t n) const;
	
	static std::string S_id(SingleIndex* i) const { return std::string("sin") + std::to_string(IB::mId); }
    };

    template <typename U, RangeType TYPE>
    class SingleRangeFactory : public RangeFactoryBase
    {
    public:
	
	typedef SingleRange<U,TYPE> oType;

	SingleRangeFactory() = delete;
	SingleRangeFactory(const std::vector<U>& space);
	std::shared_ptr<RangeBase> create();
	
    };
    
    template <typename U, RangeType TYPE>
    class SingleRange : public RangeInterface<SingleIndex<U,TYPE> >
    {
    public:
	typedef RangeBase RB;
	typedef typename RangeInterface<SingleIndex<U,TYPE> >::IndexType IndexType;
	
	virtual size_t size() const override;
	virtual size_t dim() const override;
	
	const U& get(size_t pos) const;
	size_t getMeta(const U& metaPos) const;
	
	virtual IndexType begin() const override;
	virtual IndexType end() const override;
	virtual std::shared_ptr<VIWB> index() const override;
	
	friend SingleRangeFactory<U,TYPE>;
	
    protected:

	SingleRange() = delete;
	SingleRange(const SingleRange& in) = delete;
	
	SingleRange(const std::vector<U>& space);

	std::vector<U> mSpace;
    };

}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
    /******************
     *  SingleIndex   *	     
     ******************/

    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>::SingleIndex(const std::shared_ptr<SingleRange<U,TYPE> >& range) :
	IndexInterface<U>(range, 0) {}

    template <typename U, RangeType TYPE>
    IndexType SingleIndex<U,TYPE>::S_type() const
    {
	return IndexType::SINGLE;
    }
	
    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::S_ass_op(size_t pos)
    {
	IB::mPos = pos;
	return *this;
    }
    
    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::S_pp_op()
    {
	++IB::mPos;
	return *this;
    }

    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::S_mm_op()
    {
	--IB::mPos;
	return *this;
    }

    template <typename U, RangeType TYPE>
    int SingleIndex<U,TYPE>::S_pp(std::shared_ptr<VIWB>& idxPtr)
    {
	++(*this);
	return 1;
    }

    template <typename U, RangeType TYPE>
    int SingleIndex<U,TYPE>::S_mm(std::shared_ptr<VIWB>& idxPtr)
    {
	--(*this);
	return 1;
    }
    
    template <typename U, RangeType TYPE>
    U SingleIndex<U,TYPE>::S_meta() const
    {
	return std::dynamic_pointer_cast<SingleRange<U,TYPE> const>( IB::mRangePtr )->get( IB::pos() );
    }

    template <typename U, RangeType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::S_at(const U& metaPos)
    {
	operator=( std::dynamic_pointer_cast<SingleRange<U,TYPE> const>( IB::mRangePtr )->getMeta( metaPos ) );
	return *this;
    }

    template <typename U, RangeType TYPE>
    size_t SingleIndex<U,TYPE>::S_dim() const
    {
	return 1;
    }

    template <typename U, RangeType TYPE>
    bool SingleIndex<U,TYPE>::S_last() const
    {
	return IB::mPos == IB::mRangePtr->size() - 1;
    }

    template <typename U, RangeType TYPE>
    bool SingleIndex<U,TYPE>::S_first() const
    {
	return IB::mPos == 0;
    }

    template <typename U, RangeType TYPE>
    template <size_t N>
    void SingleIndex<U,TYPE>::S_getPtr() const
    {
	return;
    }

    template <typename U, RangeType TYPE>
    std::shared_ptr<VIWB> SingleIndex<U,TYPE>::getVPtr(size_t n) const
    {
	return std::shared_ptr<VIWB>();
    }
    
    template <typename U, RangeType TYPE>    
    size_t SingleIndex<U,TYPE>::S_getStepSize(size_t n) const
    {
	return 1;
    }
    
    /********************
     *   SingleRange    *
     ********************/

    template <typename U, RangeType TYPE>
    SingleRangeFactory<U,TYPE>::SingleRangeFactory(const std::vector<U>& space)
    {
	mProd = std::shared_ptr<oType>( new SingleRange<U,TYPE>( space ) );
    }

    template <typename U, RangeType TYPE>
    std::shared_ptr<RangeBase> SingleRangeFactory<U,TYPE>::create()
    {
	setSelf();
	return mProd;
    }
    
    /********************
     *   SingleRange    *
     ********************/
    
    template <typename U, RangeType TYPE>
    SingleRange<U,TYPE>::SingleRange(const std::vector<U>& space) : RangeInterface<SingleIndex<U,TYPE> >(),
	mSpace(space) {}
    
    template <typename U, RangeType TYPE>
    const U& SingleRange<U,TYPE>::get(size_t pos) const
    {
	return mSpace[pos];
    }

    template <typename U, RangeType TYPE>
    size_t SingleRange<U,TYPE>::getMeta(const U& metaPos) const
    {
	size_t cnt = 0;
	for(auto& x: mSpace){
	    if(x == metaPos){
		return cnt;
	    }
	    ++cnt;
	}
	return cnt;
    }

    template <typename U, RangeType TYPE>
    size_t SingleRange<U,TYPE>::size() const
    {
	return mSpace.size();
    }

    template <typename U, RangeType TYPE>
    size_t SingleRange<U,TYPE>::dim() const
    {
	return 1;
    }

    template <typename U, RangeType TYPE>
    typename SingleRange<U,TYPE>::IndexType SingleRange<U,TYPE>::begin() const
    {
	SingleIndex<U,TYPE> i( std::dynamic_pointer_cast<SingleRange<U,TYPE> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    template <typename U, RangeType TYPE>
    typename SingleRange<U,TYPE>::IndexType SingleRange<U,TYPE>::end() const
    {
	SingleIndex<U,TYPE> i( std::dynamic_pointer_cast<SingleRange<U,TYPE> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }

    // put this in the interface class !!!
    template <typename U, RangeType TYPE>
    std::shared_ptr<VIWB> SingleRange<U,TYPE>::index() const
    {
	return std::make_shared<VIWB>
	    ( std::make_shared<SingleIndex<U,TYPE> >
	    ( std::dynamic_pointer_cast<SingleRange<U,TYPE> >
	      ( std::shared_ptr<RangeBase>( RB::mThis ) ) ) );
    }

}

#include "range_types/header.h"

#endif
