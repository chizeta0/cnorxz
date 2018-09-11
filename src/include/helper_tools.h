
#ifndef __helper_tools_h__
#define __helper_tools_h__

#include "base_def.h"
#include "slice.h"
#include <ostream>
#include "pack_num.h"
#include <functional>

namespace MultiArrayTools
{
    
    template <typename... T>
    std::ostream& operator<<(std::ostream& out, const std::tuple<T...>& tp);

    template <class RangeType>
    auto getIndex(std::shared_ptr<RangeType> range)
	-> std::shared_ptr<typename RangeType::IndexType>;

    // only if 'RangeType' is defaultable and unique (Singleton)
    template <class RangeType>
    auto getIndex() -> std::shared_ptr<typename RangeType::IndexType>;

    template <class... RangeTypes>
    auto mkMulti(std::shared_ptr<RangeTypes>... ranges)
	-> std::shared_ptr<MultiRange<RangeTypes...> >;

    template <class... IndexTypes>
    auto mkMIndex(std::shared_ptr<IndexTypes>... indices)
	-> decltype( getIndex( mkMulti( indices.range()... ) ) );

    template <class... RangeTypes>
    auto mkMulti(std::tuple<std::shared_ptr<RangeTypes>...> rangesTuple)
	-> MultiRange<RangeTypes...>;

    template <class RangeFactory>
    auto createExplicit(RangeFactory& rf)
	-> std::shared_ptr<typename RangeFactory::oType>;

    template <class RangeFactory>
    auto createExplicit(std::shared_ptr<RangeFactory> rfp)
	-> std::shared_ptr<typename RangeFactory::oType>;

    template <class Range>
    auto createRange(const std::vector<char>& cvec)
	-> std::shared_ptr<Range>;

    template <size_t N, class MArray>
    auto prtr(const MArray& ma)
	-> decltype(ma.template getRangePtr<N>());

    template <class IndexType>
    inline void For(const std::shared_ptr<IndexType>& ind, const std::function<void(void)>& ll);
}

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

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
    auto createRange(const std::vector<char>& cvec)
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

    template <class IndexType>
    inline void For(const std::shared_ptr<IndexType>& ind, const std::function<void(void)>& ll)
    {
	for((*ind) = 0; ind->pos() != ind->max(); ++(*ind)){
	    ll();
	}
    }

}

#endif
