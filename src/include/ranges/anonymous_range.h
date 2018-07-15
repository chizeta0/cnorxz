// -*- C++ -*-

#ifndef __anonymous_range_h__
#define __anonymous_range_h__

#include <cstdlib>
#include <map>
#include "rbase_def.h"
#include "ranges/range_base.h"
#include "ranges/rpheader.h"

namespace MultiArrayTools
{

    typedef SingleIndex<size_t,SpaceType::NONE> AnonymousIndex;

    template <class R>
    using SIZET = size_t;

    // NOT THREAD SAVE!!
    class AnonymousRangeFactory : public RangeFactoryBase
    {
    public:
	
	typedef AnonymousRange oType;

	AnonymousRangeFactory() = default;

	template <class... RangeTypes>
	AnonymousRangeFactory(const std::tuple<std::shared_ptr<RangeTypes>...>& origs);
	
	template <class... RangeTypes>
	AnonymousRangeFactory(std::shared_ptr<RangeTypes>... origs);

	template <class Range>
	void append(std::shared_ptr<Range> r);
	
	std::shared_ptr<RangeBase> create();

    private:
	std::shared_ptr<RangeBase> checkIfCreated(const std::vector<std::shared_ptr<RangeBase> >& pvec);
	
	static std::map<std::shared_ptr<RangeBase>,std::vector<std::intptr_t> > mAleadyCreated;
    };
    
    class AnonymousRange : public RangeInterface<AnonymousIndex>
    {
    public:

	static constexpr bool defaultable = false;
        static constexpr size_t ISSTATIC = 0;
        static constexpr size_t SIZE = -1;
        static constexpr bool HASMETACONT = false;
	
	typedef RangeBase RB;
	typedef typename RangeInterface<AnonymousIndex>::IndexType IndexType;
	
	virtual size_t size() const override;
	virtual size_t dim() const override;
	
	size_t get(size_t pos) const;
	size_t getMeta(size_t metaPos) const;
	
	virtual IndexType begin() const override;
	virtual IndexType end() const override;

	std::shared_ptr<RangeBase> sub(size_t num) const;

	template <class Range>
	std::shared_ptr<Range> fullsub(size_t num) const;

	template <class... Ranges>
	MultiRange<Ranges...> scast(SIZET<Ranges>... sizes) const; // save cast
	
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

    template <class Range>
    void AnonymousRangeFactory::append(std::shared_ptr<Range> r)
    {
	std::dynamic_pointer_cast<oType>(mProd)->mOrig.push_back(r);
    }

    /*****************
     *   Functions   *
     *****************/

    std::shared_ptr<AnonymousRange> defaultRange(size_t size = 0);
}

namespace MultiArrayHelper
{
    using namespace MultiArrayTools;
    template <>
    inline void resolveSetRange<AnonymousRange>(std::shared_ptr<AnonymousRange> rp,
						std::vector<std::shared_ptr<RangeBase> > orig,
						size_t origpos, size_t size)
    {
    	AnonymousRangeFactory arf;
	for(size_t op = origpos - size + 1; op != origpos + 1; ++op){
	    arf.append(orig[op]);
	}
    	rp = std::dynamic_pointer_cast<AnonymousRange>( arf.create() );
    }
}

namespace MultiArrayTools
{
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
	RPackNum<sizeof...(RangeTypes)-1>::RangesToVec( rst, mOrig );
	RPackNum<sizeof...(RangeTypes)-1>::getSize( rst );
    }

    template <class Range>
    std::shared_ptr<Range> AnonymousRange::fullsub(size_t num) const
    {
	return std::dynamic_pointer_cast<Range>( mOrig.at(num) );
    }

    template <class... Ranges>
    MultiRange<Ranges...> AnonymousRange::scast(SIZET<Ranges>... sizes) const
    {
	std::tuple<Ranges...> rtp;
	RPackNum<sizeof...(Ranges)-1>::resolveRangeType(mOrig, rtp, mOrig.size()-1, sizes...);
	MultiRangeFactory<Ranges...> mrf(rtp);
	return std::dynamic_pointer_cast<MultiRange<Ranges...> >( mrf->create() );
    }
    

    
}

#endif
