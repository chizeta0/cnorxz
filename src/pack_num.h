// -*- C++ -*-

#ifndef __pack_num_h__
#define __pack_num_h__

#include <cstdlib>
#include <type_traits>
#include <tuple>
#include <memory>

#include "base_def.h"

namespace MultiArrayHelper
{
    using namespace MultiArrayTools;

    template <typename... T>
    std::ostream& operator<<(std::ostream& out, const std::tuple<T...>& tp);
    
    template <size_t N>
    struct PackNum
    {
	template <class IndexType>
	static IndexBase& getIndex(IndexType& in, size_t n)
	{
	    if(n == N){
		return in.template get<N>();
	    }
	    else {
		return PackNum<N-1>::getIndex(in, n);
	    }
	}
	
	template <class IndexType>
	static const IndexBase& getIndex(const IndexType& in, size_t n)
	{
	    if(n == N){
		return in.template get<N>();
	    }
	    else {
		return PackNum<N-1>::getIndex(in, n);
	    }
	}

	template <class IndexType>
	static std::shared_ptr<IndexBase> getIndexPtr(const IndexType& in, size_t n)
	{
	    if(n == N){
		return in.template getPtr<N>();
	    }
	    else {
		return PackNum<N-1>::getIndexPtr(in, n);
	    }
	}

	template <class... Indices>
	static void lock(std::tuple<std::shared_ptr<Indices>...>& ip,
			 std::shared_ptr<IndexBase> toLock)
	{
	    std::get<N>(ip)->lock(toLock);
	    PackNum<N-1>::lock(ip, toLock);
	}
	
	template <class... Indices>
	static void initBlockSizes(std::array<size_t,sizeof...(Indices)+1>& bs,
				   std::tuple<std::shared_ptr<Indices>...>& ip)
	{
	    std::get<N>(bs) = PackNum<sizeof...(Indices)-N-1>::blockSize(ip);
	    PackNum<N-1>::initBlockSizes(bs, ip);
	}
	
	template <class... Indices>
	static inline void pp(std::tuple<std::shared_ptr<Indices>...>& ip)
	{
	    auto& si = *std::get<N>(ip);
	    if(si.last()){
		si = 0;
		PackNum<N-1>::pp(ip);
	    }
	    else {
		++si;
	    }
	}
	
