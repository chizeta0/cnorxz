
#include "ranges/dynamic_range.h"

namespace MultiArrayTools
{
    namespace
    {
        using namespace MultiArrayHelper;
    }


    /****************************
     *   DynamicRangeFactory    *
     ****************************/

    
    template <class... RangeTypes>
    DynamicRangeFactory::DynamicRangeFactory(const std::tuple<std::shared_ptr<RangeTypes>...>& origs)
    {
	mProd = std::shared_ptr<oType>( new DynamicRange( origs ) );
    }

    
    template <class... RangeTypes>
    DynamicRangeFactory::DynamicRangeFactory(std::shared_ptr<RangeTypes>... origs)
    {
	mProd = std::shared_ptr<oType>( new DynamicRange( origs... ) );
    }

    
    
    template <class Range>
    void DynamicRangeFactory::append(std::shared_ptr<Range> r)
    {
	if(mProductCreated){
	    
	    mProd = std::shared_ptr<oType>( new DynamicRange( *std::dynamic_pointer_cast<oType>(mProd) ) );
	    mProductCreated = false;
	}
	std::dynamic_pointer_cast<oType>(mProd)->mOrig.push_back(r);
	std::dynamic_pointer_cast<oType>(mProd)->mSize *= r->size();
	std::dynamic_pointer_cast<oType>(mProd)->mEmpty = false;
    }

    
    /*********************
     *   DynamicIndex    *
     *********************/

    
    
    template <class... Indices>
    DynamicIndex& DynamicIndex::operator()(const std::shared_ptr<Indices>&... is)
    {
        mIvecInit = true;
	vector<std::shared_ptr<IndexW>> tmp =
	    { std::make_shared<IndexWrapper<Indices>>(is)... };
	
	assert(mIVec.size() == tmp.size());
	for(size_t i = 0; i != mIVec.size(); ++i){
	    mIVec[i].first = tmp[i];
	}
	sync();
	return *this;
    }
    
    
    template <size_t N>
    void DynamicIndex::getPtr() {}
    

    struct ForMaker
    {
	template <class IVecT>
	static inline auto mk(size_t i, size_t step, DynamicExpression ex, 
			      const IVecT& ivec, bool hidden = false)
	    -> DynamicExpression
	{
	    if(i == 0) {
		auto& ii = *ivec[0].first;
		return hidden ? ii.iforh(step*ivec[i].second, ex) :
		    ii.ifor(step*ivec[i].second, ex);
	    }
	    else {
		auto& ii = *ivec[i].first;
		return mk(i-1, step,
			  (hidden ? ii.iforh(step*ivec[i].second, ex) :
			   ii.ifor(step*ivec[i].second, ex)),
			  ivec, hidden);

	    }
	}
    };

    
    template <class Expr>
    DynamicExpression DynamicIndex::ifor(size_t step, Expr ex) const
    {
	if(mIVec.size() == 0){
	    return ex;
	}
	else if(mIVec.size() == 1){
	    return mIVec.back().first->ifor(step,ex);
	}
	else {
	    return ForMaker::mk(mIVec.size()-2, step,
                                mIVec.back().first->ifor(step,ex),mIVec);
	}
    }

    
    template <class Expr>
    DynamicExpression DynamicIndex::iforh(size_t step, Expr ex) const
    {
	if(mIVec.size() == 0){
	    return ex;
	}
	else if(mIVec.size() == 1){
	    return mIVec.back().first->iforh(step,ex);
	}
	else {
	    return ForMaker::mk(mIVec.size()-2, step,
                                mIVec.back().first->iforh(step,ex),
                                mIVec, true);
	}
    }

    
    template <class Expr>
    DynamicExpression DynamicIndex::pifor(size_t step, Expr ex) const
    {
        return ifor(step, ex); // no multithreading here at the moment...
    }

    /***********************
     *   DynamicRange    *
     ***********************/

    template <class... RangeTypes>
    DynamicRange::DynamicRange(const std::tuple<std::shared_ptr<RangeTypes>...>& origs) :
	RangeInterface<DynamicIndex>()
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
    DynamicRange::DynamicRange(std::shared_ptr<RangeTypes>... origs) :
	RangeInterface<DynamicIndex>()
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
    std::shared_ptr<Range> DynamicRange::fullsub(size_t num) const
    {
	return std::dynamic_pointer_cast<Range>( mOrig.at(num) );
    }

    
    template <class... Ranges>
    std::shared_ptr<MultiRange<Ranges...> > DynamicRange::scast(SIZET<Ranges>... sizes) const
    {
	std::tuple<std::shared_ptr<Ranges>...> rtp;
	RangeHelper::resolveRangeType<0>(mOrig, rtp, 0, sizes...);
	MultiRangeFactory<Ranges...> mrf(rtp);
	return std::dynamic_pointer_cast<MultiRange<Ranges...> >( mrf.create() );
    }

    
} // end namespace MultiArrayTools

namespace MultiArrayTools
{
    namespace RangeHelper
    {
	template <>
	inline void resolveSetRange<DynamicRange>(std::shared_ptr<DynamicRange>& rp,
						  const vector<std::shared_ptr<RangeBase> >& orig,
						  size_t origpos, size_t size)
	{
	    DynamicRangeFactory arf;
	    for(size_t op = origpos; op != origpos + size; ++op){
		//VCHECK(op);
		arf.append(orig[op]);
	    }
	    rp = std::dynamic_pointer_cast<DynamicRange>( arf.create() );
	}
    

	template <>
	inline void setRangeToVec<DynamicRange>(vector<std::shared_ptr<RangeBase> >& v,
						std::shared_ptr<DynamicRange> r)
	{
	    if(not r->isEmpty()){
		for(size_t i = r->dim(); i != 0; --i){
		    v.insert(v.begin(), r->sub(i-1));
		}
	    }
	}

	template <>
	inline size_t getStepSize<DynamicIndex>(const DynamicIndex& ii, std::intptr_t j)
	{
	    size_t ss = 0;
	    size_t sx = 1;
	    for(size_t k = ii.dim(); k != 0; --k){
		const size_t i = k-1;
		const auto& ni = ii.get(i);
		const size_t max = ni.max();
		const size_t tmp = ni.getStepSizeComp(j);
		ss += tmp * ii.getStepSize(i);
		sx *= max;
	    }
	    return ss;
	
	}
    }
}

