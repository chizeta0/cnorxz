// -*- C++ -*-

#ifndef __anonymous_range_h__
#define __anonymous_range_h__

#include <cstdlib>
#include "base_def.h"

namespace MultiArrayTools
{

    typedef SingleIndex<size_t,RangeType::NONE> AnonymousIndex;

    template <typename U, RangeType TYPE>
    class AnonymousRangeFactory : public RangeFactoryBase
    {
    public:
	
	typedef AnonymousRange oType;

	AnonymousRangeFactory() = delete;

	template <class... Ranges>
	AnonymousRangeFactory(const std::tuple<std::shared_ptr<Ranges>...>& origs);
	
	template <class... Ranges>
	AnonymousRangeFactory(std::shared_ptr<Ranges>... origs);

	std::shared_ptr<RangeBase> create();
	
    };

    
    class AnonymousRange : public RangeInterface<AnonymousIndex>
    {
	typedef RangeBase RB;
	typedef typename RangeInterface<AnonymousIndex>::IndexType IndexType;
	
	virtual size_t size() const override;
	virtual size_t dim() const override;
	
	size_t get(size_t pos) const;
	size_t getMeta(size_t metaPos) const;
	
	virtual IndexType begin() const override;
	virtual IndexType end() const override;
	virtual std::shared_ptr<IndexBase> index() const override;
	
	friend AnonymousRangeFactory;
	
    protected:

	AnonymousRange() = delete;
	AnonymousRange(const AnonymousRange& in) = delete;

	template <class Ranges...>
	AnonymousRange(const std::tuple<std::shared_ptr<Ranges>...>& origs)
	
	template <class Ranges...>
	AnonymousRange(std::shared_ptr<Ranges>... origs);

	size_t mSize;
	
	std::vector<std::shared_ptr<RangeBase> > mOrig;
    };

}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{

    /***********************
     *   AnonymousRange    *
     ***********************/

    template <class... Ranges>
    AnonymousRangeFactory::AnonymousRangeFactory(const std::tuple<std::shared_ptr<Ranges>...>& origs)
    {
	mProd = std::shared_ptr<oType>( new AnonymousRange( origs ) );
    }

    template <class... Ranges>
    AnonymousRangeFactory::AnonymousRangeFactory(std::shared_ptr<Ranges>... origs)
    {
	mProd = std::shared_ptr<oType>( new AnonymousRange( origs... ) );
    }
        
    std::shared_ptr<RangeBase> AnonymousRangeFactory::create()
    {
	setSelf();
	return mProd;
    }
    
    /***********************
     *   AnonymousRange    *
     ***********************/

    template <class... Ranges>
    AnonymousRange::AnonymousRange(const std::tuple<std::shared_ptr<Ranges>...>& origs) :
	RangeInterface<AnonymousIndex>()
    {
	mOrig.resize(sizeof...(Ranges));
	PackNum<sizeof...(Ranges)-1>::RangesToVec( origs, mOrig );
	PackNum<sizeof...(Ranges)-1>::getSize( rst );
    }
    
    template <class... Ranges>
    AnonymousRange::AnonymousRange(std::shared_ptr<Ranges>... origs) :
	RangeInterface<AnonymousIndex>()
    {
	auto rst = std::make_tuple(origs...);
	mOrig.resize(sizeof...(Ranges));
	PackNum<sizeof...(Ranges)-1>::RangesToVec( rst, mOrig );
	PackNum<sizeof...(Ranges)-1>::getSize( rst );
    }
    
    size_t AnonymousRange::get(size_t pos) const
    {
	return pos;
    }
        
    size_t AnonymousRange::getMeta(size_t metaPos) const
    {
	return metaPos;
    }
        
    size_t AnonymousRange::size() const
    {
	return mSize;
    }
        
    size_t AnonymousRange::dim() const
    {
	return 1;
    }
        
    typename AnonymousRange::IndexType AnonymousRange::begin() const
    {
	SingleIndex i( std::dynamic_pointer_cast<AnonymousRange>
		       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }
    
    typename AnonymousRange::IndexType AnonymousRange::end() const
    {
	SingleIndex i( std::dynamic_pointer_cast<AnonymousRange>
		       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }

    // put this in the interface class !!!
    std::shared_ptr<IndexBase> AnonymousRange::index() const
    {
	return std::make_shared<SingleIndex >
	    ( std::dynamic_pointer_cast<AnonymousRange>
	      ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
    }
}

#endif