	template <class... Indices>
	static inline int pp(std::tuple<std::shared_ptr<Indices>...>& ip,
			     std::array<size_t,sizeof...(Indices)+1>& bs,
			     std::shared_ptr<IndexBase> idxPtr)
	{
	    auto& siPtr = std::get<N>(ip);
	    //VCHECK(siPtr.id());
	    if(siPtr.get() == idxPtr.get()){
		return PackNum<N-1>::pp(ip, bs, idxPtr);
	    }
	    else {
		int tmp = siPtr->pp(idxPtr);
		if(siPtr->pos() == siPtr->max()){
		    (*siPtr) = 0;
		    return PackNum<N-1>::pp(ip, bs, idxPtr) - siPtr->max() + 1;
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
		PackNum<N-1>::mm(ip);
	    }
	    else {
		--si;
	    }
	}
	
	template <class... Indices>
	static inline int mm(std::tuple<std::shared_ptr<Indices>...>& ip,
			     std::array<size_t,sizeof...(Indices)+1>& bs,
			     std::shared_ptr<IndexBase> idxPtr)
	{
	    auto& siPtr = std::get<N>(ip);
	    if(siPtr.get() == idxPtr.get()){
		return std::get<N>(bs) + PackNum<N-1>::mm(ip, bs, idxPtr);
	    }
	    else {
		if(siPtr->first()){
		    (*siPtr) = siPtr->max() - 1;
		    return PackNum<N-1>::mm(ip, bs, idxPtr) - siPtr->max() + 1;
		}
		else {
		    return siPtr->mm(idxPtr);
		}
	    }
	}
	
	template <class RangeTuple>
	static size_t getSize(const RangeTuple& rt)
	{
	    return std::get<N>(rt)->size() * PackNum<N-1>::getSize(rt);
	}
	
	template <class IndexPack, class MetaType>
	static void getMetaPos(MetaType& target,
			       const IndexPack& source)
	{
	    std::get<N>(target) = std::get<N>(source)->meta();
	    PackNum<N-1>::getMetaPos(target, source);
	}
	
	template <class IndexPack, typename MetaType>
	static void setMeta(IndexPack& target, const MetaType& source)
	{
	    std::get<N>(target)->at( std::get<N>(source) );
	    PackNum<N-1>::setMeta(target, source);
	}
	
	template <class IndexPack>
	static void setIndexPack(IndexPack& iPack, size_t pos)
	{
	    auto& i = *std::get<N>(iPack).get();
	    const size_t ownPos = pos % i.max(); 
	    i = ownPos;
	    PackNum<N-1>::setIndexPack(iPack, (pos - ownPos) / i.max() );
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
	    PackNum<N-1>::construct(ip, range);
	}
	/*
	template <template<class...> class IndexType, class... Indices>
	static void copy(std::tuple<std::shared_ptr<Indices>...>& ip,
			 const IndexType<Indices...>& ind)
	{
	    typedef typename std::remove_reference<decltype(ind.template get<N>())>::type SubIndexType;
	    std::get<N>(ip) = std::shared_ptr<SubIndexType>( new SubIndexType( ind.template get<N>() ) );
	    PackNum<N-1>::copy(ip, ind);
	}
	*/
	template <template<class...> class IndexType, class... Indices>
	static void copyInst(std::tuple<std::shared_ptr<Indices>...>& ip,
			     const IndexType<Indices...>& ind)
	{
	    std::get<N>(ip) = ind.template getPtr<N>() ;
	    PackNum<N-1>::copyInst(ip, ind);
	}
	
	template <class... Indices>
	static size_t makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup)
	{
	    auto ID = std::get<N>(iPtrTup)->id();
	    //VCHECK(std::get<N>(iPtrTup)->pos());
	    //VCHECK(std::get<N>(iPtrTup)->max());
	    return std::get<N>(iPtrTup)->pos() +
		PackNum<N-1>::makePos(iPtrTup) * std::get<N>(iPtrTup)->max();
	}

	template <class Pack, class IndexType, class... Indices>
	static void swapIndices(Pack& ipack, const std::shared_ptr<IndexType>& nind,
				const std::shared_ptr<Indices>&... ninds)
	{
	    std::get<std::tuple_size<Pack>::value-N-1>(ipack) = nind;
	    PackNum<N-1>::swapIndices(ipack, ninds...);
	}

	template <class... Indices>
	static size_t blockSize(const std::tuple<std::shared_ptr<Indices>...>& pack)
	{
	    return std::get<sizeof...(Indices)-N-1>(pack)->max() * PackNum<N-1>::blockSize(pack);
	}

	template <typename T, class Func, class ArgTuple, class... Args>
	static BlockResult<T> unpackArgs(const ArgTuple& tp, const Args&... args)
	{
	    return PackNum<N-1>::template unpackArgs<T,Func>(tp, std::get<N>(tp).get(), args...);
	}

	template <class... Ops>
	static void makeBlockTypeVec(std::vector<std::pair<BlockType,size_t> >& btv,
				     const std::tuple<Ops...>& ops,
				     std::shared_ptr<IndexBase> idxPtr)
	{
	    auto subvec = std::move( std::get<N>(ops).block(idxPtr) );
	    btv.insert(btv.end(), subvec.begin(), subvec.end() );
	    PackNum<N-1>::makeBlockTypeVec(btv, ops, idxPtr);
	}

	template <typename... T>
	static void printTuple(std::ostream& out, const std::tuple<T...>& tp){
	    out << std::get<sizeof...(T)-N-1>(tp) << ", ";
	    PackNum<N-1>::printTuple(out, tp);
	}

	template <class RangeType, class... Ranges>
	static void RangesToVec(const std::shared_ptr<RangeType> r,
				const std::shared_ptr<Ranges>... rs,
				std::vector<RangeBase> v)
	{
	    
	}
    };
    
    template<>
    struct PackNum<0>
    {
	template <class MultiIndex>
	static IndexBase& getIndex(MultiIndex& in, size_t n)
	{
	    return in.template get<0>();
	}
	
