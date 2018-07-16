
#ifndef __rpack_num_h__
#define __rpack_num_h__

#include <memory>
#include <vector>
#include <cassert>
#include "ranges/rbase_def.h"
#include "ranges/index_base.h"


namespace MultiArrayHelper
{
    using namespace MultiArrayTools;

    
    template <class Index1>
    size_t mkTotalDim()
    {
	return Index1::totalDim();
    }
    
    template <class Index1, class Index2, class... Indices>
    size_t mkTotalDim()
    {
	return Index1::totalDim() * mkTotalDim<Index2,Indices...>();
    }


    template <class RangeType, class... Ranges>
    struct SubProp
    {
	static constexpr size_t ISSTATIC = RangeType::ISSTATIC & SubProp<Ranges...>::ISSTATIC;
	static constexpr size_t SIZE = RangeType::SIZE * SubProp<Ranges...>::SIZE;
    };

    template <>
    struct SubProp<void>
    {
	static constexpr size_t ISSTATIC = 1;
	static constexpr size_t SIZE = 1;
    };

    template <class Index>
    inline size_t getStepSize(const Index& ii, std::intptr_t j);

    
    template <class Range>
    inline void resolveSetRange(std::shared_ptr<Range> rp, std::vector<std::shared_ptr<RangeBase> > orig,
				size_t origpos, size_t size)
    {
	assert(size == 1);
	rp = std::dynamic_pointer_cast<Range>( orig[origpos] ); // catch bad cast here!!
    }

    template <class Range>
    inline void setRangeToVec(std::vector<std::shared_ptr<RangeBase> >& v,
			      std::shared_ptr<Range> r)
    {
	v.insert(v.begin(), r);
    }
    
    template <size_t N>
    struct RPackNum
    {
	template <class... Indices>
	static void initBlockSizes(std::array<size_t,sizeof...(Indices)+1>& bs,
				   std::tuple<std::shared_ptr<Indices>...>& ip)
	{
	    std::get<N>(bs) = RPackNum<sizeof...(Indices)-N>::blockSize(ip);
	    RPackNum<N-1>::initBlockSizes(bs, ip);
	}
	
	template <class... Indices>
	static inline void pp(std::tuple<std::shared_ptr<Indices>...>& ip)
	{
	    auto& si = *std::get<N>(ip);
	    if(si.last()){
		si = 0;
		RPackNum<N-1>::pp(ip);
	    }
	    else {
		++si;
	    }
	}
	
	template <class... Indices>
	static inline int pp(std::tuple<std::shared_ptr<Indices>...>& ip,
			     std::array<size_t,sizeof...(Indices)+1>& bs,
			     std::intptr_t idxPtrNum)
	{
	    auto& siPtr = std::get<N>(ip);
	    //VCHECK(siPtr.id());
	    if(reinterpret_cast<std::intptr_t>(siPtr.get()) == idxPtrNum){
		return RPackNum<N-1>::pp(ip, bs, idxPtrNum);
	    }
	    else {
		int tmp = siPtr->pp(idxPtrNum);
		if(siPtr->pos() == siPtr->max()){
		    (*siPtr) = 0;
		    return RPackNum<N-1>::pp(ip, bs, idxPtrNum) - siPtr->max() + 1;
		}
		else {
		    return tmp * std::get<N+1>(bs);
		}
	    }
	}

	template <class... Indices>
	static inline void mm(std::tuple<std::shared_ptr<Indices>...>& ip)
	{
	    auto& si = *std::get<N>(ip);
	    if(si.first()){
		si = si.max() - 1;
		RPackNum<N-1>::mm(ip);
	    }
	    else {
		--si;
	    }
	}
	
	// !!!!
	template <class... Indices>
	static inline int mm(std::tuple<std::shared_ptr<Indices>...>& ip,
			     std::array<size_t,sizeof...(Indices)+1>& bs,
			     std::intptr_t idxPtrNum)
	{
	    auto& siPtr = std::get<N>(ip);
	    if(reinterpret_cast<std::intptr_t>(siPtr.get()) == idxPtrNum){
		return std::get<N>(bs) + RPackNum<N-1>::mm(ip, bs, idxPtrNum);
	    }
	    else {
		if(siPtr->first()){
		    (*siPtr) = siPtr->max() - 1;
		    return RPackNum<N-1>::mm(ip, bs, idxPtrNum) - siPtr->max() + 1;
		}
		else {
		    return siPtr->mm(idxPtrNum);
		}
	    }
	}
	
