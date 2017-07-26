
#include "multi_range.h"

namespace MultiArrayTools
{
    /*******************
     *  MultiIndexBase *	     
     *******************/

    namespace
    {

	template <size_t N>
	struct PackNum
	{
	    template <class MultiIndex>
	    static IndexBase& getIndex(MultiIndex& in, size_t n)
	    {
		if(n == N){
		    return in.getIndex<N>();
		}
		else {
		    return PackNum<N-1>::getIndex(in, n);
		}
	    }

	    template <class MultiIndex>
	    static const IndexBase& getIndex(const MultiIndex& in, size_t n)
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
	    
	    template <class... Ranges>
	    static void setBegin(MultiRange<Ranges...>::IndexType& i, const MultiRange<Ranges...>::SpaceType& r)
	    {
		*std::get<N>(i) = std::get<N>(r)->begin();
		PackNum<N-1>::setBegin(i,r);
	    }
	    
	    template <class... Ranges>
	    static void setEnd(MultiRange<Ranges...>::IndexType& i, const MultiRange<Ranges...>::SpaceType& r)
	    {
		*std::get<N>(i) = std::get<N>(r)->end();
		*std::get<N>(i) -= 1;
		PackNum<N-1>::setEnd(i,r);
	    }

	    template <class... Ranges>
	    static void buildRangeVec(std::vector<MultiRangeType>& rvec, const MultiRange<Ranges...>::SpaceType& rs)
	    {
		rvec.push_back(std::get<sizeof...(Ranges)-N-1>(rs)->type());
		PackNum<N-1>::buildRangeVec(rvec, rs);
	    }

	    template <class... Indices>
	    static void getMetaPos(std::tuple<decltype(Indices().meta())...>& target,
				   const typename MultiIndex<Indices...>::IndexPack& source)
	    {
		std::get<N>(target) = std::get<N>(source)->meta();
		PackNum<N-1>::getMetaPos(target, source);
	    }

	    template <class... Indices>
	    static void setMeta(typename MultiIndex<Indices...>::IndexPack& target,
				const typename MultiIndex<Indices...>::MetaType& source)
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

