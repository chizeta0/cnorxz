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

#include "map_range_factory_product_map.h"
#include "ranges/x_to_string.h"
#include "ranges/type_map.h"

#include "xfor/xfor.h"

namespace CNORXZ
{
    namespace
    {
	using namespace CNORXZInternal;
    }


    template <class Func, class... Indices>
    auto mkMapOp(const std::shared_ptr<Func>& func,
		 const std::shared_ptr<Indices>&... is)
	-> decltype(std::make_tuple(FunctionalMultiArray<typename Func::value_type,Func,
                                    typename Indices::RangeType...>().exec(is...),
                                    FunctionalMultiArray<typename Func::value_type,Func,
                                    typename Indices::RangeType...>()))
    {
        typedef FunctionalMultiArray<typename Func::value_type,Func,typename Indices::RangeType...> FMA;
        if(Func::FISSTATIC){
            FMA fma(is->range()...);
            return std::make_tuple(fma.exec(is...),fma);
        }
        else {
            FMA fma(is->range()...,func);
            return std::make_tuple(fma.exec(is...),fma);
        }
    }

    
    
    template <class Op, class Index, class Expr, SpaceType STYPE = SpaceType::ANY>
    //template <class MapF, class IndexPack, class Expr, SpaceType STYPE = SpaceType::ANY>
    class OpExpr : public ExpressionBase
    {
    public:
        //typedef typename Index::OIType OIType;
	//typedef SingleIndex<typename Op::value_type,STYPE> OIType;
	static constexpr size_t LAYER = Expr::LAYER + 1;
	static constexpr size_t SIZE = Expr::SIZE + Op::SIZE;
	static constexpr size_t NHLAYER = Expr::NHLAYER + 1;
	
    private:
	OpExpr() = default;

        const Index* mIndPtr;
	//const OIType* mIndPtr;
	size_t mSPos;
	size_t mMax;
	size_t mStep;
	Expr mExpr;
	Op mOp;
	
	typedef decltype(mOp.rootSteps(std::declval<intptr_t>()).extend( mExpr.rootSteps(std::declval<intptr_t>()) )) ExtType;
	ExtType mExt;

    public:
	OpExpr(const OpExpr& in) = default;
	OpExpr(OpExpr&& in) = default;
	OpExpr& operator=(const OpExpr& in) = default;
	OpExpr& operator=(OpExpr&& in) = default;

        OpExpr(const Op& mapf, const Index* ind, size_t step, Expr ex);

	virtual std::shared_ptr<ExpressionBase> deepCopy() const override final;

	template <size_t VS>
	inline auto vec() const { return *this; }
	
	inline void operator()(size_t mlast, DExt last) override final;
	inline void operator()(size_t mlast, ExtType last);
	inline void operator()(size_t mlast = 0) override final;

	auto rootSteps(std::intptr_t iPtrNum = 0) const -> ExtType;

	virtual DExt dRootSteps(std::intptr_t iPtrNum = 0) const override final;
	virtual DExt dExtension() const override final;
	
    };
    
    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    class GenMapIndex : public IndexInterface<GenMapIndex<OIType,Op,XSTYPE,Indices...>,
                                              typename Op::value_type>
                                              //std::tuple<typename Indices::MetaType...> >
    {
    public:
	
	typedef IndexInterface<GenMapIndex<OIType,Op,XSTYPE,Indices...>,
                               typename Op::value_type> IB;
			       //std::tuple<typename Indices::MetaType...> > IB;
	typedef std::tuple<std::shared_ptr<Indices>...> IndexPack;
	//typedef std::tuple<typename Indices::MetaType...> MetaType;
        typedef typename Op::value_type MetaType;
	typedef GenMapRange<typename OIType::RangeType,Op,XSTYPE,typename Indices::RangeType...> RangeType;
	typedef GenMapIndex IType;
	//typedef SingleIndex<typename Op::value_type,XSTYPE> OIType;
	
	static constexpr IndexType sType() { return IndexType::SINGLE; }
	static constexpr size_t sDim() { return sizeof...(Indices); }
	static constexpr size_t totalDim() { return (... * Indices::totalDim()); }
	static void check_type() { static_assert( std::is_same<typename OIType::MetaType,typename Op::value_type>::value, "inconsitent value types" ); }
	
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
	
	// raplace instances (in contrast to its analogon in ConstContainerIndex
	// MultiIndices CANNOT be influences be its subindices, so there is
	// NO foreign/external controll)
	// Do NOT share index instances between two or more MapIndex instances
	GenMapIndex& operator()(const std::shared_ptr<Indices>&... indices);
	GenMapIndex& operator()(const std::tuple<std::shared_ptr<Indices>...>& indices);

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
        size_t posAt(const MetaType& metaPos) const;
        
	size_t dim() const;
	bool first() const;
	bool last() const;
	std::shared_ptr<RangeType> range() const;

	template <size_t N>
	auto getPtr() -> decltype( std::get<N>( mIPack ) )&;

	size_t getStepSize(size_t n) const;

	template <class Exprs>
	auto ifor(size_t step, Exprs exs) const; // first step arg not used!

        template <class Exprs>
	auto pifor(size_t step, Exprs exs) const; // NO MULTITHREADING
	
	template <class Exprs>
	auto iforh(size_t step, Exprs exs) const;
	
    };

    
    /*************************
     *   MapRangeFactory   *
     *************************/

