// -*- C++ -*-

#ifndef __anonymous_range_h__
#define __anonymous_range_h__

#include <cstdlib>
#include <map>
#include "rbase_def.h"
#include "ranges/range_base.h"
#include "ranges/rpheader.h"
#include "ranges/x_to_string.h"

namespace MultiArrayTools
{

    typedef SingleIndex<size_t,SpaceType::ANON> AnonymousIndex;

    template <class R>
    using SIZET = size_t;

    typedef SingleRange<size_t,SpaceType::ANON> AnonymousRange;
    
    // NOT THREAD SAVE!!
    class AnonymousRangeFactory : public RangeFactoryBase
    {
    public:
	
	typedef AnonymousRange oType;

	AnonymousRangeFactory();

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

	bool mProductCreated = false;
    };

    template <>
    class SingleRange<size_t,SpaceType::ANON> : public RangeInterface<AnonymousIndex>
    {
    public:

	static constexpr bool defaultable = true;
	static constexpr size_t ISSTATIC = 0;
	static constexpr size_t SIZE = -1;
	static constexpr bool HASMETACONT = false;
	
	typedef RangeBase RB;
	typedef typename RangeInterface<AnonymousIndex>::IndexType IndexType;
	typedef SingleRange<size_t,SpaceType::ANON> RangeType;
	typedef size_t MetaType;
	
	virtual size_t size() const final;
	virtual size_t dim() const final;

	size_t anonymousDim() const;
	size_t get(size_t pos) const;
	size_t getMeta(size_t metaPos) const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;

	virtual std::string stringMeta(size_t pos) const final;
	virtual std::vector<char> data() const final;
	
	std::shared_ptr<RangeBase> sub(size_t num) const;

	template <class Range>
	std::shared_ptr<Range> fullsub(size_t num) const;

	template <class... Ranges>
	std::shared_ptr<MultiRange<Ranges...> > scast(SIZET<Ranges>... sizes) const; // save cast
	
	friend AnonymousRangeFactory;
	
	static AnonymousRangeFactory factory()
	{ return AnonymousRangeFactory(); }
	
    protected:

	SingleRange() = default;
	SingleRange(const AnonymousRange& in) = default;

	template <class... RangeTypes>
	SingleRange(const std::tuple<std::shared_ptr<RangeTypes>...>& origs);
	
	template <class... RangeTypes>
	SingleRange(std::shared_ptr<RangeTypes>... origs);

	size_t mSize = 1;
	
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
	if(mProductCreated){
	    
	    mProd = std::shared_ptr<oType>( new AnonymousRange( *std::dynamic_pointer_cast<oType>(mProd) ) );
	    mProductCreated = false;
	}
	std::dynamic_pointer_cast<oType>(mProd)->mOrig.push_back(r);
	std::dynamic_pointer_cast<oType>(mProd)->mSize *= r->size();
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
    inline void resolveSetRange<AnonymousRange>(std::shared_ptr<AnonymousRange>& rp,
						const std::vector<std::shared_ptr<RangeBase> >& orig,
						size_t origpos, size_t size)
    {
    	AnonymousRangeFactory arf;
	//VCHECK(size);
	for(size_t op = origpos; op != origpos + size; ++op){
	    //VCHECK(op);
	    arf.append(orig[op]);
	}
    	rp = std::dynamic_pointer_cast<AnonymousRange>( arf.create() );
    }

    template <>
    inline void setRangeToVec<AnonymousRange>(std::vector<std::shared_ptr<RangeBase> >& v,
					      std::shared_ptr<AnonymousRange> r)
    {
	for(size_t i = 0; i != r->dim(); ++i){
	    v.insert(v.begin(), r->sub(i));
	}
    }

}

namespace MultiArrayTools
{
    /***********************
     *   AnonymousRange    *
     ***********************/

    template <class... RangeTypes>
    SingleRange<size_t,SpaceType::ANON>::SingleRange(const std::tuple<std::shared_ptr<RangeTypes>...>& origs) :
	RangeInterface<AnonymousIndex>()
    {
	RPackNum<sizeof...(RangeTypes)-1>::RangesToVec( origs, mOrig );
	mSize = RPackNum<sizeof...(RangeTypes)-1>::getSize( origs );
    }
    
    template <class... RangeTypes>
    SingleRange<size_t,SpaceType::ANON>::SingleRange(std::shared_ptr<RangeTypes>... origs) :
	RangeInterface<AnonymousIndex>()
    {
	auto rst = std::make_tuple(origs...);
	RPackNum<sizeof...(RangeTypes)-1>::RangesToVec( rst, mOrig );
	mSize = RPackNum<sizeof...(RangeTypes)-1>::getSize( rst );
    }

    template <class Range>
    std::shared_ptr<Range> SingleRange<size_t,SpaceType::ANON>::fullsub(size_t num) const
    {
	return std::dynamic_pointer_cast<Range>( mOrig.at(num) );
    }

    template <class... Ranges>
    std::shared_ptr<MultiRange<Ranges...> > SingleRange<size_t,SpaceType::ANON>::scast(SIZET<Ranges>... sizes) const
    {
	std::tuple<std::shared_ptr<Ranges>...> rtp;
	RPackNum<sizeof...(Ranges)-1>::resolveRangeType(mOrig, rtp, 0, sizes...);
	MultiRangeFactory<Ranges...> mrf(rtp);
	return std::dynamic_pointer_cast<MultiRange<Ranges...> >( mrf.create() );
    }
    

    
}

#endif