	    template <class IndexPack, class... Indices>
	    static auto setFromPointerList(IndexPack& ipack,
					   std::vector<std::shared_ptr<IndefinitIndexBase> >& ptrList,
					   std::shared_ptr<Indices>&... inds)
		-> decltype(PackNum<N-1>::setFromPointerList(ipack,
								     std::shared_ptr<decltype(std::get<N>(ipack))>,
								     newPtr, inds...))
	    {
		typedef std::shared_ptr<decltype(std::get<N>(ipack))> NewIndexPtrType;
		NewIndexPtrType newPtr = dynamic_pointer_cast<NewIndexPtrType>(ptrList.at(N));
		return PackNum<N-1>::setFromPointerList(ipack, ptrList, newPtr, inds...);
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

	    template <class... Indices>
	    static void copy(std::tuple<std::shared_ptr<Indices>...>& ip,
			     const MultiIndex<Indices...>& ind)
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

	    template <typename... Ts>
	    static void print(std::ostream& os, const std::tuple<Ts...>& meta)
	    {
		PackNum<N-1>::print(os, meta);
		os << std::get<N>(meta) << '\t';
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

	    template <class MultiIndex>
	    static inline void pp(std::tuple<std::shared_ptr<Indices>...>& ip)
	    {
		auto& si = *std::get<0>(ip);
		++si;
	    }

	    template <class MultiIndex>
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

	    template <class... Ranges>
	    static void setBegin(MultiRange<Ranges...>::IndexType& i, const MultiRange<Ranges...>::SpaceType& r)
	    {
		*std::get<0>(i) = std::get<0>(r)->begin();
	    }
	    
	    template <class... Ranges>
	    static void setEnd(MultiRange<Ranges...>::IndexType& i, const MultiRange<Ranges...>::SpaceType& r)
	    {
		*std::get<0>(i) = std::get<0>(r)->end();
		*std::get<0>(i) -= 1;
	    }

	    template <class... Ranges>
	    static void buildRangeVec(std::vector<MultiRangeType>& rvec, const MultiRange<Ranges...>::SpaceType& rs)
	    {
		rvec.push_back(std::get<sizeof...(Ranges)-1>(rs)->type());
	    }

	    template <class... Indices>
	    static void getMetaPos(std::tuple<decltype(Indices().meta())...>& target,
				   const typename MultiIndex<Indices...>::IndexPack& source)
	    {
		std::get<0>(target) = std::get<0>(source)->meta();
	    }

	    template <class... Indices>
	    static void setMeta(typename MultiIndex<Indices...>::IndexPack& target,
				const typename MultiIndex<Indices...>::MetaType& source)
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
	    
	    template <class IndexPack, class... Indices>
	    static auto setFromPointerList(IndexPack& ipack,
					   std::vector<std::shared_ptr<IndefinitIndexBase> >& ptrList,
					   std::shared_ptr<Indices>&... inds)
		-> decltype(std::make_tuple(std::shared_ptr<decltype(std::get<0>(ipack))>, inds...))
	    {
		typedef std::shared_ptr<decltype(std::get<0>(ipack))> NewIndexPtrType;
		NewIndexPtrType newPtr = dynamic_pointer_cast<NewIndexPtrType>(ptrList.at(0));
		return std::make_tuple(newPtr, inds...);
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

	    template <class... Indices>
	    static void copy(std::tuple<std::shared_ptr<Indices>...>& ip,
			     const MultiIndex<Indices...>& ind)
	    {
		typedef decltype(ind.template get<0>()) SubIndexType;
		std::get<0>(ip).swap( std::make_shared<SubIndexType>( ind.template get<0>() ) );
	    }

	    template <class... Indices>
	    static size_t makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup)
	    {
		return std::get<0>(iPtrTup)->pos();
	    }

	    template <typename... Ts>
	    static void print(std::ostream& os, const std::tuple<Ts...>& meta)
	    {
		os << std::get<0>(meta) << '\t';
	    }
	};
    }

    template <class... Indices>
    MultiIndex<Indices...>::MultiIndex(const MultiIndex<Indices...>& in) :
	IndexInterface<std::tuple<decltype(Indices().meta())...> >(in)
    {
	PackNum<sizeof...(Indices)-1>::copy(mIPack, in);
	mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
    }
	
    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator=(const MultiIndex<Indices...>& in)
    {
	IndexI::operator=(in);
	PackNum<sizeof...(Indices)-1>::copy(mIPack, in);
	mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
	return *this;
    }
    
    template <class... Indices>
    template <class MRange>
    MultiIndex<Indices...>::MultiIndex(const std::shared_ptr<MRange>& range) :
	IndexInterface<std::tuple<decltype(Indices().meta())...> >(range, 0)
    {
	PackNum<sizeof...(Indices)-1>::construct(mIPack, *range);
	mPos = PackNum<sizeof...(Indices)-1>::makePos(mIPack);
    }
    
    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator++()
    {
	PackNum<sizeof...(Indices)-1>::pp( mIPack );
	++mPos;
	return *this;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator--()
    {
	PackNum<sizeof...(Indices)-1>::mm( mIPack );
	--mPos;
	return *this;
    }
    
    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator=(size_t pos)
    {
	mPos = pos;
	PackNum<sizeof...(Indices)-1>::setIndexPack(mIPack, pos);
	return *this;
    }

    template <class... Indices>
    template <size_t DIR>
    MultiIndex<Indices...>& MultiIndex<Indices...>::up()
    {
	static_assert(DIR < sizeof...(Indices), "DIR exceeds number of sub-indices");
	mPos += /*!!!*/;
	PackNum<DIR>::pp( mIPack );
	return *this;
    }

    template <class... Indices>
    template <size_t DIR>
    MultiIndex<Indices...>& MultiIndex<Indices...>::down()
    {
	static_assert(DIR < sizeof...(Indices), "DIR exceeds number of sub-indices");
	mPos -= /*!!!*/;
	PackNum<DIR>::mm( mIPack );
	return *this;
    }
    
    template <class... Indices>
    size_t MultiIndex<Indices...>::dim() const
    {
	return sizeof...(Indices);
    }
    
    template <class... Indices>
    template <size_t N>
    auto MultiIndex<Indices...>::get() const -> const decltype(*std::get<N>(mIPack))&
    {
	return *std::get<N>(mIPack);
    }
    
    template <class... Indices>
    const IndexBase& MultiIndex<Indices...>::get(size_t n) const
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	MultiIndex<Indices...> const* t = this;
	return IndexGetter<sizeof...(Indices)-1>::getIndex(*t, n);
    }

