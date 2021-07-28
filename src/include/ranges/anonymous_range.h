// -*- C++ -*-

#ifndef __anonymous_range_h__
#define __anonymous_range_h__

#include <cstdlib>
#include <map>
#include "rbase_def.h"
#include "ranges/range_base.h"
#include "ranges/rpheader.h"
#include "ranges/x_to_string.h"
#include "ranges/type_map.h"

namespace CNORXZ
{

    typedef GenSingleIndex<size_t,SpaceType::ANON,MUI> AnonymousIndex;

    //template <class R>
    //using SIZET = size_t;

    typedef GenSingleRange<size_t,SpaceType::ANON,MUI> AnonymousRange;
    
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

	AnonymousRangeFactory(const vector<std::shared_ptr<RangeBase>>& origs);
	
	template <class Range>
	void append(std::shared_ptr<Range> r);
	
	std::shared_ptr<RangeBase> create();

    private:
	
	std::shared_ptr<RangeBase> checkIfCreated(const vector<std::shared_ptr<RangeBase> >& pvec);
	
	static std::map<std::shared_ptr<RangeBase>,vector<std::intptr_t> > mAleadyCreated;

	bool mProductCreated = false;
    };

    template <>
    class GenSingleRange<size_t,SpaceType::ANON,MUI> : public RangeInterface<AnonymousIndex>
    {
    public:

	static constexpr bool defaultable = true;
	static constexpr size_t ISSTATIC = 0;
	static constexpr size_t SIZE = MUI;
	static constexpr bool HASMETACONT = false;
	
	typedef RangeBase RB;
	typedef typename RangeInterface<AnonymousIndex>::IndexType IndexType;
	typedef GenSingleRange<size_t,SpaceType::ANON,MUI> RangeType;
	typedef size_t MetaType;
        typedef AnonymousRangeFactory FType;
        
	virtual size_t size() const final;
	virtual size_t dim() const final;

	size_t anonymousDim() const;
	size_t get(size_t pos) const;
	size_t getMeta(size_t metaPos) const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;

	virtual SpaceType spaceType() const final;
        virtual DataHeader dataHeader() const final;
        
        virtual vector<size_t> typeNum() const final;
        virtual size_t cmeta(char* target, size_t pos) const final;
        virtual size_t cmetaSize() const final;
	virtual std::string stringMeta(size_t pos) const final;
	virtual vector<char> data() const final;
	
	std::shared_ptr<RangeBase> sub(size_t num) const;

	template <class Range>
	std::shared_ptr<Range> fullsub(size_t num) const;

	template <class... Ranges>
	std::shared_ptr<MultiRange<Ranges...> > scast(SIZET<Ranges>... sizes) const; // save cast

        const vector<std::shared_ptr<RangeBase> >& orig() const;
        
        std::shared_ptr<AnonymousRange> sreplace(const std::shared_ptr<RangeBase> in, size_t num) const;
        std::shared_ptr<AnonymousRange> sreplace(const vector<std::shared_ptr<RangeBase>>& in, size_t num) const;
        std::shared_ptr<AnonymousRange> sreplace(const std::shared_ptr<RangeBase>& in,
                                                 const vector<size_t>& num) const;
	
	bool isEmpty() const;
	
	friend AnonymousRangeFactory;
	
	static AnonymousRangeFactory factory()
	{ return AnonymousRangeFactory(); }
	
    protected:

	GenSingleRange() = default;
	GenSingleRange(const AnonymousRange& in) = default;

	template <class... RangeTypes>
	GenSingleRange(const std::tuple<std::shared_ptr<RangeTypes>...>& origs);
	
	template <class... RangeTypes>
	GenSingleRange(std::shared_ptr<RangeTypes>... origs);

	GenSingleRange(const vector<std::shared_ptr<RangeBase>>& origs);
	
	size_t mSize = 1;
	bool mEmpty = true;
	
	vector<std::shared_ptr<RangeBase> > mOrig;
    };
}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace CNORXZ
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
	std::dynamic_pointer_cast<oType>(mProd)->mEmpty = false;
    }

    /*****************
     *   Functions   *
     *****************/

    std::shared_ptr<AnonymousRange> defaultRange(size_t size = 0);
}
namespace CNORXZ
{
    namespace RangeHelper
    {
	template <>
	inline void resolveSetRange<AnonymousRange>(std::shared_ptr<AnonymousRange>& rp,
						    const vector<std::shared_ptr<RangeBase> >& orig,
						    size_t origpos, size_t size)
	{
	    AnonymousRangeFactory arf;
	    for(size_t op = origpos; op != origpos + size; ++op){
		//VCHECK(op);
		arf.append(orig[op]);
	    }
	    rp = std::dynamic_pointer_cast<AnonymousRange>( arf.create() );
	}


	template <>
	inline void setRangeToVec<AnonymousRange>(vector<std::shared_ptr<RangeBase> >& v,
						  std::shared_ptr<AnonymousRange> r)
	{
	    if(not r->isEmpty()){
		for(size_t i = r->anonymousDim(); i != 0; --i){
		    v.insert(v.begin(), r->sub(i-1));
		}
	    }
	}
    }
}

namespace CNORXZ
{
    /***********************
     *   AnonymousRange    *
     ***********************/

    template <class... RangeTypes>
    GenSingleRange<size_t,SpaceType::ANON,MUI>::GenSingleRange(const std::tuple<std::shared_ptr<RangeTypes>...>& origs) :
	RangeInterface<AnonymousIndex>()
    {
	mOrig.resize(sizeof...(RangeTypes));
	sfor_pn<0,sizeof...(RangeTypes)>
	    ( [&](auto i) { mOrig[i] = std::get<i>(origs); return 0; } );
	mSize = sfor_p<0,sizeof...(RangeTypes)>
	    ( [&](auto i) { return std::get<i>(origs)->size(); },
	      [&](auto a, auto b) { return a * b; } );
	if(sizeof...(RangeTypes)){
	    mEmpty = false;
	}
    }
    
    template <class... RangeTypes>
    GenSingleRange<size_t,SpaceType::ANON,MUI>::GenSingleRange(std::shared_ptr<RangeTypes>... origs) :
	RangeInterface<AnonymousIndex>()
    {
	auto rst = std::make_tuple(origs...);
	mOrig.resize(sizeof...(RangeTypes));
	sfor_pn<0,sizeof...(RangeTypes)>
	    ( [&](auto i) { mOrig[i] = std::get<i>(rst); return 0; } );
	mSize = sfor_p<0,sizeof...(RangeTypes)>
	    ( [&](auto i) { return std::get<i>(rst)->size(); },
	      [&](auto a, auto b) { return a * b; } );
	if(sizeof...(RangeTypes)){
	    mEmpty = false;
	}
    }
   
    template <class Range>
    std::shared_ptr<Range> GenSingleRange<size_t,SpaceType::ANON,MUI>::fullsub(size_t num) const
    {
	return std::dynamic_pointer_cast<Range>( mOrig.at(num) );
    }

    template <class... Ranges>
    std::shared_ptr<MultiRange<Ranges...> > GenSingleRange<size_t,SpaceType::ANON,MUI>::scast(SIZET<Ranges>... sizes) const
    {
	std::tuple<std::shared_ptr<Ranges>...> rtp;
	RangeHelper::resolveRangeType<0>(mOrig, rtp, 0, sizes...);
	MultiRangeFactory<Ranges...> mrf(rtp);
	return std::dynamic_pointer_cast<MultiRange<Ranges...> >( mrf.create() );
    }
    

    
}

#endif
