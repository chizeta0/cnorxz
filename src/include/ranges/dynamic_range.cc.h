
#include "ranges/dynamic_range.h"
//#include "ranges/dynamic_meta.h"

#include "rpack_num.h"

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
    void DynamicRangeFactory::appendx(std::shared_ptr<Range> r)
    {
	if(mProductCreated){
	    
	    mProd = std::shared_ptr<oType>( new DynamicRange( *std::dynamic_pointer_cast<oType>(mProd) ) );
	    mProductCreated = false;
	}
	std::dynamic_pointer_cast<oType>(mProd)->mOrig.push_back(r);
	std::dynamic_pointer_cast<oType>(mProd)->mSize *= r->size();
	std::dynamic_pointer_cast<oType>(mProd)->mEmpty = false;
	//std::dynamic_pointer_cast<oType>(mProd)->mProtoI.push_back(mkIndexWrapper(r->begin()));
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
	std::dynamic_pointer_cast<oType>(mProd)->mProtoI.push_back(mkIndexWrapper(r->begin()));
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
	if(mIVec.size() == 1){
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
	if(mIVec.size() == 1){
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

    template <size_t N>
    struct InitProto
    {
	template <class... RangeTypes>
	static void mk(vector<std::shared_ptr<IndexW>>& proto,
		       const std::tuple<std::shared_ptr<RangeTypes>...>& orig)
	{
	    proto[N] = mkIndexWrapper(std::get<N>(orig)->begin());
	    InitProto<N-1>::mk(proto, orig);
	}

	template <class RangeType, class... RangeTypes>
	static void mk(vector<std::shared_ptr<IndexW>>& proto, size_t n,
		       std::shared_ptr<RangeType> o, std::shared_ptr<RangeTypes>... origs)
	{
	    proto[n-1-N] = mkIndexWrapper(o->begin());
	    InitProto<N-1>::mk(proto, n, origs...);
	}
    };

    template <>
    struct InitProto<0>
    {
	template <class... RangeTypes>
	static void mk(vector<std::shared_ptr<IndexW>>& proto,
		       const std::tuple<std::shared_ptr<RangeTypes>...>& orig)
	{
	    proto[0] = mkIndexWrapper(std::get<0>(orig)->begin());
	}

	template <class RangeType, class... RangeTypes>
	static void mk(vector<std::shared_ptr<IndexW>>& proto, size_t n,
		       std::shared_ptr<RangeType> o, std::shared_ptr<RangeTypes>... origs)
	{
	    proto[n-1] = mkIndexWrapper(o->begin());
}
    };

    template <class... RangeTypes>
    DynamicRange::DynamicRange(const std::tuple<std::shared_ptr<RangeTypes>...>& origs) :
	RangeInterface<DynamicIndex>()
    {
	RPackNum<sizeof...(RangeTypes)-1>::RangesToVec( origs, mOrig );
	mProtoI.resize(sizeof...(RangeTypes));
	InitProto<sizeof...(RangeTypes)-1>::mk(mProtoI, origs);
	mSize = RPackNum<sizeof...(RangeTypes)-1>::getSize( origs );
	if(sizeof...(RangeTypes)){
	    mEmpty = false;
	}
    }

    
    template <class... RangeTypes>
    DynamicRange::DynamicRange(std::shared_ptr<RangeTypes>... origs) :
	RangeInterface<DynamicIndex>()
    {
	auto rst = std::make_tuple(origs...);
	RPackNum<sizeof...(RangeTypes)-1>::RangesToVec( rst, mOrig );
	mProtoI.resize(sizeof...(RangeTypes));
	InitProto<sizeof...(RangeTypes)-1>::mk(mProtoI, sizeof...(RangeTypes), origs...);
	mSize = RPackNum<sizeof...(RangeTypes)-1>::getSize( rst );
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
	RPackNum<sizeof...(Ranges)-1>::resolveRangeType(mOrig, rtp, 0, sizes...);
	MultiRangeFactory<Ranges...> mrf(rtp);
	return std::dynamic_pointer_cast<MultiRange<Ranges...> >( mrf.create() );
    }

    
} // end namespace MultiArrayTools
