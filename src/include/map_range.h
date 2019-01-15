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
    
    template <class MapF, class IndexPack, class Expr>
    class OpExpr
    {
    public:
	typedef SingleIndex<typename MapF::value_type,SpaceType::ANY> OIType;
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
    
    template <class MapF, class... Indices>
    class MapIndex : public IndexInterface<MapIndex<MapF,Indices...>,
					     std::tuple<typename Indices::MetaType...> >
    {
    public:
	
	typedef IndexInterface<MapIndex<MapF,Indices...>,
			       std::tuple<typename Indices::MetaType...> > IB;
	typedef std::tuple<std::shared_ptr<Indices>...> IndexPack;
	typedef std::tuple<typename Indices::MetaType...> MetaType;
	typedef MapRange<MapF,typename Indices::RangeType...> RangeType;
	typedef MapIndex IType;
	typedef SingleIndex<typename MapF::value_type,SpaceType::ANY> OIType;
	
	static constexpr IndexType sType() { return IndexType::MULTI; }
	static constexpr size_t sDim() { return sizeof...(Indices); }
	static constexpr size_t totalDim() { return mkTotalDim<Indices...>(); }

	static constexpr SpaceType STYPE = SpaceType::ANY;
        static constexpr bool PARALLEL = false;
        
    private:
	
	IndexPack mIPack;
	std::array<size_t,sizeof...(Indices)+1> mBlockSizes;
	std::shared_ptr<OIType> mOutIndex;
	
    public:

	const IndexPack& pack() const { return mIPack; }
	
	MapIndex() = delete;
	
	// NO DEFAULT HERE !!!
	// ( have to assign sub-indices (ptr!) correctly )
	//MapIndex(const MapIndex& in);
	//MapIndex& operator=(const MapIndex& in);

	template <class MRange>
	MapIndex(const std::shared_ptr<MRange>& range);

	template <size_t DIR>
	MapIndex& up();

	template <size_t DIR>
	MapIndex& down();

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
	MapIndex& operator()(std::shared_ptr<Indices>&... indices);

	// ==== >>>>> STATIC POLYMORPHISM <<<<< ====
	
	IndexType type() const;

	MapIndex& operator=(size_t pos);

	MapIndex& operator++();
	MapIndex& operator--();

	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);

	std::string stringMeta() const;
	MetaType meta() const;
	MapIndex& at(const MetaType& metaPos);
	
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
			(step, mIPack, mBlockSizes, OpExpr<MapF,IndexPack,Exprs>( range()->map(), mIPack, mOutIndex, step, exs ) ) );
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

    template <class MapF, class... Indices>
    auto mapResult/*<MapIndex<MapF,Indices...> >*/(const std::shared_ptr<MapIndex<MapF,Indices...> >& ind)
	-> decltype(ind->outIndex())
    {
	return ind->outIndex();
    }
    
    /*************************
     *   MapRangeFactory   *
     *************************/

    // NOT THREAD SAVE
    template <class MapF, class... Ranges>
    class MapRangeFactory : public RangeFactoryBase
    {
    public:
	typedef MapRange<MapF,Ranges...> oType;
	
	MapRangeFactory() = delete;
	MapRangeFactory(const MapF& mapf, const std::shared_ptr<Ranges>&... rs);
	MapRangeFactory(const MapF& mapf, const typename MapRange<MapF,Ranges...>::Space& space);

	virtual std::shared_ptr<RangeBase> create() override;

    private:
	
	std::shared_ptr<RangeBase> checkIfCreated(const std::tuple<std::shared_ptr<Ranges>...>& ptp);
	
    };
    
    /******************
     *   MapRange   *
     ******************/
    
    template <class MapF, class... Ranges>
    class MapRange : public RangeInterface<MapIndex<MapF,typename Ranges::IndexType...> >
    {
    public:
	typedef RangeBase RB;
	typedef std::tuple<std::shared_ptr<Ranges>...> Space;
	typedef MapIndex<MapF,typename Ranges::IndexType...> IndexType;
	typedef MapRange RangeType;
	typedef SingleRange<typename MapF::value_type,SpaceType::ANY> ORType;
	typedef SingleRangeFactory<typename MapF::value_type,SpaceType::ANY> ORFType;
	//typedef typename RangeInterface<MapIndex<typename Ranges::IndexType...> >::IndexType IndexType;

    protected:
	MapRange() = delete;
	MapRange(const MapRange& in) = delete;
	MapRange& operator=(const MapRange& in) = delete;
	
	MapRange(const MapF& mapf, const std::shared_ptr<Ranges>&... rs);
	MapRange(const MapF& mapf, const Space& space);
	
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
	virtual std::vector<char> data() const final;
	
	const Space& space() const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;

	const MultiArray<size_t,ORType>& mapMultiplicity() const;
	MultiArray<size_t,MapRange> explMapMultiplicity() const;

	
	template <class... ERanges>
	auto cat(const std::shared_ptr<MapRange<ERanges...> >& erange)
	    -> std::shared_ptr<MapRange<Ranges...,ERanges...> >;
	
	friend MapRangeFactory<MapF,Ranges...>;

	static constexpr bool HASMETACONT = false;
	static constexpr bool defaultable = false;
	static constexpr size_t ISSTATIC = SubProp<MapF,Ranges...>::ISSTATIC;
	static constexpr size_t SIZE = SubProp<MapF,Ranges...>::SIZE;
    };
    
}


#endif
