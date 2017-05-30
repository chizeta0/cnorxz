
#include "multi_range.h"

namespace MultiArrayTools
{
    /*******************
     *  MultiIndexBase *	     
     *******************/

    namespace
    {
	template <size_t N>
	struct IndexGetter
	{	
	    template <class MultiIndex>
	    static IndefinitIndexBase& getIndex(MultiIndex& in, size_t n)
	    {
		if(n == N){
		    return in.getIndex<N>();
		}
		else {
		    return IndexGetter<N-1>::getIndex(in, n);
		}
	    }

	    template <class MultiIndex>
	    static const IndefinitIndexBase& getIndex(const MultiIndex& in, size_t n)
	    {
		if(n == N){
		    return in.getIndex<N>();
		}
		else {
		    return IndexGetter<N-1>::getIndex(in, n);
		}
	    }
	};

	template <>
	struct IndexGetter<0>
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
	};

	template <size_t N>
	struct Evaluation
	{
	    template <class MultiIndex>
	    static size_t evaluate(const MultiIndex& index)
	    {
		const auto& subIndex = index.template getIndex<N>();
		return Evaluation<N-1>::evaluate(index) * subIndex.max() + subIndex.pos();
	    }
	};

	template <>
	struct Evaluation<0>
	{
	    template <class MultiIndex>
	    static size_t evaluate(const MultiIndex& index)
	    {
		const auto& subIndex = index.template getIndex<0>();
		return subIndex.pos();
	    }
	};

	// rewrite !!
	template <size_t DIGIT>
	struct SubIteration
	{
	    // use 'plus' as few as possible !!
	    template <class MultiIndex>
	    static inline void plus(MultiIndex& index, int num)
	    {
		auto& si = index.template getIndex<DIGIT>();
		si.setPosRel(num, &index);
		size_t oor = si.outOfRange();
		while(oor > 0){
		    SubIteration<DIGIT-1>::pp(index);
		    SubIteration<DIGIT>::plus(index, -si.max());
		}
		while(oor < 0){
		    SubIteration<DIGIT-1>::mm(index);
		    SubIteration<DIGIT>::plus(index, si.max());
		}
	    }

	    template <class MultiIndex>
	    static inline void pp(MultiIndex& index)
	    {
		auto& si = index.template getIndex<DIGIT>();
		if(si.pos() == si.atEdge()){
		    si.setPos(0, &index);
		    SubIteration<DIGIT-1>::pp(index);
		}
		else {
		    si.setPosRel(1, &index);
		}
	    }

	    template <class MultiIndex>
	    static inline void mm(MultiIndex& index)
	    {
		auto& si = index.template getIndex<DIGIT>();
		if(si.pos() == si.atEdge()){
		    si.setPos(si.atEdge(), &index);
		    SubIteration<DIGIT-1>::mm(index);
		}
		else {
		    si.setPosRel(1, &index);
		}
	    }
	};

	template <>
	struct SubIteration<0>
	{
	    template <class MultiIndex>
	    static inline void plus(MultiIndex& index, int num)
	    {
		auto& si = index.template getIndex<0>();
		si.setPosRel(num, &index);
	    }

	    template <class MultiIndex>
	    static inline void pp(MultiIndex& index)
	    {
		auto& si = index.template getIndex<0>();
		si.setPosRel(1, &index);
	    }

	    template <class MultiIndex>
	    static inline void mm(MultiIndex& index)
	    {
		auto& si = index.template getIndex<0>();
		si.setPosRel(-1, &index);
	    }
	};

	template <size_t N>
	struct IndexSubOrder
	{
	    template <class IndexPack>
	    static void subOrd(IndexPack& iPack, IndefinitIndexBase* major)
	    {
		std::get<N>(iPack).subOrd(major);
		IndexSubOrder<N-1>::subOrd(iPack, major);
	    }
	};
	
