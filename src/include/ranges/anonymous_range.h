// -*- C++ -*-

#ifndef __anonymous_range_h__
#define __anonymous_range_h__

#include <cstdlib>
//#include "base_def.h"
#include "ranges/range_base.h"
#include "ranges/rpheader.h"

namespace MultiArrayTools
{

    typedef SingleIndex<size_t,SpaceType::NONE> AnonymousIndex;

    class AnonymousRangeFactory : public RangeFactoryBase
    {
    public:
	
	typedef AnonymousRange oType;

	AnonymousRangeFactory() = delete;

	template <class... RangeTypes>
	AnonymousRangeFactory(const std::tuple<std::shared_ptr<RangeTypes>...>& origs);
	
	template <class... RangeTypes>
	AnonymousRangeFactory(std::shared_ptr<RangeTypes>... origs);

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
		
	friend AnonymousRangeFactory;
	
    protected:

	AnonymousRange() = delete;
	AnonymousRange(const AnonymousRange& in) = delete;

	template <class... RangeTypes>
	AnonymousRange(const std::tuple<std::shared_ptr<RangeTypes>...>& origs);
	
	template <class... RangeTypes>
	AnonymousRange(std::shared_ptr<RangeTypes>... origs);

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

    template <class... RangeTypes>
    AnonymousRangeFactory::AnonymousRangeFactory(const std::tuple<std::shared_ptr<RangeTypes>...>& origs)
    {
	mProd = std::shared_ptr<oType>( new AnonymousRange( origs ) );
    }

    template <class... RangeTypes>
    AnonymousRangeFactory::AnonymousRangeFactory(std::shared_ptr<RangeTypes>... origs)
    {
	mProd = std::shared_ptr<oType>( new AnonymousRange( origs... ) );
    }
    
    /***********************
     *   AnonymousRange    *
     ***********************/

    template <class... RangeTypes>
    AnonymousRange::AnonymousRange(const std::tuple<std::shared_ptr<RangeTypes>...>& origs) :
	RangeInterface<AnonymousIndex>()
    {
	mOrig.resize(sizeof...(RangeTypes));
	RPackNum<sizeof...(RangeTypes)-1>::RangesToVec( origs, mOrig );
	RPackNum<sizeof...(RangeTypes)-1>::getSize( origs );
    }
    
    template <class... RangeTypes>
    AnonymousRange::AnonymousRange(std::shared_ptr<RangeTypes>... origs) :
	RangeInterface<AnonymousIndex>()
    {
	auto rst = std::make_tuple(origs...);
	mOrig.resize(sizeof...(RangeTypes));
	//RPackNum<sizeof...(RangeTypes)-1>::RangesToVec( rst, mOrig );
	//RPackNum<sizeof...(RangeTypes)-1>::getSize( rst );
    }

    /*****************
     *   Functions   *
     *****************/

    std::shared_ptr<AnonymousRange> defaultRange(size_t size = 0);
}

#endif