    // NOT THREAD SAVE
    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    class GenMapRangeFactory : public RangeFactoryBase
    {
    public:
	//typedef SingleRange<typename Op::value_type,XSTYPE> ORType;
	typedef GenMapRange<ORType,Op,XSTYPE,Ranges...> oType;
	
	GenMapRangeFactory() = delete;

        template <class MA>
        GenMapRangeFactory(const std::shared_ptr<ORType>& outr, const std::tuple<Op,MA>& mapf,
                           const std::shared_ptr<Ranges>&... rs);

        template <class MA>
        GenMapRangeFactory(const std::shared_ptr<ORType>& outr, const std::tuple<Op,MA>& mapf,
                           const typename GenMapRange<ORType,Op,XSTYPE,Ranges...>::Space& st);

        template <class MA>
	GenMapRangeFactory(const std::tuple<Op,MA>& mapf, const std::shared_ptr<Ranges>&... rs);

        template <class MA>
	GenMapRangeFactory(const std::tuple<Op,MA>& mapf,
                           const typename GenMapRange<ORType,Op,XSTYPE,Ranges...>::Space& space);

	virtual std::shared_ptr<RangeBase> create() override;

    private:
	
	std::shared_ptr<RangeBase> checkIfCreated(const std::tuple<std::shared_ptr<Ranges>...>& ptp);
	
    };
    
    /******************
     *   MapRange   *
     ******************/
    
    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    class GenMapRange : public RangeInterface<GenMapIndex<typename ORType::IndexType,Op,XSTYPE,typename Ranges::IndexType...> >
    {
    public:
	typedef RangeBase RB;
	typedef std::tuple<std::shared_ptr<Ranges>...> Space;
	typedef GenMapIndex<typename ORType::IndexType,Op,XSTYPE,typename Ranges::IndexType...> IndexType;
        typedef typename Op::value_type MetaType;

    protected:
	GenMapRange() = delete;
	GenMapRange(const GenMapRange& in) = delete;
	GenMapRange& operator=(const GenMapRange& in) = delete;

        template <class MA>
        GenMapRange(const std::shared_ptr<ORType>& outr, const std::tuple<Op,MA>& mapf,
                    const std::shared_ptr<Ranges>&... rs);

        template <class MA>
        GenMapRange(const std::shared_ptr<ORType>& outr, const std::tuple<Op,MA>& mapf,
                    const Space& space);

        template <class MA>
        GenMapRange(const std::tuple<Op,MA>& mapf, const Space& space);

        template <class MA>
        GenMapRange(const std::tuple<Op,MA>& mapf, const std::shared_ptr<Ranges>&... rs);
	
	Space mSpace;
        Op mMapf;
	//Op mMapf;
	std::shared_ptr<ORType> mOutRange;
	MultiArray<size_t,ORType> mMapMult;
        vector<size_t> mMapPos;
        
    private:
        template <class MA>
	void mkOutRange(const MA& mapf);
	
    public:
	
	static constexpr size_t sdim = sizeof...(Ranges);

	template <size_t N>
	auto get() const -> decltype( *std::get<N>( mSpace ) )&;

	template <size_t N>
	auto getPtr() const -> decltype( std::get<N>( mSpace ) )&;

        std::shared_ptr<ORType> outRange() const;
	const Op& map() const;
	
	virtual size_t dim() const final;
	virtual size_t size() const final;

	virtual SpaceType spaceType() const final;
        virtual DataHeader dataHeader() const final;
        
        virtual vector<size_t> typeNum() const final;
        virtual size_t cmeta(char* target, size_t pos) const final;
        virtual size_t cmetaSize() const final;
	virtual std::string stringMeta(size_t pos) const final;
	virtual vector<char> data() const final;
	
	const Space& space() const;
	
	virtual IndexType begin() const final;
	virtual IndexType end() const final;

	const MultiArray<size_t,ORType>& mapMultiplicity() const;
	ConstSlice<size_t,GenMapRange> explMapMultiplicity() const;

        vector<size_t> mapPos() const;
        
	/*
	template <class... ERanges>
	auto cat(const std::shared_ptr<GenMapRange<ERanges...> >& erange)
	    -> std::shared_ptr<GenMapRange<Ranges...,ERanges...> >;
        */
	friend GenMapRangeFactory<ORType,Op,XSTYPE,Ranges...>;

	static constexpr bool HASMETACONT = false;
	static constexpr bool defaultable = false;
	static constexpr size_t ISSTATIC = Op::ISSTATIC & (... & Ranges::ISSTATIC);
	static constexpr size_t SIZE = Op::SIZE * (... * Ranges::SIZE);
    };

    // for legacy
    template <class OIType, class Op, class... Indices>
    using MapIndex = GenMapIndex<OIType,Op,SpaceType::ANY,Indices...>;

    template <class ORType, class Op, class... Ranges>
    using MapRangeFactory = GenMapRangeFactory<ORType,Op,SpaceType::ANY,Ranges...>;

    template <class ORType, class Op, class... Ranges>
    using MapRange = GenMapRange<ORType,Op,SpaceType::ANY,Ranges...>;

    template <class OIType, class Op, class... Indices>
    auto mapResult/*<MapIndex<Op,Indices...> >*/(const std::shared_ptr<MapIndex<OIType,Op,Indices...> >& ind)
	-> decltype(ind->outIndex())
    {
	return ind->outIndex();
    }
}


#endif