	template <>
	struct IndexSubOrder<0>
	{
	    template <class IndexPack>
	    static void subOrd(IndexPack& iPack, IndefinitIndexBase* major)
	    {
		std::get<0>(iPack).subOrd(major);
	    }
	};

	template <size_t N>
	struct PositionCopy
	{
	    template <class MultiIndex>
	    static void copyPos(MultiIndex& target, const MultiIndex& source)
	    {
		target.template getIndex<N>().copyPos( source.template getIndex<N>() );
		PositionCopy<N-1>::copyPos(target, source);
	    }
	};

	template <>
	struct PositionCopy<0>
	{
	    template <class MultiIndex>
	    static void copyPos(MultiIndex& target, const MultiIndex& source)
	    {
		target.template getIndex<0>().copyPos( source.template getIndex<0>() );
	    }
	};

	template <size_t N>
	struct TupleSize
	{
	    template <class RangeTuple>
	    static size_t getSize(const RangeTuple& rt)
	    {
		return std::get<N>(rt).size() * TupleSize<N-1>::getSize(rt);
	    }
	};
	
	template <>
	struct TupleSize<0>
	{
	    template <class RangeTuple>
	    static size_t getSize(const RangeTuple& rt)
	    {
		return std::get<0>(rt).size();
	    }
	};
	
	template <size_t N>
	struct IndexSetter
	{
	    template <class... Ranges>
	    static void setBegin(MultiRange<Ranges...>::IndexType& i, const MultiRange<Ranges...>::SpaceType& r)
	    {
		*std::get<N>(i) = std::get<N>(r)->begin();
		IndexSetter<N-1>::setBegin(i,r);
	    }
	    
	    template <class... Ranges>
	    static void setEnd(MultiRange<Ranges...>::IndexType& i, const MultiRange<Ranges...>::SpaceType& r)
	    {
		*std::get<N>(i) = std::get<N>(r)->end();
		*std::get<N>(i) -= 1;
		IndexSetter<N-1>::setEnd(i,r);
	    }
	};
	
	template <>
	struct IndexSetter<0>
	{
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
	};
	
	template <size_t N>
	struct RangeVecBuilder
	{
	    template <class... Ranges>
	    static void buildRangeVec(std::vector<MultiRangeType>& rvec, const MultiRange<Ranges...>::SpaceType& rs)
	    {
		rvec.push_back(std::get<sizeof...(Ranges)-N-1>(rs)->type());
		RangeVecBuilder<N-1>::buildRangeVec(rvec, rs);
	    }
	};
	template <>
	struct RangeVecBuilder<0>
	{
	    template <class... Ranges>
	    static void buildRangeVec(std::vector<MultiRangeType>& rvec, const MultiRange<Ranges...>::SpaceType& rs)
	    {
		rvec.push_back(std::get<sizeof...(Ranges)-1>(rs)->type());
	    }
	};
	// HERE !!!
	template <size_t N>
	struct MetaPosGetter
	{
	    template <class... Indices>
	    static void getMetaPos(std::tuple<decltype(Indices().getMetaPos())...>& target,
				   const typename MultiIndex<Indices...>::IndexPack& source)
	    {
		std::get<N>(target) = std::get<N>(source)->getMetaPos();
		MetaPosGetter<N-1>::getMetaPos(target, source);
	    }
	};

	template <>
	struct MetaPosGetter<0>
	{
	    template <class... Indices>
	    static void getMetaPos(std::tuple<decltype(Indices().getMetaPos())...>& target,
				   const typename MultiIndex<Indices...>::IndexPack& source)
	    {
		std::get<0>(target) = std::get<0>(source)->getMetaPos();
	    }
	};

	template <size_t N>
	struct MetaSetter
	{
	    template <class... Indices>
	    static void setMeta(typename MultiIndex<Indices...>::IndexPack& target,
				const typename MultiIndex<Indices...>::MetaType& source)
	    {
		std::get<N>(target).atMeta( std::get<N>(source) );
		MetaSetter<N-1>::setMeta(target, source);
	    }
	};