    template <class... Indices>
    typename MultiIndex<Indices...>::MetaType MultiIndex<Indices...>::meta() const
    {
        MetaType metaTuple;
	PackNum<sizeof...(Indices)-1>::getMetaPos(metaTuple, mIPack);
	return metaTuple;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::at(const MultiIndex<Indices...>::MetaType& metaPos)
    {
	PackNum<sizeof...(Indices)-1>::setMeta(mIPack, metaPos);
	return *this;
    }
    
    /******************
     *   MultiRange   *
     ******************/

    template <class... Ranges>
    MultiRange<Ranges...>::MultiRange(const std::shared_ptr<Ranges>&... rs) : mSpace(std::make_tuple(rs...)) {}

    template <class... Ranges>
    MultiRange<Ranges...>::MultiRange(const SpaceType& space) : mSpace(space) {}

    template <class... Ranges>
    template <size_t N>
    auto getRange() const -> const decltype(*std::get<N>(mSpace))&
    //typename std::tuple_element<N, std::tuple<Ranges...> >::type const& MultiRange<Ranges...>::getRange() const
    {
	return *std::get<N>(mSpace);
    }

    template <class... Ranges>
    size_t MultiRange<Ranges...>::size() const
    {
	return PackNum<sizeof...(Ranges)-1>::getSize(mSpace);
    }
    
    template <class... Ranges>
    MultiRangeType MultiRange<Ranges...>::type() const
    {
	std::vector<MultiRangeType> rvec;
	PackNum<sizeof...(Ranges)-1>::buildRangeVec(rvec, mSpace);
	return MultiRangeType(rvec);
    }

    template <class... Ranges>
    const typename MultiRange<Ranges...>::SpaceType& MultiRange<Ranges...>::space() const
    {
	return mSpace;
    }
    
    template <class... Ranges>
    MultiIndex<typename Ranges::IndexType...> MultiRange<Ranges...>::begin() const
    {
	std::tuple<std::shared_ptr<typename Ranges::IndexType>...> is;
	PackNum<sizeof...(Ranges)-1>::setBegin(is,mSpace);
	return MultiIndex<typename Ranges::IndexType...>(this, is);
    }

    template <class... Ranges>
    MultiIndex<typename Ranges::IndexType...> MultiRange<Ranges...>::end() const
    {
	std::tuple<std::shared_ptr<typename Ranges::IndexType>...> is;
	PackNum<sizeof...(Ranges)-1>::setEnd(is,mSpace);
	return ++MultiIndex<typename Ranges::IndexType...>(this, is);
    }

    template <class... Ranges>
    std::shared_ptr<IndefinitIndexBase> MultiRange<Ranges...>::indexInstance() const
    {
	std::tuple<std::shared_ptr<typename Ranges::IndexType>...> is;
	PackNum<sizeof...(Ranges)-1>::setBegin(is,mSpace);
	std::shared_ptr<IndefinitIndexBase> sptr(new MultiIndex<typename Ranges::IndexType...>(this, is));
	return sptr;
    }
}

template <typename... Ts>
std::ostream& operator<<(std::ostream& os,
			 const std::tuple<Ts...>& meta)
{
    MultiArrayTools::PackNum<sizeof...(Ts)-1>::print(os, meta);
    return os;
}
