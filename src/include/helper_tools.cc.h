
#include "helper_tools.h"

namespace MultiArrayTools
{
    template <typename... T>
    std::ostream& operator<<(std::ostream& out, const std::tuple<T...>& tp)
    {
	PackNum<sizeof...(T)-1>::printTuple(out, tp);
	return out;
    }
    
    template <class RangeType>
    auto getIndex(std::shared_ptr<RangeType> range)
	-> std::shared_ptr<typename RangeType::IndexType>
    {
	return std::make_shared<typename RangeType::IndexType>(range);
    }

    template <class RangeType>
    auto getIndex()
	-> std::shared_ptr<typename RangeType::IndexType>
    {
	static_assert( RangeType::defaultable,
		       /*typeid(typename RangeType).name() + */" is not defaultable" );
	static auto f = RangeType::factory();
	static auto r = std::dynamic_pointer_cast<RangeType>( f.create() );
	return std::make_shared<typename RangeType::IndexType>(r);
    }
    
    template <class... RangeTypes>
    auto mkMulti(std::shared_ptr<RangeTypes>... ranges)
	-> std::shared_ptr<MultiRange<RangeTypes...> >
    {
	MultiRangeFactory<RangeTypes...> mrf( ranges... );
	return std::dynamic_pointer_cast<MultiRange<RangeTypes...> >( mrf.create() );
    }

    template <class... IndexTypes>
    auto mkMIndex(std::shared_ptr<IndexTypes>... indices)
	-> decltype( getIndex( mkMulti( indices->range()... ) ) )
    {
	auto mi = getIndex( mkMulti( indices->range()... ) );	    
	(*mi)( indices... );
	return mi;
    }

    template <SpaceType STYPE, class Func, class... RangeTypes>
    auto mkGenMapR(const std::shared_ptr<Func>& f, std::shared_ptr<RangeTypes>... ranges)
	-> std::shared_ptr<GenMapRange<FunctionalMultiArray<typename Func::value_type,Func,RangeTypes...>,
                                       STYPE,RangeTypes...> >
    {
	typedef FunctionalMultiArray<typename Func::value_type,Func,RangeTypes...> FMA;
	std::shared_ptr<GenMapRangeFactory<FMA,STYPE,RangeTypes...>> mrfptr;
	if(Func::FISSTATIC){
	    FMA fma(ranges...);
	    mrfptr = std::make_shared<GenMapRangeFactory<FMA,STYPE,RangeTypes...> >( fma, ranges... );
	}
	else {
	    FMA fma(ranges...,f);
	    mrfptr = std::make_shared<GenMapRangeFactory<FMA,STYPE,RangeTypes...> >( fma, ranges... );
	}
	return std::dynamic_pointer_cast<GenMapRange<FMA,STYPE,RangeTypes...> >( mrfptr->create() );
    }

    template <SpaceType STYPE, class Func, class... IndexTypes>
    auto mkGenMapI(const std::shared_ptr<Func>& f, std::shared_ptr<IndexTypes>... indices)
	-> decltype( getIndex( mkGenMapR<STYPE>( f, indices->range()... ) ) )
    {
	auto mi = getIndex( mkGenMapR<STYPE>( f, indices->range()... ) );
	(*mi)(indices...);
	return mi;
    }
    
    template <class Func, class... RangeTypes>
    auto mkMapR(const std::shared_ptr<Func>& f, std::shared_ptr<RangeTypes>... ranges)
	-> decltype( mkGenMapR<SpaceType::ANY>(f, ranges... ) )
    {
        return mkGenMapR<SpaceType::ANY>(f, ranges... );
    }

    template <class Func, class... IndexTypes>
    auto mkMapI(const std::shared_ptr<Func>& f, std::shared_ptr<IndexTypes>... indices)
	-> decltype( mkGenMapI<SpaceType::ANY>(f, indices... ) )
    {
        return mkGenMapI<SpaceType::ANY>(f, indices... );
    }

    template <class... RangeTypes>
    auto mkMulti(std::tuple<std::shared_ptr<RangeTypes>...> rangesTuple)
	-> MultiRange<RangeTypes...>
    {
	MultiRangeFactory<RangeTypes...> mrf( rangesTuple );
	return std::dynamic_pointer_cast<MultiRange<RangeTypes...> >( mrf.create() );
    }

    template <class RangeFactory>
    auto createExplicit(RangeFactory& rf)
	-> std::shared_ptr<typename RangeFactory::oType>
    {
	return std::dynamic_pointer_cast<typename RangeFactory::oType>( rf.create() );
    }

    template <class RangeFactory>
    auto createExplicit(std::shared_ptr<RangeFactory> rfp)
	-> std::shared_ptr<typename RangeFactory::oType>
    {
	return std::dynamic_pointer_cast<typename RangeFactory::oType>( rfp->create() );
    }