	template <>
	struct MetaSetter<0>
	{
	    template <class... Indices>
	    static void setMeta(typename MultiIndex<Indices...>::IndexPack& target,
				const typename MultiIndex<Indices...>::MetaType& source)
	    {
		std::get<0>(target).atMeta( std::get<0>(source) );
	    }
	};

	template <size_t N>
	struct IndexPackSetter
	{
	    template <class IndexPack>
	    static void setIndexPack(IndexPack& iPack, size_t pos)
	    {
		auto i = std::get<N>(iPack);
		const size_t ownPos = pos % i.max(); 
		i = ownPos;
		if(ownPos == pos){
		    IndexPackSetter<N-1>::setIndexPack(iPack, (pos - ownPos) / i.max() );
		}
	    }

	    template <class IndexPack, class... Indices>
	    static auto setFromPointerList(IndexPack& ipack,
					   std::vector<std::shared_ptr<IndefinitIndexBase> >& ptrList,
					   std::shared_ptr<Indices>&... inds)
		-> decltype(IndexPackSetter<N-1>::setFromPointerList(ipack,
								     std::shared_ptr<decltype(std::get<N>(ipack))>,
								     newPtr, inds...))
	    {
		typedef std::shared_ptr<decltype(std::get<N>(ipack))> NewIndexPtrType;
		NewIndexPtrType newPtr = dynamic_pointer_cast<NewIndexPtrType>(ptrList.at(N));
		return IndexPackSetter<N-1>::setFromPointerList(ipack, ptrList, newPtr, inds...);
	    }
	};