	template <class MultiIndex>
	static const IndexBase& getIndex(const MultiIndex& in, size_t n)
	{
	    return in.template get<0>();
	}

	template <class IndexType>
	static std::shared_ptr<IndexBase> getIndexPtr(const IndexType& in, size_t n)
	{
	    return in.template getPtr<0>();
	}

	template <class... Indices>
	static void lock(std::tuple<std::shared_ptr<Indices>...>& ip,
			 std::shared_ptr<IndexBase> toLock)
	{
	    std::get<0>(ip)->lock(toLock);
	}
	
	template <class... Indices>
	static void initBlockSizes(std::array<size_t,sizeof...(Indices)+1>& bs,
				   std::tuple<std::shared_ptr<Indices>...>& ip)
	{
	    std::get<0>(bs) = PackNum<sizeof...(Indices)-1>::blockSize(ip);
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
			     std::shared_ptr<IndexBase> idxPtr)
	{
	    auto& siPtr = std::get<0>(ip);
	    if(siPtr.get() == idxPtr.get()){
		return std::get<0>(bs);
	    }
	    else {
		int tmp = siPtr->pp(idxPtr);
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
			     std::shared_ptr<IndexBase> idxPtr)
	{
	    auto& siPtr = std::get<0>(ip);
	    if(siPtr.get() == idxPtr.get()){
		return std::get<0>(bs);
		//return 1;
	    }
	    else {
		return siPtr->mm(idxPtr);
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
	/*
	template <template<class...> class IndexType, class... Indices>
	static void copy(std::tuple<std::shared_ptr<Indices>...>& ip,
			 const IndexType<Indices...>& ind)
	{
	    typedef typename std::remove_reference<decltype(ind.template get<0>())>::type SubIndexType;
	    std::get<0>(ip) = std::shared_ptr<SubIndexType>( new SubIndexType( ind.template get<0>() ) );
	}
	*/
	template <template<class...> class IndexType, class... Indices>
	static void copyInst(std::tuple<std::shared_ptr<Indices>...>& ip,
			     const IndexType<Indices...>& ind)
	{
	    std::get<0>(ip) = ind.template getPtr<0>();
	}
	
	template <class... Indices>
	static size_t makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup)
	{
	    auto ID = std::get<0>(iPtrTup)->id();
	    //VCHECK(std::get<0>(iPtrTup)->pos());
	    //VCHECK(std::get<0>(iPtrTup)->max());
	    return std::get<0>(iPtrTup)->pos();
	}
	
	template <class Pack, class IndexType>
	static void swapIndices(Pack& ipack, const std::shared_ptr<IndexType>& nind)
	{
	    std::get<std::tuple_size<Pack>::value-1>(ipack) = nind;
	}

	template <class... Indices>
	static size_t blockSize(const std::tuple<std::shared_ptr<Indices>...>& pack)
	{
	    return std::get<sizeof...(Indices)-1>(pack)->max();
	}

	template <typename T, class Func, class ArgTuple, class... Args>
	static BlockResult<T> unpackArgs(const ArgTuple& tp, const Args&... args)
	{
	    static_assert(sizeof...(Args) == std::tuple_size<ArgTuple>::value-1,
			  "inconsistent number of arguments");
	    static BlockBinaryOp<T,Func,decltype(std::get<0>(tp).get()), decltype(args)...> f;
	    return f(std::get<0>(tp).get(), args...);
	}
	
	template <class... Ops>
	static void makeBlockTypeVec(std::vector<std::pair<BlockType,size_t> >& btv,
				     const std::tuple<Ops...>& ops,
				     std::shared_ptr<IndexBase> idxPtr)
	{
	    auto subvec = std::move( std::get<0>(ops).block(idxPtr) );
	    btv.insert(btv.end(), subvec.begin(), subvec.end() );
	}

	template <typename... T>
	static void printTuple(std::ostream& out, const std::tuple<T...>& tp){
	    out << std::get<sizeof...(T)-1>(tp);
	}

    };

    template <typename... T>
    std::ostream& operator<<(std::ostream& out, const std::tuple<T...>& tp)
    {
	PackNum<sizeof...(T)-1>::printTuple(out, tp);
	return out;
    }

    
} // end namespace MultiArrayHelper

#endif
