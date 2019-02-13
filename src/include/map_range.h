// -*- C++ -*-

#ifndef __map_range_h__
#define __map_range_h__

#include <cstdlib>
#include <tuple>
#include <memory>
#include <map>

#include "mbase_def.h"
#include "ranges/range_base.h"
#include "ranges/index_base.h"

#include "ranges/rpack_num.h"
#include "map_range_factory_product_map.h"
#include "ranges/x_to_string.h"
#include "ranges/type_map.h"

#include "xfor/xfor.h"

namespace MultiArrayTools
{
    namespace
    {
	using namespace MultiArrayHelper;
    }

    template <class MA, class... Indices>
    auto mkMapOp(const MA& ma,
		 const std::tuple<std::shared_ptr<Indices>...>& itp)
	-> decltype(PackNum<sizeof...(Indices)-1>::mkMapOp(ma, itp))
    {
	return PackNum<sizeof...(Indices)-1>::mkMapOp(ma, itp);
    }
    
    template <class MapF, class IndexPack, class Expr, SpaceType STYPE = SpaceType::ANY>
    class OpExpr
    {
    public:
	typedef SingleIndex<typename MapF::value_type,STYPE> OIType;
	//typedef typename MapF::IndexPack IndexPack;
	static constexpr size_t LAYER = Expr::LAYER + 1;
	static constexpr size_t SIZE = Expr::SIZE;
	
    private:
	OpExpr() = default;
	
	const OIType* mIndPtr;
	size_t mSPos;
	size_t mMax;
	size_t mStep;
	Expr mExpr;

	typedef decltype(mkMapOp(std::declval<MapF>(), std::declval<IndexPack>())) Op;
	Op mOp;
	
	typedef decltype(mOp.rootSteps(std::declval<intptr_t>()).extend( mExpr.rootSteps(std::declval<intptr_t>()) )) ExtType;
	ExtType mExt;

    public:
	OpExpr(const OpExpr& in) = default;
	OpExpr(OpExpr&& in) = default;
	OpExpr& operator=(const OpExpr& in) = default;
	OpExpr& operator=(OpExpr&& in) = default;
	
	OpExpr(const MapF& mapf, const IndexPack& ipack, const std::shared_ptr<OIType>& oind, size_t step, Expr ex);

	inline void operator()(size_t mlast, ExtType last) const;
	inline void operator()(size_t mlast = 0) const;

	auto rootSteps(std::intptr_t iPtrNum = 0) const -> ExtType;

    };
    
    template <class MapF, SpaceType XSTYPE, class... Indices>
    class GenMapIndex : public IndexInterface<GenMapIndex<MapF,XSTYPE,Indices...>,
                                              std::tuple<typename Indices::MetaType...> >
    {
    public:
	
	typedef IndexInterface<GenMapIndex<MapF,XSTYPE,Indices...>,
			       std::tuple<typename Indices::MetaType...> > IB;
	typedef std::tuple<std::shared_ptr<Indices>...> IndexPack;
	typedef std::tuple<typename Indices::MetaType...> MetaType;
	typedef GenMapRange<MapF,XSTYPE,typename Indices::RangeType...> RangeType;
	typedef GenMapIndex IType;
	typedef SingleIndex<typename MapF::value_type,XSTYPE> OIType;
	
	static constexpr IndexType sType() { return IndexType::MULTI; }
	static constexpr size_t sDim() { return sizeof...(Indices); }
	static constexpr size_t totalDim() { return mkTotalDim<Indices...>(); }

        static constexpr SpaceType STYPE = XSTYPE;
        static constexpr bool PARALLEL = false;
        
    private:
	
	IndexPack mIPack;
	std::array<size_t,sizeof...(Indices)+1> mBlockSizes;
	std::shared_ptr<OIType> mOutIndex;
	
    public:

	const IndexPack& pack() const { return mIPack; }
	
	GenMapIndex() = delete;
	
	// NO DEFAULT HERE !!!
	// ( have to assign sub-indices (ptr!) correctly )
	//MapIndex(const MapIndex& in);
	//MapIndex& operator=(const MapIndex& in);

	template <class MRange>
	GenMapIndex(const std::shared_ptr<MRange>& range);

	template <size_t DIR>
	GenMapIndex& up();

	template <size_t DIR>
	GenMapIndex& down();

	template <size_t N>
	auto get() const -> decltype( *std::get<N>( mIPack ) )&;

	template <size_t N>
	auto getPtr() const -> decltype( std::get<N>( mIPack ) )&;

	template <size_t N>
	size_t getBlockSize() const { return std::get<N>(mBlockSizes); }
	
	std::shared_ptr<OIType> outIndex() const;
	
	// raplace instances (in contrast to its analogon in ContainerIndex
	// MultiIndices CANNOT be influences be its subindices, so there is
	// NO foreign/external controll)
	// Do NOT share index instances between two or more MapIndex instances
	GenMapIndex& operator()(std::shared_ptr<Indices>&... indices);

	// ==== >>>>> STATIC POLYMORPHISM <<<<< ====
	
	IndexType type() const;

	GenMapIndex& operator=(size_t pos);

	GenMapIndex& operator++();
	GenMapIndex& operator--();

	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);

	std::string stringMeta() const;
	MetaType meta() const;
	GenMapIndex& at(const MetaType& metaPos);
	
