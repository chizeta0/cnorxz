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

	typedef IndexInterface<SingleIndex<U,TYPE>,U> IB;
	typedef U MetaType;
	typedef SingleRange<U,TYPE> RangeType;
	
	//DEFAULT_MEMBERS_X(SingleIndex);
	
	SingleIndex(const std::shared_ptr<SingleRange<U,TYPE> >& range);

	SingleIndex& operator=(size_t pos) { IB::operator=(pos); return *this; } 

	std::shared_ptr<RangeType> range() const { return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr ); }
	
    private:

	friend IB;

	// ==== >>>>> STATIC POLYMORPHISM <<<<< ====
	
	static IndexType S_type(SingleIndex const* i)
	{
	    return IndexType::SINGLE;
	}
	
	static SingleIndex& S_ass_op(SingleIndex* i, size_t pos)
	{
	    i->mPos = pos;
	    return *i;
	}

	static SingleIndex& S_pp_op(SingleIndex* i)
	{
	    ++i->mPos;
	    return *i;
	}

	static SingleIndex& S_mm_op(SingleIndex* i)
	{
	    --i->mPos;
	    return *i;
	}

	static int S_pp(SingleIndex* i, std::intptr_t idxPtrNum)
	{
	    ++(*i);
	    return 1;
	}

	static int S_mm(SingleIndex* i, std::intptr_t idxPtrNum)
	{
	    --(*i);
	    return 1;
	}
	
	static U S_meta(SingleIndex const* i)
	{
	    return std::dynamic_pointer_cast<SingleRange<U,TYPE> const>( i->mRangePtr )->get( i->pos() );
	}

	static SingleIndex& S_at(SingleIndex* i, const U& metaPos)
	{
	    (*i) = std::dynamic_pointer_cast<SingleRange<U,TYPE> const>( i->mRangePtr )->getMeta( metaPos );
	    return *i;
	}
	
	static size_t S_dim(SingleIndex const* i) // = 1
	{
	    return 1;
	}

	static bool S_last(SingleIndex const* i)
	{
	    return i->mPos == i->mMax - 1;
	}

	static bool S_first(SingleIndex const* i)
	{
	    return i->mPos == 0;
	}

	static std::shared_ptr<RangeType> S_range(SingleIndex const* i)
	{
	    return std::dynamic_pointer_cast<RangeType>( i->mRangePtr );
	}
	
	template <size_t N>
	static void S_getPtr(SingleIndex* i) {}

	static std::shared_ptr<VIWB> S_getVPtr(SingleIndex const* i, size_t n)
	{
	    return std::shared_ptr<VIWB>();
	}
	
	static size_t S_getStepSize(SingleIndex const* i, size_t n)
	{
	    return 1;
	}
	
	static std::string S_id(SingleIndex const* i) { return std::string("sin") + std::to_string(i->mId); }
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
	typedef SingleIndex<U,TYPE> IndexType;
	//typedef typename RangeInterface<SingleIndex<U,TYPE> >::IndexType IndexType;
	
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
	IndexInterface<SingleIndex<U,TYPE>,U>(range, 0) {}
    
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
	typedef IndexWrapper<IndexType> IW;
	return std::make_shared<IW>
	    ( std::make_shared<IndexType>
	      ( std::dynamic_pointer_cast<SingleRange<U,TYPE> >
		( std::shared_ptr<RangeBase>( RB::mThis ) ) ) );
    }

}

#include "range_types/header.h"

#endif
