// -*- C++ -*-

#ifndef __single_range_h__
#define __single_range_h__

#include <cstdlib>
#include <vector>
#include <memory>

//#include "base_def.h"
#include "ranges/index_base.h"
#include "ranges/range_base.h"

#include "xfor/xfor.h"

using MultiArrayHelper::For;

namespace MultiArrayTools
{

    template <typename U, SpaceType TYPE>
    class SingleIndex : public IndexInterface<SingleIndex<U,TYPE>,U>
    {
    public:

	typedef IndexInterface<SingleIndex<U,TYPE>,U> IB;
	typedef U MetaType;
	typedef SingleRange<U,TYPE> RangeType;
	typedef SingleIndex IType;

	//DEFAULT_MEMBERS_X(SingleIndex);
	
	SingleIndex(const std::shared_ptr<SingleRange<U,TYPE> >& range);

	static constexpr IndexType sType() { return IndexType::SINGLE; }
	static constexpr size_t totalDim() { return 1; }
	static constexpr size_t sDim() { return 1; }
	
	// ==== >>>>> STATIC POLYMORPHISM <<<<< ====
	
	IndexType type() const;
	
	SingleIndex& operator=(size_t pos);
	SingleIndex& operator++();
	SingleIndex& operator--();

	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);
	
	U meta();
	SingleIndex& at(const U& metaPos);
	
	size_t dim(); // = 1
	bool last();
	bool first();

	std::shared_ptr<RangeType> range();
	
	template <size_t N>
	void getPtr();

	size_t getStepSize(size_t n);
	
	std::string id() const;
	void print(size_t offset);

	template <class Expr>
	auto ifor(Expr ex) const
	    -> For<SingleIndex<U,TYPE>,Expr>;

