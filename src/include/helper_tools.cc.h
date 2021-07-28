
#include "helper_tools.h"

namespace CNORXZ
{
    template <typename... T>
    std::ostream& operator<<(std::ostream& out, const std::tuple<T...>& tp)
    {
	sfor_pn<0,sizeof...(T)-1>( [&](auto i){ out << std::get<i>(tp) << ", "; return 0; } );
	out << std::get<sizeof...(T)-1>(tp);
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
	static_assert( RangeType::defaultable, "Range not defaultable" );
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

    namespace
    {
	template <size_t N>
	struct IndexToRangeTuple
	{
	    template <class... IndexTypes>
	    static inline void set(std::tuple<std::shared_ptr<typename IndexTypes::RangeType>...>& out,
		     const std::tuple<std::shared_ptr<IndexTypes>...>& indices)
	    {
		std::get<N>(out) = std::get<N>(indices)->range();
		IndexToRangeTuple<N-1>::set(out,indices);
	    }
	};

	template <>
	struct IndexToRangeTuple<0>
	{
	    template <class... IndexTypes>
	    static inline void set(std::tuple<std::shared_ptr<typename IndexTypes::RangeType>...>& out,
		     const std::tuple<std::shared_ptr<IndexTypes>...>& indices)
	    {
		std::get<0>(out) = std::get<0>(indices)->range();
	    }
	};
    }
    
    template <class... IndexTypes>
    auto indexToRangeTuple(const std::tuple<std::shared_ptr<IndexTypes>...>& indices)
	-> std::tuple<std::shared_ptr<typename IndexTypes::RangeType>...>
    {
	std::tuple<std::shared_ptr<typename IndexTypes::RangeType>...> out;
	IndexToRangeTuple<sizeof...(IndexTypes)-1>::set(out, indices);
	return out;
    }

    template <class... IndexTypes>
    auto mkMIndex(std::shared_ptr<IndexTypes>... indices)
	-> decltype( getIndex( mkMulti( indices->range()... ) ) )
    {
	auto mi = getIndex( mkMulti( indices->range()... ) );	    
	(*mi)( indices... );
	return mi;
    }

    template <class... IndexTypes>
    auto mkMIndex(const std::tuple<std::shared_ptr<IndexTypes>...>& indices)
	-> decltype( getIndex( mkMulti( indexToRangeTuple(indices) ) ) )
    {
	auto mi = getIndex( mkMulti( indexToRangeTuple(indices) ) );	    
	(*mi)( indices );
	return mi;
    }


    template <class Index>
    auto mkIndexW(const std::shared_ptr<Index>& ind)
	-> std::shared_ptr<IndexW>
    {
	return std::make_shared<IndexWrapper<Index>>(ind);
    }

    template <SpaceType STYPE, class Op, class MA, class... RangeTypes>
    auto mkGenMapR(const std::tuple<Op,MA>& f, std::shared_ptr<RangeTypes>... ranges)
	-> std::shared_ptr<GenMapRange<MapORType<Op,STYPE>,Op,STYPE,RangeTypes...> >
    {
        GenMapRangeFactory<MapORType<Op,STYPE>,Op,STYPE,RangeTypes...> mrf(f, ranges... );
	return createExplicit( mrf );
    }

    template <SpaceType STYPE, class ORType, class Op, class MA, class... RangeTypes>
    auto mkGenMapRwith(const std::shared_ptr<ORType>& outr, const std::tuple<Op,MA>& f,
                       std::shared_ptr<RangeTypes>... ranges)
	-> std::shared_ptr<GenMapRange<ORType,Op,STYPE,RangeTypes...> >
    {
        GenMapRangeFactory<ORType,Op,STYPE,RangeTypes...> mrf(outr, f, ranges... );
	return createExplicit( mrf );
    }

    template <SpaceType STYPE, class Op, class MA, class... IndexTypes>
    auto mkGenMapI(const std::tuple<Op,MA>& f, std::shared_ptr<IndexTypes>... indices)
	-> decltype( getIndex( mkGenMapR<STYPE>( f, indices->range()... ) ) )
    {
	auto mi = getIndex( mkGenMapR<STYPE>( f, indices->range()... ) );
	(*mi)(indices...);
	return mi;
    }
    
    template <class Op, class MA, class... RangeTypes>
    auto mkMapR(const std::tuple<Op,MA>& f, std::shared_ptr<RangeTypes>... ranges)
	-> decltype( mkGenMapR<SpaceType::ANY>(f, ranges... ) )
    {
        return mkGenMapR<SpaceType::ANY>(f, ranges... );
    }

    template <class ORType, class Op, class MA, class... RangeTypes>
    auto mkMapRwith(const std::shared_ptr<ORType>& outr, const std::tuple<Op,MA>& f,
                    std::shared_ptr<RangeTypes>... ranges)
	-> decltype( mkGenMapRwith<SpaceType::ANY>(outr, f, ranges... ) )
    {
        return mkGenMapRwith<SpaceType::ANY>(outr, f, ranges... );
    }

    template <class Func, class... Indices>
    auto mkMapR(const std::shared_ptr<Func>& func, const std::shared_ptr<Indices>&... is)
	-> decltype( mkMapR( mkMapOp( func, is... ), is->range()... ) )
    {
	return mkMapR( mkMapOp( func, is... ), is->range()... );
    }

    template <class ORType, class Func, class... Indices>
    auto mkMapRwith(const std::shared_ptr<ORType>& outr, const std::shared_ptr<Func>& func, const std::shared_ptr<Indices>&... is)
	-> decltype( mkMapRwith(outr, mkMapOp( func, is... ), is->range()... ) )
    {
	return mkMapRwith(outr, mkMapOp( func, is... ), is->range()... );
    }

    template <class Op, class MA, class... IndexTypes>
    auto mkMapI(const std::tuple<Op,MA>& f, std::shared_ptr<IndexTypes>... indices)
	-> decltype( mkGenMapI<SpaceType::ANY>(f, indices... ) )
    {
        return mkGenMapI<SpaceType::ANY>(f, indices... );
    }

    template <class... RangeTypes>
    auto mkMulti(std::tuple<std::shared_ptr<RangeTypes>...> rangesTuple)
	-> std::shared_ptr<MultiRange<RangeTypes...>>
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

    inline auto createRange(const vector<char>* cvec, int metaType, size_t size)
        -> std::shared_ptr<RangeBase>
    {
        auto f = createSingleRangeFactory(cvec, metaType, size);
        return f->create();
    }

    inline auto createRangeA(const vector<char>* cvec, int metaType, size_t size)
	-> std::shared_ptr<AnonymousRange>
    {
        AnonymousRangeFactory arf(createRange(cvec, metaType, size));
        return createExplicit(arf);
    }

    inline auto cvecMetaCast(const std::shared_ptr<SingleRange<vector<char>,SpaceType::ANY>>& r, int metaType)
        -> std::shared_ptr<RangeBase>
    {
        return createRange(&r->get(0), metaType, r->size());
    }

    inline auto cvecMetaCastA(const std::shared_ptr<SingleRange<vector<char>,SpaceType::ANY>>& r, int metaType)
        -> std::shared_ptr<AnonymousRange>
    {
        return createRangeA(&r->get(0), metaType, r->size());
    }

    template <class Range, typename... Args>
    auto createRangeE(Args&&... args)
        -> std::shared_ptr<Range>
    {
        typename Range::FType f(args...);
        return createExplicit(f);
    }

    template <size_t N, class MArray>
    auto rptr(const MArray& ma)
	-> decltype(ma.template getRangePtr<N>())
    {
	return ma.template getRangePtr<N>();
    }


    template <class MArray>
    auto dynamic(const MArray& ma, bool slice)
	-> std::shared_ptr<MultiArrayBase<typename MArray::value_type,DynamicRange>>
    {
	DynamicRangeFactory drf(ma.range()->space());
	if(slice){
	    return std::make_shared<ConstSlice<typename MArray::value_type,DynamicRange>>
		( std::dynamic_pointer_cast<DynamicRange>( drf.create() ),
		  ma.data() );
	}
	else {
	    return std::make_shared<MultiArray<typename MArray::value_type,DynamicRange>>
		( std::dynamic_pointer_cast<DynamicRange>( drf.create() ),
		  ma.vdata() );
	}
    }

    template <class MArray>
    auto mdynamic(MArray& ma, bool slice)
	-> std::shared_ptr<MutableMultiArrayBase<typename MArray::value_type,DynamicRange>>
    {
	DynamicRangeFactory drf(ma.range()->space());
	if(slice){
	    return std::make_shared<Slice<typename MArray::value_type,DynamicRange>>
		( std::dynamic_pointer_cast<DynamicRange>( drf.create() ),
		  ma.data() );
	}
	else {
	    return std::make_shared<MultiArray<typename MArray::value_type,DynamicRange>>
		( std::dynamic_pointer_cast<DynamicRange>( drf.create() ),
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
    
    template <typename T, class Range1, class... RangeTypes>
    auto anonToDynView(const MultiArrayBase<T,Range1,RangeTypes...,AnonymousRange>& ma)
	-> ConstSlice<T,Range1,RangeTypes...,DynamicRange>
    {
	constexpr size_t LAST = sizeof...(RangeTypes)+1;
	DynamicRangeFactory drf(rptr<LAST>(ma)->orig());
	std::tuple<std::shared_ptr<Range1>,std::shared_ptr<RangeTypes>...,
		   std::shared_ptr<DynamicRange>> mNSpace;
	CopyRanges<LAST-1>::exec(ma.range()->space(),mNSpace);
	std::get<LAST>(mNSpace) = createExplicit( drf );
	return ConstSlice<T,Range1,RangeTypes...,DynamicRange>(mNSpace, ma.data());
    }

    template <typename T, class Range1, class... RangeTypes>
    auto anonToDynView(MutableMultiArrayBase<T,Range1,RangeTypes...,AnonymousRange>& ma)
	-> Slice<T,Range1,RangeTypes...,DynamicRange>
    {
	constexpr size_t LAST = sizeof...(RangeTypes)+1;
	DynamicRangeFactory drf(rptr<LAST>(ma)->orig());
	std::tuple<std::shared_ptr<Range1>,std::shared_ptr<RangeTypes>...,
		   std::shared_ptr<DynamicRange>> mNSpace;
	CopyRanges<LAST-1>::exec(ma.range()->space(),mNSpace);
	std::get<LAST>(mNSpace) = createExplicit( drf );
	return Slice<T,Range1,RangeTypes...,DynamicRange>(mNSpace, ma.data());
    }

    template <typename T, class Range1, class... RangeTypes>
    auto dynToAnonMove(MultiArray<T,Range1,RangeTypes...,DynamicRange>&& ma)
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

    template <typename T>
    auto anonToDynView(const MultiArrayBase<T,AnonymousRange>& ma)
	-> ConstSlice<T,DynamicRange>
    {
	DynamicRangeFactory drf(rptr<0>(ma)->orig());
	auto mNSpace = std::make_tuple( createExplicit( drf ) );
	return ConstSlice<T,DynamicRange>(mNSpace, ma.data());
    }

    template <typename T>
    auto anonToDynView(MutableMultiArrayBase<T,AnonymousRange>& ma)
	-> Slice<T,DynamicRange>
    {
	DynamicRangeFactory drf(rptr<0>(ma)->orig());
	auto mNSpace = std::make_tuple( createExplicit( drf ) );
	return Slice<T,DynamicRange>(mNSpace, ma.data());
    }

    template <typename T>
    auto dynToAnonMove(MultiArray<T,DynamicRange>&& ma)
	-> MultiArray<T,AnonymousRange>
    {
	AnonymousRangeFactory arf(rptr<0>(ma)->orig());
	auto mNSpace = std::make_tuple( createExplicit( arf ) );
	return ma.format(mNSpace);
    }

    template <class Range>
    auto metaSlice(const std::shared_ptr<Range>& r)
        -> ConstSlice<typename Range::MetaType,ClassicRange>
    {
        ClassicRF crf(r->size());
        return ConstSlice<typename Range::MetaType,ClassicRange>( createExplicit(crf), &r->get(0) );
    }

    template <class Range, class ORange>
    auto metaSlice(const std::shared_ptr<Range>& r, const std::shared_ptr<ORange>& ro)
        -> ConstSlice<typename Range::MetaType,ORange>
    {
        return ConstSlice<typename Range::MetaType,ORange>( ro, &r->get(0) );
    }

    template <typename T, class... Ranges>
    auto mkArray(const std::shared_ptr<Ranges>&... rs)
        -> MultiArray<T,Ranges...>
    {
        return MultiArray<T,Ranges...>(rs...);
    }

    template <typename T, class... Ranges>
    auto mkArray(const std::shared_ptr<Ranges>&... rs, const T& val)
        -> MultiArray<T,Ranges...>
    {
        return MultiArray<T,Ranges...>(rs..., val);
    }

    template <typename T, class... Ranges>
    auto mkArrayPtr(const std::shared_ptr<Ranges>&... rs)
        -> std::shared_ptr<MultiArray<T,Ranges...>>
    {
	return std::make_shared<MultiArray<T,Ranges...>>(rs...);
    }

}