	template <class RangeTuple>
	static size_t getSize(const RangeTuple& rt)
	{
	    return std::get<N>(rt)->size() * RPackNum<N-1>::getSize(rt);
	}
	
	template <class IndexPack, class MetaType>
	static void getMetaPos(MetaType& target,
			       const IndexPack& source)
	{
	    std::get<N>(target) = std::get<N>(source)->meta();
	    RPackNum<N-1>::getMetaPos(target, source);
	}
	
	template <class IndexPack, typename MetaType>
	static void setMeta(IndexPack& target, const MetaType& source)
	{
	    std::get<N>(target)->at( std::get<N>(source) );
	    RPackNum<N-1>::setMeta(target, source);
	}
	
	template <class IndexPack>
	static void setIndexPack(IndexPack& iPack, size_t pos)
	{
	    auto& i = *std::get<N>(iPack).get();
	    const size_t ownPos = pos % i.max(); 
	    i = ownPos;
	    RPackNum<N-1>::setIndexPack(iPack, (pos - ownPos) / i.max() );
	}
	
	template <class MRange, class... Indices>
	static void construct(std::tuple<std::shared_ptr<Indices>...>& ip,
			      const MRange& range)
	{
	    typedef typename std::remove_reference<decltype(range.template get<N>())>::type SubRangeType;
	    typedef typename SubRangeType::IndexType SubIndexType;
	    typedef typename std::remove_reference<decltype(*std::get<N>(ip).get())>::type TypeFromIndexPack;
	    
	    static_assert(std::is_same<SubIndexType,TypeFromIndexPack>::value,
			  "inconsiśtent types");
	    
	    std::get<N>(ip) = std::shared_ptr<SubIndexType>( new SubIndexType( range.template getPtr<N>() ) );
	    RPackNum<N-1>::construct(ip, range);
	}

	template <class IndexType, class... Indices>
	static void copyInst(std::tuple<std::shared_ptr<Indices>...>& ip,
			     const IndexType& ind)
	{
	    std::get<N>(ip) = ind.template getPtr<N>() ;
	    RPackNum<N-1>::copyInst(ip, ind);
	}
	
	template <class... Indices>
	static inline size_t makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup)
	{
	    //const auto& idx = *std::get<N>(iPtrTup);
	    return std::get<N>(iPtrTup)->pos() + RPackNum<N-1>::makePos(iPtrTup) * std::get<N>(iPtrTup)->max();
	}

