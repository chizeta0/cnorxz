// -*- C++ -*-

#include <cstdlib>
#include <type_traits>
#include <tuple>
#include <memory>

namespace MultiArrayHelper
{
    
    template <size_t N>
    struct PackNum
    {
	template <class IndexType>
	static IndexBase& getIndex(IndexType& in, size_t n)
	{
	    if(n == N){
		return in.getIndex<N>();
	    }
	    else {
		return PackNum<N-1>::getIndex(in, n);
	    }
	}
	
	template <class IndexType>
	static const IndexBase& getIndex(const IndexType& in, size_t n)
	{
	    if(n == N){
		return in.getIndex<N>();
	    }
	    else {
		return PackNum<N-1>::getIndex(in, n);
	    }
	}
	
	template <class... Indices>
	static inline void pp(std::tuple<std::shared_ptr<Indices>...>& ip)
	{
	    auto& si = *std::get<N>(ip);
	    if(si.pos() == si.last()){
		si = 0;
		PackNum<N-1>::pp(index);
	    }
	    else {
		++si;
	    }
	}
	
	template <class... Indices>
	static inline void mm(std::tuple<std::shared_ptr<Indices>...>& ip)
	{
	    auto& si = *std::get<N>(ip);
	    if(si.pos() == si.atEdge()){
		si = si.max();
		PackNum<N-1>::mm(index);
	    }
	    else {
		--si;
	    }
	}
	
	template <class RangeTuple>
	static size_t getSize(const RangeTuple& rt)
	{
	    return std::get<N>(rt).size() * PackNum<N-1>::getSize(rt);
	}
	
	template <template <class...> IndexType, class... Indices>
	static void getMetaPos(std::tuple<decltype(Indices().meta())...>& target,
			       const typename IndexType<Indices...>::IndexPack& source)
	{
	    std::get<N>(target) = std::get<N>(source)->meta();
	    PackNum<N-1>::getMetaPos(target, source);
	}
	
	template <class IndexPack>
	static void setMeta(IndexPack& target, const MetaType& source)
	{
	    std::get<N>(target).atMeta( std::get<N>(source) );
	    PackNum<N-1>::setMeta(target, source);
	}
	
	template <class IndexPack>
	static void setIndexPack(IndexPack& iPack, size_t pos)
	{
	    auto i = std::get<N>(iPack);
	    const size_t ownPos = pos % i.max(); 
	    i = ownPos;
	    if(ownPos == pos){
		PackNum<N-1>::setIndexPack(iPack, (pos - ownPos) / i.max() );
	    }
	}
	
	template <class MRange, class... Indices>
	static void construct(std::tuple<std::shared_ptr<Indices>...>& ip,
			      const MRange& range)
	{
	    typedef decltype(range.template get<N>()) SubIndexType;
	    typedef decltype(std::get<N>(ip).get()) TypeFromIndexPack;
	    
	    static_assert(std::is_same<SubIndexType,TypeFromIndexPack>::value,
			  "inconsiśtent types");
	    
	    std::get<N>(ip).swap( std::make_shared<SubIndexType>( range.template get<N>() ) );
	    PackNum<N-1>::construct(ip, range);
	}
	
	template <template<class...> IndexType, class... Indices>
	static void copy(std::tuple<std::shared_ptr<Indices>...>& ip,
			 const IndexType<Indices...>& ind)
	{
	    typedef decltype(ind.template get<N>()) SubIndexType;
	    std::get<N>(ip).swap( std::make_shared<SubIndexType>( ind.template get<N>() ) );
	    PackNum<N-1>::copy(ip, ind);
	}
	
	template <class... Indices>
	static size_t makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup)
	{
	    return std::get<N>(iPtrTup)->pos() +
		PackNum<N-1>::makePos(iPtrTup) * std::get<N-1>(iPtrTup)->max();
	}

	template <class Pack, class IndexType, class... Indices>
	static void swapIndices(Pack& ipack, const std::shared_ptr<Indices>&... ninds,
				const std::shared_ptr<IndexType>& nind)
	{
	    std::get<N>(ipack).swap( nind );
	    PackNum<N-1>::swapIndices(ipack, ninds...);
	}

	template <class... Indices>
	static size_t blockSize(const std::tuple<std::shared_ptr<Indices>...>& pack)
	{
	    return std::get<sizeof...(Indices)-N-1>(pack)->size() * PackNum<N-1>::blockSize(pack);
	}
	
    };
    
    template<>
    struct PackNum<0>
    {
	template <class MultiIndex>
	static IndefinitIndexBase& getIndex(MultiIndex& in, size_t n)
	{
	    return in.getIndex<0>();
	}
	
	template <class MultiIndex>
	static const IndefinitIndexBase& getIndex(const MultiIndex& in, size_t n)
	{
	    return in.getIndex<0>();
	}
	
	template <class... Indices>
	static inline void pp(std::tuple<std::shared_ptr<Indices>...>& ip)
	{
	    auto& si = *std::get<0>(ip);
	    ++si;
	}
	
	template <class... Indices>
	static inline void mm(std::tuple<std::shared_ptr<Indices>...>& ip)
	{
	    auto& si = *std::get<0>(ip);
	    --si;
	}
	
	template <class RangeTuple>
	static size_t getSize(const RangeTuple& rt)
	{
	    return std::get<0>(rt).size();
	}
	
	template <template <class...> IndexType, class... Indices>
	static void getMetaPos(std::tuple<decltype(Indices().meta())...>& target,
			       const typename IndexType<Indices...>::IndexPack& source)
	{
	    std::get<0>(target) = std::get<0>(source)->meta();
	}
	
	template <class IndexPack>
	static void setMeta(IndexPack& target, const MetaType& source)
	{
	    std::get<0>(target).atMeta( std::get<0>(source) );
	}
	
	template <class IndexPack>
	static void setIndexPack(IndexPack& iPack, size_t pos)
	{
	    auto i = std::get<0>(iPack);
	    const size_t ownPos = pos % i.max(); 
	    i = ownPos;
	}
	
	template <class MRange, class... Indices>
	static void construct(std::tuple<std::shared_ptr<Indices>...>& ip,
			      const MRange& range)
	{
	    typedef decltype(range.template get<0>()) SubIndexType;
	    typedef decltype(std::get<0>(ip).get()) TypeFromIndexPack;
	    
	    static_assert(std::is_same<SubIndexType,TypeFromIndexPack>::value,
			  "inconsiśtent types");
	    
	    std::get<0>(ip).swap( std::make_shared<SubIndexType>( range.template get<0>() ) );
	}
	
	template <template<class...> IndexType, class... Indices>
	static void copy(std::tuple<std::shared_ptr<Indices>...>& ip,
			 const IndexType<Indices...>& ind)
	{
	    typedef decltype(ind.template get<0>()) SubIndexType;
	    std::get<0>(ip).swap( std::make_shared<SubIndexType>( ind.template get<0>() ) );
	}
	
	template <class... Indices>
	static size_t makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup)
	{
	    return std::get<0>(iPtrTup)->pos();
	}
	
	template <class Pack, class IndexType>
	static void swapIndices(Pack& ipack, const std::shared_ptr<IndexType>& nind)
	{
	    std::get<0>(ipack).swap( nind );
	}

	template <class... Indices>
	static size_t blockSize(const std::tuple<std::shared_ptr<Indices>...>& pack)
	{
	    return std::get<sizeof...(Indices)-1>(pack)->size();
	}
	
    };
    
} // end namespace MultiArrayHelper