    template <class Range>
    auto createRange(const vector<char>& cvec)
	-> std::shared_ptr<Range>
    {
	const char* dp = cvec.data();
	auto ff = createRangeFactory(&dp);
	auto rbptr = ff->create();
	assert(rbptr->spaceType() == Range::STYPE);
	// CATCH CAST ERROR HERE !!!
	return std::dynamic_pointer_cast<Range>( rbptr );
    }

    template <size_t N, class MArray>
    auto rptr(const MArray& ma)
	-> decltype(ma.template getRangePtr<N>())
    {
	return ma.template getRangePtr<N>();
    }


    template <class EC, class MArray>
    auto dynamic(const MArray& ma, bool slice)
	-> std::shared_ptr<MultiArrayBase<typename MArray::value_type,DynamicRange<EC>>>
    {
	DynamicRangeFactory<EC> drf(ma.range()->space());
	if(slice){
	    return std::make_shared<ConstSlice<typename MArray::value_type,DynamicRange<EC>>>
		( std::dynamic_pointer_cast<DynamicRange<EC>>( drf.create() ),
		  ma.data() );
	}
	else {
	    return std::make_shared<MultiArray<typename MArray::value_type,DynamicRange<EC>>>
		( std::dynamic_pointer_cast<DynamicRange<EC>>( drf.create() ),
		  ma.vdata() );
	}
    }

    namespace
    {
	template <size_t N>
	struct CopyRanges
	{
	    template <class Space1, class Space2>
	    static inline void exec(const Space1& space1, Space2& space2)
	    {
		std::get<N>(space2) = std::get<N>(space1);
		CopyRanges<N-1>::exec(space1,space2);
	    }
	};

	template <>
	struct CopyRanges<0>
	{
	    template <class Space1, class Space2>
	    static inline void exec(const Space1& space1, Space2& space2)
	    {
		std::get<0>(space2) = std::get<0>(space1);
	    }
	};
    }
    
    template <typename T, class EC, class Range1, class... RangeTypes>
    auto anonToDynView(const MultiArray<T,Range1,RangeTypes...,AnonymousRange>& ma)
	-> ConstSlice<T,Range1,RangeTypes...,DynamicRange<EC>>
    {
	constexpr size_t LAST = sizeof...(RangeTypes)+1;
	DynamicRangeFactory<EC> drf(rptr<LAST>(ma)->orig());
	std::tuple<std::shared_ptr<Range1>,std::shared_ptr<RangeTypes>...,
		   std::shared_ptr<DynamicRange<EC>>> mNSpace;
	CopyRanges<LAST-1>::exec(ma.range()->space(),mNSpace);
	std::get<LAST>(mNSpace) = createExplicit( drf );
	return ConstSlice<T,Range1,RangeTypes...,DynamicRange<EC>>(mNSpace, ma.data());
    }

    template <typename T, class EC, class Range1, class... RangeTypes>
    auto dynToAnonMove(MultiArray<T,Range1,RangeTypes...,DynamicRange<EC>>&& ma)
	-> MultiArray<T,Range1,RangeTypes...,AnonymousRange>
    {
	constexpr size_t LAST = sizeof...(RangeTypes)+1;
	AnonymousRangeFactory arf(rptr<LAST>(ma)->orig());
	std::tuple<std::shared_ptr<Range1>,std::shared_ptr<RangeTypes>...,
		   std::shared_ptr<AnonymousRange>> mNSpace;
	CopyRanges<LAST-1>::exec(ma.range()->space(),mNSpace);
	std::get<LAST>(mNSpace) = createExplicit( arf );
	return ma.format(mNSpace);
    }

    template <typename T, class EC>
    auto anonToDynView(const MultiArray<T,AnonymousRange>& ma)
	-> ConstSlice<T,DynamicRange<EC>>
    {
	DynamicRangeFactory<EC> drf(rptr<0>(ma)->orig());
	auto mNSpace = std::make_tuple( createExplicit( drf ) );
	return ConstSlice<T,DynamicRange<EC>>(mNSpace, ma.data());
    }

    template <typename T, class EC>
    auto dynToAnonMove(MultiArray<T,DynamicRange<EC>>&& ma)
	-> MultiArray<T,AnonymousRange>
    {
	AnonymousRangeFactory arf(rptr<0>(ma)->orig());
	auto mNSpace = std::make_tuple( createExplicit( arf ) );
	return ma.format(mNSpace);
    }

    template <class Range>
    auto metaSlice(const std::shared_ptr<Range>& r)
        -> ConstSlice<typename Range::value_type,ClassicRange>
    {
        ClassicRF crf(r->size());
        return ConstSlice<typename Range::value_type,ClassicRange>( createExplicit(crf), &r->get(0) );
    }

    template <class Range, class ORange>
    auto metaSlice(const std::shared_ptr<Range>& r, const std::shared_ptr<ORange>& ro)
        -> ConstSlice<typename Range::value_type,ORange>
    {
        return ConstSlice<typename Range::value_type,ORange>( ro, &r->get(0) );
    }


}