	template <class... Indices>
	static inline size_t makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup,
				     const std::array<size_t,sizeof...(Indices)+1>& blockSize)
	{
	    return RPackNum<N-1>::makePos(iPtrTup, blockSize) + std::get<N>(iPtrTup)->pos() * std::get<N+1>(blockSize);
	}

	template <class Pack, class IndexType, class... Indices>
	static void swapIndices(Pack& ipack, const std::shared_ptr<IndexType>& nind,
				const std::shared_ptr<Indices>&... ninds)
	{
	    std::get<std::tuple_size<Pack>::value-N-1>(ipack) = nind;
	    RPackNum<N-1>::swapIndices(ipack, ninds...);
	}

	template <class... Indices>
	static size_t blockSize(const std::tuple<std::shared_ptr<Indices>...>& pack)
	{
	    return std::get<sizeof...(Indices)-N>(pack)->max() * RPackNum<N-1>::blockSize(pack);
	}


	template <class... Ranges>
	static inline void RangesToVec(const std::tuple<std::shared_ptr<Ranges>...>& rst,
				std::vector<std::shared_ptr<RangeBase> >& v)
	{
	    setRangeToVec(v, std::get<N>(rst));
	    //v[N] = std::get<N>(rst);
	    RPackNum<N-1>::RangesToVec(rst, v);
	}

	template <class... Indices>
	static void printIndex(const std::tuple<std::shared_ptr<Indices>...>& ip, size_t offset)
	{
	    std::get<N>(ip)->print(offset);
	    RPackNum<N-1>::printIndex(ip, offset);
	}

	template <class Range, class... Ranges>
	static void checkDefaultable()
	{
	    static_assert( Range::defaultable, "not defaultable" );
	    RPackNum<N-1>::template checkDefaultable<Ranges...>();
	}

	template <class IndexPack, class Exprs>
	static auto mkFor(const IndexPack& ipack, Exprs exs)
	    -> decltype(std::get<std::tuple_size<IndexPack>::value-N-1>(ipack)
			->ifor( RPackNum<N-1>::mkFor(ipack, exs) ) )
	{
	    return std::get<std::tuple_size<IndexPack>::value-N-1>(ipack)
		->ifor( RPackNum<N-1>::mkFor(ipack, exs) );
	}

	template <class IndexPack, class Exprs>
	static auto mkForh(const IndexPack& ipack, Exprs exs)
	    -> decltype(std::get<std::tuple_size<IndexPack>::value-N-1>(ipack)
			->iforh( RPackNum<N-1>::mkForh(ipack, exs) ) )
	{
	    return std::get<std::tuple_size<IndexPack>::value-N-1>(ipack)
		->iforh( RPackNum<N-1>::mkForh(ipack, exs) );
	}

	template <class Index>
	static inline void getStepSizeX(const Index& ii, std::intptr_t j, size_t& ss, size_t& sx)
	{
	    //constexpr size_t DIM = Index::sDim();
	    const auto& ni = ii.template get<N>();
	    const size_t max = ni.max();
	    const size_t tmp = getStepSize(ni, j);
	    //VCHECK(tmp);
	    ss += tmp * sx;
	    //VCHECK(ss);
	    sx *= max;
	    RPackNum<N-1>::getStepSizeX(ii, j, ss, sx);
	}

	template <class RangeTuple, typename... SIZET>
	static inline void resolveRangeType(const std::vector<std::shared_ptr<RangeBase> > orig,
					    const RangeTuple& rtp, size_t off, size_t size, SIZET... sizes)
	{
	    constexpr size_t tps = std::tuple_size<RangeTuple>::value;
	    resolveSetRange(std::get<tps-N-1>(rtp), orig, off, size);
	    RPackNum<N-1>::resolveRangeType(orig, rtp, off-size, sizes...);
	}
    };

    
    template<>
    struct RPackNum<0>
    {

	template <class... Indices>
	static void initBlockSizes(std::array<size_t,sizeof...(Indices)+1>& bs,
				   std::tuple<std::shared_ptr<Indices>...>& ip)
	{
	    std::get<0>(bs) = RPackNum<sizeof...(Indices)>::blockSize(ip);
	}

	template <class... Indices>
	static inline void pp(std::tuple<std::shared_ptr<Indices>...>& ip)
	{
	    auto& si = *std::get<0>(ip);
	    ++si;
	}
	
	template <class... Indices>
	static inline int pp(std::tuple<std::shared_ptr<Indices>...>& ip,
			     std::array<size_t,sizeof...(Indices)+1>& bs,
			     std::intptr_t idxPtrNum)
	{
	    auto& siPtr = std::get<0>(ip);
	    if(reinterpret_cast<std::intptr_t>(siPtr.get()) == idxPtrNum){
		return std::get<0>(bs);
	    }
	    else {
		int tmp = siPtr->pp(idxPtrNum);
		return tmp * std::get<1>(bs);
	    }
	}

	template <class... Indices>
	static inline void mm(std::tuple<std::shared_ptr<Indices>...>& ip)
	{
	    auto& si = *std::get<0>(ip);
	    --si;
	}
	
	template <class... Indices>
	static inline int mm(std::tuple<std::shared_ptr<Indices>...>& ip,
			     std::array<size_t,sizeof...(Indices)+1>& bs,
			     std::intptr_t idxPtrNum)
	{
	    auto& siPtr = std::get<0>(ip);
	    if(reinterpret_cast<std::intptr_t>(siPtr.get()) == idxPtrNum){
		return std::get<0>(bs);
		//return 1;
	    }
	    else {
		return siPtr->mm(idxPtrNum);
	    }
	}
	
	template <class RangeTuple>
	static size_t getSize(const RangeTuple& rt)
	{
	    return std::get<0>(rt)->size();
	}
	
	template <class IndexPack, class MetaType>
	static void getMetaPos(MetaType& target,
			       const IndexPack& source)
	{
	    std::get<0>(target) = std::get<0>(source)->meta();
	}
	
	template <class IndexPack, typename MetaType>
	static void setMeta(IndexPack& target, const MetaType& source)
	{
	    std::get<0>(target)->at( std::get<0>( source ) );
	}
	
	template <class IndexPack>
	static void setIndexPack(IndexPack& iPack, size_t pos)
	{
	    auto& i = *std::get<0>(iPack);
	    const size_t ownPos = pos % i.max(); 
	    i = ownPos;
	}
	
	template <class MRange, class... Indices>
	static void construct(std::tuple<std::shared_ptr<Indices>...>& ip,
			      const MRange& range)
	{
	    typedef typename std::remove_reference<decltype(range.template get<0>())>::type SubRangeType;
	    typedef typename SubRangeType::IndexType SubIndexType;
	    typedef typename std::remove_reference<decltype(*std::get<0>(ip).get())>::type TypeFromIndexPack;
	    
	    static_assert(std::is_same<SubIndexType,TypeFromIndexPack>::value,
			  "inconsiśtent types");
	    
	    std::get<0>(ip) = std::shared_ptr<SubIndexType>( new SubIndexType( range.template getPtr<0>() ) );
	}

	template <class IndexType, class... Indices>
	static void copyInst(std::tuple<std::shared_ptr<Indices>...>& ip,
			     const IndexType& ind)
	{
	    std::get<0>(ip) = ind.template getPtr<0>();
	}
	
	template <class... Indices>
	static inline size_t makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup)
	{
	    return std::get<0>(iPtrTup)->pos();
	}

	template <class... Indices>
	static inline size_t makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup,
				     const std::array<size_t,sizeof...(Indices)+1>& blockSize)
	{
	    return std::get<0>(iPtrTup)->pos() * std::get<1>(blockSize);
	}

	template <class Pack, class IndexType>
	static void swapIndices(Pack& ipack, const std::shared_ptr<IndexType>& nind)
	{
	    std::get<std::tuple_size<Pack>::value-1>(ipack) = nind;
	}

	template <class... Indices>
	static size_t blockSize(const std::tuple<std::shared_ptr<Indices>...>& pack)
	{
	    return 1;
	}

	template <class... Ranges>
	static inline void RangesToVec(const std::tuple<std::shared_ptr<Ranges>...>& rst,
				std::vector<std::shared_ptr<RangeBase> >& v)
	{
	    v[0] = std::get<0>(rst);
	}

	template <class... Indices>
	static void printIndex(const std::tuple<std::shared_ptr<Indices>...>& ip, size_t offset)
	{
	    std::get<0>(ip)->print(offset);
	}

	template <class Range>
	static void checkDefaultable()
	{
	    static_assert( Range::defaultable, "not defaultable" );
	}

	template <class IndexPack, class Exprs>
	static auto mkFor(const IndexPack& ipack, Exprs exs)
	    -> decltype(std::get<std::tuple_size<IndexPack>::value-1>(ipack)
			->ifor(exs) )
	{
	    return std::get<std::tuple_size<IndexPack>::value-1>(ipack)->ifor(exs);
	}

	template <class IndexPack, class Exprs>
	static auto mkForh(const IndexPack& ipack, Exprs exs)
	    -> decltype(std::get<std::tuple_size<IndexPack>::value-1>(ipack)
			->iforh(exs) )
	{
	    return std::get<std::tuple_size<IndexPack>::value-1>(ipack)->iforh(exs);
	}

	template <class Index>
	static inline void getStepSizeX(const Index& ii, std::intptr_t j, size_t& ss, size_t& sx)
	{
	    //constexpr size_t DIM = Index::sDim();
	    const auto& ni = ii.template get<0>();
	    const size_t max = ni.max();
	    const size_t tmp = getStepSize(ni, j);
	    //VCHECK(tmp);
	    ss += tmp * sx;
	    //VCHECK(ss);
	    sx *= max;
	}

	template <class RangeTuple, typename... SIZET>
	static inline void resolveRangeType(const std::vector<std::shared_ptr<RangeBase> > orig,
					    const RangeTuple& rtp, size_t off, size_t size)
	{
	    constexpr size_t tps = std::tuple_size<RangeTuple>::value;
	    resolveSetRange(std::get<tps-1>(rtp), orig, off, size);
	}

    };
    
    template <IndexType IT>
    struct SSG
    {
	template <class Index>
	static inline size_t getStepSize(const Index& ii, std::intptr_t j)
	{
	    size_t ss = 0;
	    size_t sx = 1;
	    constexpr size_t DIM = Index::sDim();
	    RPackNum<DIM-1>::getStepSizeX(ii, j, ss, sx);
	    return ss;
	}
    };

    template <>
    struct SSG<IndexType::SINGLE>
    {
	template <class Index>
	static inline size_t getStepSize(const Index& ii, std::intptr_t j)
	{
	    //VCHECK(ii.ptrNum());
	    //VCHECK(j);
	    return ii.ptrNum() == j ? 1 : 0;
	}
    };
    
    template <class Index>
    inline size_t getStepSize(const Index& ii, std::intptr_t j)
    {
	constexpr IndexType IT = Index::sType();
	return SSG<IT>::getStepSize(ii, j);
    }

} //  end namespace MultiArrayHelper

#endif