	template <class Expr>
	auto iforh(Expr ex) const
	    -> For<SingleIndex<U,TYPE>,Expr,ForType::HIDDEN>;

    };

    template <typename U, SpaceType TYPE>
    class SingleRangeFactory : public RangeFactoryBase
    {
    public:
	
	typedef SingleRange<U,TYPE> oType;

	SingleRangeFactory() = delete;
	SingleRangeFactory(const std::vector<U>& space);
	std::shared_ptr<RangeBase> create();
	
    };
    
    template <typename U, SpaceType TYPE>
    class SingleRange : public RangeInterface<SingleIndex<U,TYPE> >
    {
    public:
	typedef RangeBase RB;
	typedef SingleIndex<U,TYPE> IndexType;
	typedef SingleRange RangeType;
	//typedef typename RangeInterface<SingleIndex<U,TYPE> >::IndexType IndexType;
	
	virtual size_t size() const override;
	virtual size_t dim() const override;
	
	const U& get(size_t pos) const;
	size_t getMeta(const U& metaPos) const;
	
	virtual IndexType begin() const override;
	virtual IndexType end() const override;
		
	friend SingleRangeFactory<U,TYPE>;

	static constexpr bool defaultable = false;
	static constexpr size_t ISSTATIC = 0;
	static constexpr size_t SIZE = -1;

	
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

    template <typename U, SpaceType TYPE>
    SingleIndex<U,TYPE>::SingleIndex(const std::shared_ptr<SingleRange<U,TYPE> >& range) :
	IndexInterface<SingleIndex<U,TYPE>,U>(range, 0) {}

    template <typename U, SpaceType TYPE>
    IndexType SingleIndex<U,TYPE>::type() const
    {
	return IndexType::SINGLE;
    }

    template <typename U, SpaceType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::operator=(size_t pos)
    {
	IB::mPos = pos;
	return *this;
    }

    template <typename U, SpaceType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::operator++()
    {
	++IB::mPos;
	return *this;
    }

    template <typename U, SpaceType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::operator--()
    {
	--IB::mPos;
	return *this;
    }

    template <typename U, SpaceType TYPE>
    int SingleIndex<U,TYPE>::pp(std::intptr_t idxPtrNum)
    {
	++(*this);
	return 1;
    }

    template <typename U, SpaceType TYPE>
    int SingleIndex<U,TYPE>::mm(std::intptr_t idxPtrNum)
    {
	--(*this);
	return 1;
    }

    template <typename U, SpaceType TYPE>
    U SingleIndex<U,TYPE>::meta()
    {
	return std::dynamic_pointer_cast<SingleRange<U,TYPE> const>( IB::mRangePtr )->get( IB::pos() );
    }

    template <typename U, SpaceType TYPE>
    SingleIndex<U,TYPE>& SingleIndex<U,TYPE>::at(const U& metaPos)
    {
	(*this) = std::dynamic_pointer_cast<SingleRange<U,TYPE> const>( IB::mRangePtr )->getMeta( metaPos );
	return *this;
    }

    template <typename U, SpaceType TYPE>
    size_t SingleIndex<U,TYPE>::dim() // = 1
    {
	return 1;
    }

    template <typename U, SpaceType TYPE>
    bool SingleIndex<U,TYPE>::last()
    {
	return IB::mPos == IB::mMax - 1;
    }

    template <typename U, SpaceType TYPE>
    bool SingleIndex<U,TYPE>::first()
    {
	return IB::mPos == 0;
    }

    template <typename U, SpaceType TYPE>
    std::shared_ptr<typename SingleIndex<U,TYPE>::RangeType> SingleIndex<U,TYPE>::range()
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr );
    }

    template <typename U, SpaceType TYPE>
    template <size_t N>
    void SingleIndex<U,TYPE>::getPtr() {}
    
    template <typename U, SpaceType TYPE>
    size_t SingleIndex<U,TYPE>::getStepSize(size_t n)
    {
	return 1;
    }

    template <typename U, SpaceType TYPE>
    std::string SingleIndex<U,TYPE>::id() const
    {
	return std::string("sin") + std::to_string(IB::mId);
    }

    template <typename U, SpaceType TYPE>
    void SingleIndex<U,TYPE>::print(size_t offset)
    {
	if(offset == 0){
	    std::cout << " === " << std::endl;
	}
	for(size_t j = 0; j != offset; ++j) { std::cout << "\t"; }
	std::cout << id() << "[" << reinterpret_cast<std::intptr_t>(this)
		  << "](" << IB::mRangePtr << "): " << meta() << std::endl;
    }

    template <typename U, SpaceType TYPE>
    template <class Expr>
    auto SingleIndex<U,TYPE>::ifor(Expr ex) const
	-> For<SingleIndex<U,TYPE>,Expr>
    {
	//static const size_t LAYER = typename Expr::LAYER; 
	return For<SingleIndex<U,TYPE>,Expr>(this, ex);
    }

    template <typename U, SpaceType TYPE>
    template <class Expr>
    auto SingleIndex<U,TYPE>::iforh(Expr ex) const
	-> For<SingleIndex<U,TYPE>,Expr,ForType::HIDDEN>
    {
	//static const size_t LAYER = typename Expr::LAYER; 
	return For<SingleIndex<U,TYPE>,Expr,ForType::HIDDEN>(this, ex);
    }

    
    /********************
     *   SingleRange    *
     ********************/

    template <typename U, SpaceType TYPE>
    SingleRangeFactory<U,TYPE>::SingleRangeFactory(const std::vector<U>& space)
    {
	mProd = std::shared_ptr<oType>( new SingleRange<U,TYPE>( space ) );
    }

    template <typename U, SpaceType TYPE>
    std::shared_ptr<RangeBase> SingleRangeFactory<U,TYPE>::create()
    {
	setSelf();
	return mProd;
    }
    
    /********************
     *   SingleRange    *
     ********************/
    
    template <typename U, SpaceType TYPE>
    SingleRange<U,TYPE>::SingleRange(const std::vector<U>& space) : RangeInterface<SingleIndex<U,TYPE> >(),
	mSpace(space) {}
    
    template <typename U, SpaceType TYPE>
    const U& SingleRange<U,TYPE>::get(size_t pos) const
    {
	return mSpace[pos];
    }

    template <typename U, SpaceType TYPE>
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

    template <typename U, SpaceType TYPE>
    size_t SingleRange<U,TYPE>::size() const
    {
	return mSpace.size();
    }

    template <typename U, SpaceType TYPE>
    size_t SingleRange<U,TYPE>::dim() const
    {
	return 1;
    }

    template <typename U, SpaceType TYPE>
    typename SingleRange<U,TYPE>::IndexType SingleRange<U,TYPE>::begin() const
    {
	SingleIndex<U,TYPE> i( std::dynamic_pointer_cast<SingleRange<U,TYPE> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    template <typename U, SpaceType TYPE>
    typename SingleRange<U,TYPE>::IndexType SingleRange<U,TYPE>::end() const
    {
	SingleIndex<U,TYPE> i( std::dynamic_pointer_cast<SingleRange<U,TYPE> >
			       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }
}

#include "range_types/header.h"

#endif
