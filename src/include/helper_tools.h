
#ifndef __helper_tools_h__
#define __helper_tools_h__

#include "base_def.h"
#include "slice.h"
#include <ostream>
#include "pack_num.h"
#include "map_range.h"
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

    template <SpaceType STYPE, class Op, class MA, class... RangeTypes>
    auto mkGenMapR(const std::tuple<Op,MA>& f, std::shared_ptr<RangeTypes>... ranges)
	-> std::shared_ptr<GenMapRange<Op,STYPE,RangeTypes...> >;

    template <SpaceType STYPE, class Op, class MA, class... IndexTypes>
    auto mkGenMapI(const std::tuple<Op,MA>& f, std::shared_ptr<IndexTypes>... indices)
	-> decltype( getIndex( mkGenMapR<STYPE>( f, indices->range()... ) ) );

    template <class Op, class MA, class... RangeTypes>
    auto mkMapR(const std::tuple<Op,MA>& f, std::shared_ptr<RangeTypes>... ranges)
	-> decltype( mkGenMapR<SpaceType::ANY>(f, ranges... ) );

    template <class Op, class MA, class... IndexTypes>
    auto mkMapI(const std::tuple<Op,MA>& f, std::shared_ptr<IndexTypes>... indices)
	-> decltype( mkGenMapI<SpaceType::ANY>(f, indices... ) );
    
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
    auto createRange(const vector<char>& cvec)
	-> std::shared_ptr<Range>;

    inline auto createRange(const vector<char>* cvec, int metaType, size_t size)
	-> std::shared_ptr<RangeBase>;

    inline auto createRangeA(const vector<char>* cvec, int metaType, size_t size)
	-> std::shared_ptr<AnonymousRange>;

    template <class Range, typename... Args>
    auto createRangeE(Args&&... args)
        -> std::shared_ptr<Range>;
    
    template <size_t N, class MArray>
    auto rptr(const MArray& ma)
	-> decltype(ma.template getRangePtr<N>());

    template <size_t I, class MIndex>
    auto get(const std::shared_ptr<MIndex>& i)
        -> decltype(i->template getPtr<I>())
    {
        return i->template getPtr<I>();
    }
    
    template <class EC, class MArray>
    auto dynamic(const MArray& ma, bool slice = false)
	-> std::shared_ptr<MultiArrayBase<typename MArray::value_type,DynamicRange<EC>>>;

    template <typename T, class EC, class Range1, class... RangeTypes>
    auto anonToDynView(const MultiArrayBase<T,Range1,RangeTypes...,AnonymousRange>& ma)
	-> ConstSlice<T,Range1,RangeTypes...,DynamicRange<EC>>;

    template <typename T, class EC, class Range1, class... RangeTypes>
    auto dynToAnonMove(MultiArray<T,Range1,RangeTypes...,DynamicRange<EC>>&& ma)
	-> MultiArray<T,Range1,RangeTypes...,AnonymousRange>;

    template <typename T, class EC>
    auto anonToDynView(const MultiArrayBase<T,AnonymousRange>& ma)
	-> ConstSlice<T,DynamicRange<EC>>;

    template <typename T, class EC>
    auto dynToAnonMove(MultiArray<T,DynamicRange<EC>>&& ma)
	-> MultiArray<T,AnonymousRange>;

    template <class Range>
    auto metaSlice(const std::shared_ptr<Range>& r)
        -> ConstSlice<typename Range::MetaType,ClassicRange>;

    template <class Range, class ORange>
    auto metaSlice(const std::shared_ptr<Range>& r, const std::shared_ptr<ORange>& ro)
        -> ConstSlice<typename Range::MetaType,ORange>;

    template <typename T, class... Ranges>
    auto mkArray(const std::shared_ptr<Ranges>&... rs)
        -> MultiArray<T,Ranges...>;

    template <typename T, class... Ranges>
    auto mkArray(const std::shared_ptr<Ranges>&... rs, const T& val)
        -> MultiArray<T,Ranges...>;

    template <class IndexType>
    inline void For(const std::shared_ptr<IndexType>& ind, const std::function<void(void)>& ll)
    {
	for((*ind) = 0; ind->pos() != ind->max(); ++(*ind)){
	    ll();
	}
    }

    // parallel:
    template <class IndexType>
    inline void PFor(const std::shared_ptr<IndexType>& ind,
                    const std::function<void(const std::shared_ptr<IndexType>&)>& ll)
    {
        const int max = static_cast<int>(ind->max());
        int i = 0;
#pragma omp parallel shared(ind,ll) private(i)
        {
#pragma omp for nowait
            for(i = 0; i < max; i++) {
                auto ii = getIndex( ind->range() );
                ((*ii) = i)();
                ll(ii);
            }
        }
    }


    template <class Index>
    inline auto mkOp(const std::shared_ptr<Index>& i)
	-> decltype(std::declval<FunctionalMultiArray<typename Index::MetaType,
		    identity<typename Index::MetaType>,typename Index::RangeType> >
		    ().exec(i))
    {
	FunctionalMultiArray<typename Index::MetaType,
			     identity<typename Index::MetaType>,
			     typename Index::RangeType> fma(i->range());
	return fma.exec(i);
    }

    template <typename R, typename... Ts>
    struct Func
    {
        static inline std::shared_ptr<function<R,Ts...>> mk(const std::function<R(Ts...)>& ll)
        {
            return std::make_shared<function<R,Ts...>>(ll);
        }
    };
}


#endif