	template <>
	struct IndexPackSetter<0>
	{
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

	};

    }

    template <class... Indices>
    MultiIndex<Indices...>::MultiIndex(const MultiIndex<Indices...>& in) :
	IndexBase<MultiIndex<Indices...> >(in),
	mIPack(in.mIPack)
    {
	// THAT's the point:
	IndexSubOrder<sizeof...(Indices)-1>::subOrd(mIPack, this);
	IIB::mPos = evaluate(*this);
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator=(const MultiIndex<Indices...>& in)
    {
	IndexBase<MultiIndex<Indices...> >::operator=(in);
	mIPack = in.mIPack;

	// THAT's the point:
	IndexSubOrder<sizeof...(Indices)-1>::subOrd(mIPack, this);
	IIB::mPos = evaluate(*this);
	return *this;
    }
    
    template <class... Indices>
    MultiIndex<Indices...>::MultiIndex(RangeBase<MultiIndex<Indices...> > const* range) :
	IndexBase<MultiIndex<Indices...> >(range),
	mIPack()
    {
	operator=(IB::mRange->begin());
	IIB::mPos = evaluate(*this);
    }
    
    template <class... Indices>
    MultiIndex<Indices...>::MultiIndex(RangeBase<MultiIndex<Indices...> > const* range,
				       Indices&&... inds) : IndexBase<MultiIndex<Indices...> >(range),
							    mIPack(std::make_tuple(inds...))
    {
	IndexSubOrder<sizeof...(Indices)-1>::subOrd(mIPack, this);
	IIB::mPos = evaluate(*this);
    }

    template <class... Indices>
    MultiIndex<Indices...>::MultiIndex(RangeBase<MultiIndex<Indices...> > const* range,
				       const IndexPack& ipack) : IndexBase<MultiIndex<Indices...> >(range),
								 mIPack(ipack)
    {
	IndexSubOrder<sizeof...(Indices)-1>::subOrd(mIPack, this);
	IIB::mPos = evaluate(*this);
    }

    template <class... Indices>
    MultiIndex<Indices...>::MultiIndex(std::vector<std::shared_ptr<IndefinitIndexBase> >& indexList)
    {
	mIPack = IndexPackSetter<sizeof...(Indices)-1>::setFromPointerList(mIPack, indexList);
	IndexSubOrder<sizeof...(Indices)-1>::subOrd(mIPack, this);
    }
    
    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator++()
    {
	SubIteration<sizeof...(Indices)-1>::pp(*this);
	//plus(*this, sizeof...(Indices)-1, 1);
	IIB::setPosRel(1);
	return *this;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator--()
    {
	SubIteration<sizeof...(Indices)-1>::mm(*this);
	//plus(*this, sizeof...(Indices)-1, -1);
	IIB::setPosRel(-1);
	return *this;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator+=(int n)
    {
	SubIteration<sizeof...(Indices)-1>::plus(*this, n);
	//plus(*this, sizeof...(Indices)-1, n);
	IIB::setPosRel(n);
	return *this;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator-=(int n)
    {
	SubIteration<sizeof...(Indices)-1>::plus(*this, -n);
	//plus(*this, sizeof...(Indices)-1, -n);
	IIB::setPosRel(-n);
	return *this;
    }

    template <class... Indices>
    bool MultiIndex<Indices...>::operator==(const MultiIndex<Indices...>& in)
    {
	return IB::mRange == in.mRange and IIB::pos() == in.pos();
    }

    template <class... Indices>
    bool MultiIndex<Indices...>::operator!=(const MultiIndex<Indices...>& in)
    {
	return IB::mRange != in.mRange or IIB::pos() != in.pos();
    }
    
    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator=(size_t pos)
    {
	IIB::setPos( pos );
	IndexPackSetter<sizeof...(Indices)-1>::setIndexPack(mIPack, pos);
	return *this;
    }

    template <class... Indices>
    MultiRangeType MultiIndex<Indices...>::rangeType() const
    {
	return IB::mRange->type();
    }
    
    template <class... Indices>
    size_t MultiIndex<Indices...>::evaluate(const MultiIndex<Indices...>& in) const
    {
	size_t res = Evaluation<sizeof...(Indices)-1>::evaluate(in);
	return res;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator()(Indices&&... inds)
    {
	mIPack = std::make_tuple(inds...);
	IndexSubOrder<sizeof...(Indices)-1>::subOrd(mIPack, this);
	IIB::mPos = evaluate(*this);
	return *this;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::operator()(const Indices&... inds)
    {
	mIPack = std::make_tuple(Indices(inds)...);
	IndexSubOrder<sizeof...(Indices)-1>::subOrd(mIPack, this);
	IIB::mPos = evaluate(*this);
	return *this;
    }
    
    template <class... Indices>
    size_t MultiIndex<Indices...>::dim() const
    {
	size_t res = 1;
	for(size_t i = 0; i != sizeof...(Indices); ++i){
	    res *= get(i).dim();
	}
	return res;
    }
    
    template <class... Indices>
    template <size_t N>
    auto getIndex() -> decltype(*std::get<N>(mIPack))&
    //typename std::tuple_element<N, std::tuple<Indices...> >::type& MultiIndex<Indices...>::getIndex()
    {
	return *std::get<N>(mIPack);
    }

    template <class... Indices>
    template <size_t N>
    auto getIndex() const -> const decltype(*std::get<N>(mIPack))&
    {
	return *std::get<N>(mIPack);
    }
    
    template <class... Indices>
    IndefinitIndexBase& MultiIndex<Indices...>::get(size_t n)
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	MultiIndex<Indices...>* t = this;
	return IndexGetter<sizeof...(Indices)-1>::getIndex(*t, n);
    }

    template <class... Indices>
    const IndefinitIndexBase& MultiIndex<Indices...>::get(size_t n) const
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	MultiIndex<Indices...> const* t = this;
	return IndexGetter<sizeof...(Indices)-1>::getIndex(*t, n);
    }

    template <class... Indices>
    typename MultiIndex<Indices...>::MetaType MultiIndex<Indices...>::getMetaPos() const
    {
        MetaType metaTuple;
	MetaPosGetter<sizeof...(Indices)-1>::getMetaPos(metaTuple, mIPack);
	return metaTuple;
    }

    template <class... Indices>
    MultiIndex<Indices...>& MultiIndex<Indices...>::atMeta(const MultiIndex<Indices...>::MetaType& metaPos)
    {
	MetaSetter<sizeof...(Indices)-1>::setMeta(mIPack, metaPos);
	return *this;
    }

    template <class... Indices>
    void MultiIndex<Indices...>::copyPos(const MultiIndex<Indices...>& in)
    {
	PositionCopy<sizeof...(Indices)-1>::copyPos(*this, in);
    }

    template <class... Indices>
    size_t MultiIndex<Indices...>::giveSubStepSize(IndefinitIndexBase* subIndex)
    {
	size_t sss = 1;
	for(size_t i = sizeof...(Indices)-1; i != 0; --i){
	    IndefinitIndexBase* iibPtr = &get(i);
	    if(iibPtr == subIndex){
		return sss;
	    }
	    sss *= iibPtr->max();
	}
	if(&get(0) == subIndex){
	    return sss;
	}
	else {
	    return 0;
	}
    }
    
    template <size_t N>
    struct TuplePrinter
    {
	template <typename... Ts>
	static void print(std::ostream& os, const std::tuple<Ts...>& meta)
	{
	    TuplePrinter<N-1>::print(os, meta);
	    os << std::get<N>(meta) << '\t';
	}
    };

    template <>
    struct TuplePrinter<0>
    {
	template <typename... Ts>
	static void print(std::ostream& os, const std::tuple<Ts...>& meta)
	{
	    os << std::get<0>(meta) << '\t';
	}
    };
    
    /******************
     *   MultiRange   *
     ******************/

    template <class... Ranges>
    MultiRange<Ranges...>::MultiRange(const std::shared_ptr<Ranges>&... rs) : mSpace(std::make_tuple(rs...)) {}

    template <class... Ranges>
    MultiRange<Ranges...>::MultiRange(const SpaceType& space) : mSpace(space) {}
    
    template <class... Ranges>
    template <size_t N>
    auto getRange() -> decltype(*std::get<N>(mSpace))&
    //typename std::tuple_element<N, std::tuple<Ranges...> >::type& MultiRange<Ranges...>::getRange()
    {
	return *std::get<N>(mSpace);
    }

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
	return TupleSize<sizeof...(Ranges)-1>::getSize(mSpace);
    }
    
    template <class... Ranges>
    MultiRangeType MultiRange<Ranges...>::type() const
    {
	std::vector<MultiRangeType> rvec;
	RangeVecBuilder<sizeof...(Ranges)-1>::buildRangeVec(rvec, mSpace);
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
	IndexSetter<sizeof...(Ranges)-1>::setBegin(is,mSpace);
	return MultiIndex<typename Ranges::IndexType...>(this, is);
    }

    template <class... Ranges>
    MultiIndex<typename Ranges::IndexType...> MultiRange<Ranges...>::end() const
    {
	std::tuple<std::shared_ptr<typename Ranges::IndexType>...> is;
	IndexSetter<sizeof...(Ranges)-1>::setEnd(is,mSpace);
	return ++MultiIndex<typename Ranges::IndexType...>(this, is);
    }

    template <class... Ranges>
    std::shared_ptr<IndefinitIndexBase> MultiRange<Ranges...>::indexInstance() const
    {
	std::tuple<std::shared_ptr<typename Ranges::IndexType>...> is;
	IndexSetter<sizeof...(Ranges)-1>::setBegin(is,mSpace);
	std::shared_ptr<IndefinitIndexBase> sptr(new MultiIndex<typename Ranges::IndexType...>(this, is));
	return sptr;
    }
}

template <typename... Ts>
std::ostream& operator<<(std::ostream& os,
			 const std::tuple<Ts...>& meta)
{
    MultiArrayTools::TuplePrinter<sizeof...(Ts)-1>::print(os, meta);
    return os;
}