	size_t dim() const;
	bool first() const;
	bool last() const;
	std::shared_ptr<RangeType> range() const;

	template <size_t N>
	auto getPtr() -> decltype( std::get<N>( mIPack ) )&;

	size_t getStepSize(size_t n) const;

	std::string id() const;
	void print(size_t offset) const;

	template <class Exprs>
	auto ifor(size_t step, Exprs exs) const
	    -> decltype(RPackNum<sizeof...(Indices)-1>::mkForh
			(step, mIPack, mBlockSizes, OpExpr<MapF,IndexPack,Exprs,XSTYPE>( range()->map(), mIPack, mOutIndex, step, exs ) ) );
	// first step arg not used!

        template <class Exprs>
	auto pifor(size_t step, Exprs exs) const
	    -> decltype(ifor(step, exs)); // NO MULTITHREADING

	/*
	template <class Exprs>
	auto iforh(Exprs exs) const
	    -> decltype(RPackNum<sizeof...(Indices)-1>::mkForh(mIPack, exs));
	*/
    };

    
    /*************************
     *   MapRangeFactory   *
     *************************/

    // NOT THREAD SAVE
    template <class MapF, SpaceType XSTYPE, class... Ranges>
    class GenMapRangeFactory : public RangeFactoryBase
    {
    public:
	typedef GenMapRange<MapF,XSTYPE,Ranges...> oType;
	
	GenMapRangeFactory() = delete;
	GenMapRangeFactory(const MapF& mapf, const std::shared_ptr<Ranges>&... rs);
	GenMapRangeFactory(const MapF& mapf, const typename GenMapRange<MapF,XSTYPE,Ranges...>::Space& space);

	virtual std::shared_ptr<RangeBase> create() override;

    private:
	
	std::shared_ptr<RangeBase> checkIfCreated(const std::tuple<std::shared_ptr<Ranges>...>& ptp);
	
    };
    
    /******************
     *   MapRange   *
     ******************/
    
    template <class MapF, SpaceType XSTYPE, class... Ranges>
    class GenMapRange : public RangeInterface<GenMapIndex<MapF,XSTYPE,typename Ranges::IndexType...> >
    {
    public:
	typedef RangeBase RB;
	typedef std::tuple<std::shared_ptr<Ranges>...> Space;
	typedef GenMapIndex<MapF,XSTYPE,typename Ranges::IndexType...> IndexType;
	typedef GenMapRange RangeType;
	typedef SingleRange<typename MapF::value_type,XSTYPE> ORType;
	typedef SingleRangeFactory<typename MapF::value_type,XSTYPE> ORFType;
	//typedef typename RangeInterface<MapIndex<typename Ranges::IndexType...> >::IndexType IndexType;

    protected:
	GenMapRange() = delete;
	GenMapRange(const GenMapRange& in) = delete;
	GenMapRange& operator=(const GenMapRange& in) = delete;
	
	GenMapRange(const MapF& mapf, const std::shared_ptr<Ranges>&... rs);
	GenMapRange(const MapF& mapf, const Space& space);
	
	Space mSpace;
	const MapF& mMapf;
	std::shared_ptr<ORType> mOutRange;
	MultiArray<size_t,ORType> mMapMult;

    private:
	void mkOutRange();
	
    public:
	
	static constexpr size_t sdim = sizeof...(Ranges);

	template <size_t N>
	auto get() const -> decltype( *std::get<N>( mSpace ) )&;

	template <size_t N>
	auto getPtr() const -> decltype( std::get<N>( mSpace ) )&;

	std::shared_ptr<ORType> outRange() const;
	const MapF& map() const;
	
	virtual size_t dim() const final;
	virtual size_t size() const final;

	virtual SpaceType spaceType() const final;
        virtual DataHeader dataHeader() const final;
        
	virtual std::string stringMeta(size_t pos) const final;
	virtual vector<char> data() const final;
	
	const Space& space() const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;

	const MultiArray<size_t,ORType>& mapMultiplicity() const;
	MultiArray<size_t,GenMapRange> explMapMultiplicity() const;

	/*
	template <class... ERanges>
	auto cat(const std::shared_ptr<GenMapRange<ERanges...> >& erange)
	    -> std::shared_ptr<GenMapRange<Ranges...,ERanges...> >;
        */
	friend GenMapRangeFactory<MapF,XSTYPE,Ranges...>;

	static constexpr bool HASMETACONT = false;
	static constexpr bool defaultable = false;
	static constexpr size_t ISSTATIC = SubProp<MapF,Ranges...>::ISSTATIC;
	static constexpr size_t SIZE = SubProp<MapF,Ranges...>::SIZE;
    };

    // for legacy
    template <class MapF, class... Indices>
    using MapIndex = GenMapIndex<MapF,SpaceType::ANY,Indices...>;

    template <class MapF, class... Ranges>
    using MapRangeFactory = GenMapRangeFactory<MapF,SpaceType::ANY,Ranges...>;

    template <class MapF, class... Ranges>
    using MapRange = GenMapRange<MapF,SpaceType::ANY,Ranges...>;

    template <class MapF, class... Indices>
    auto mapResult/*<MapIndex<MapF,Indices...> >*/(const std::shared_ptr<MapIndex<MapF,Indices...> >& ind)
	-> decltype(ind->outIndex())
    {
	return ind->outIndex();
    }

}


#endif
