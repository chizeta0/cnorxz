
#include "map_range.h"
#include <type_traits>

namespace MultiArrayTools
{

    namespace
    {
	using namespace MultiArrayHelper;
    }

    /**************
     *   OpExpr   *
     **************/

    template <class Op, class Index, class Expr, SpaceType STYPE>
    OpExpr<Op,Index,Expr,STYPE>::OpExpr(const Op& mapf, const Index* ind,
                                            size_t step, Expr ex) :
	mIndPtr(ind), mSPos(mIndPtr->pos()), mMax(mIndPtr->max()),
	mStep(step), mExpr( ex ),
	mOp(mapf),
	//mExt(ex.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr )))
	mExt( mOp.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr ) ).extend
	      ( ex.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr ) ) ) )
    {
        assert(mIndPtr != nullptr);
    }

    template <class Op, class Index, class Expr, SpaceType STYPE>
    std::shared_ptr<ExpressionBase> OpExpr<Op,Index,Expr,STYPE>::deepCopy() const
    {
	return std::make_shared<OpExpr<Op,Index,Expr,STYPE>>(*this);
    }
    
    template <class Op, class Index, class Expr, SpaceType STYPE>
    inline void OpExpr<Op,Index,Expr,STYPE>::operator()(size_t mlast, DExt last)
    {
	operator()(mlast, std::dynamic_pointer_cast<ExtT<ExtType>>(last)->ext());
    }

    template <class Op, class Index, class Expr, SpaceType STYPE>
    inline void OpExpr<Op,Index,Expr,STYPE>::operator()(size_t mlast,
                                                            ExtType last)
    {
	constexpr size_t NEXT = Op::SIZE;
	const ExtType nxpos = last;
	const size_t pos = mIndPtr->posAt( mOp.get( nxpos ) );
        if(pos != mIndPtr->max()){
            const ExtType npos = last + mExt*pos;
            const size_t mnpos = PosForward<ForType::DEFAULT>::valuex(mlast, mStep, pos);
            mExpr(mnpos, getX<NEXT>( npos ) );
        }
    }

    template <class Op, class Index, class Expr, SpaceType STYPE>
    inline void OpExpr<Op,Index,Expr,STYPE>::operator()(size_t mlast)
    {
	const ExtType last;
	constexpr size_t NEXT = Op::SIZE;
	const ExtType nxpos = last;
	const size_t pos = mIndPtr->posAt( mOp.get( nxpos ) );
        if(pos != mIndPtr->max()){
            const ExtType npos = last + mExt*pos;
            const size_t mnpos = PosForward<ForType::DEFAULT>::valuex(mlast, mStep, pos);
            mExpr(mnpos, getX<NEXT>( npos ));
        }
    }
    
    template <class Op, class Index, class Expr, SpaceType STYPE>
    auto OpExpr<Op,Index,Expr,STYPE>::rootSteps(std::intptr_t iPtrNum) const
	-> ExtType
    {
	return mOp.rootSteps(iPtrNum).extend( mExpr.rootSteps(iPtrNum) );
	//return mExpr.rootSteps(iPtrNum).extend( mOp.rootSteps(iPtrNum) );
    }

    template <class Op, class Index, class Expr, SpaceType STYPE>
    DExt OpExpr<Op,Index,Expr,STYPE>::dRootSteps(std::intptr_t iPtrNum) const
    {
	return std::make_shared<ExtT<ExtType>>(rootSteps(iPtrNum));
    }
    
    template <class Op, class Index, class Expr, SpaceType STYPE>
    DExt OpExpr<Op,Index,Expr,STYPE>::dExtension() const
    {
	return std::make_shared<ExtT<ExtType>>(mExt);
    }

    /******************
     *   MapIndex   *
     ******************/
    
    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    template <class MRange>
    GenMapIndex<OIType,Op,XSTYPE,Indices...>::GenMapIndex(const std::shared_ptr<MRange>& range) :
	IndexInterface<GenMapIndex<OIType,Op,XSTYPE,Indices...>,typename Op::value_type>(range, 0)
    {
	std::get<sizeof...(Indices)>(mBlockSizes) = 1;
	sfor_mn<sizeof...(Indices),0>
	    ( [&](auto i) {
		  auto r = range->template getPtr<i>();
		  std::get<i>(mIPack) = r->beginPtr();
		  *std::get<i>(mIPack) = 0;
		  
		  std::get<i>(mBlockSizes) = sfor_p<i,sizeof...(Indices)>
		      ( [&](auto j) { return std::get<j>(mIPack)->max(); } ,
			[&](auto a, auto b) { return a * b; });
		  return 0;
	      });
	
	IB::mPos = sfor_m<sizeof...(Indices),0>
	    ( [&](auto i) { return std::get<i>(mIPack); },
	      [&](auto a, auto b) {return a->pos() + b*a->max();}, 0 );
	mOutIndex = std::make_shared<OIType>
	    ( std::dynamic_pointer_cast<RangeType>( IB::mRangePtr )->outRange()->begin() );
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    template <size_t DIR>
    GenMapIndex<OIType,Op,XSTYPE,Indices...>& GenMapIndex<OIType,Op,XSTYPE,Indices...>::up()
    {
	static_assert(DIR < sizeof...(Indices), "DIR exceeds number of sub-indices");
	IB::mPos += sfor_p<DIR,sizeof...(Indices)>
	    ( [&](auto i) { return std::get<i>(mIPack)->max(); },
	      [&](auto a, auto b) { return a * b; } );
	sfor_m<DIR+1,0>
	    ( [&](auto i) {
		auto& si = *std::get<i>( mIPack );
		if(si.last() and i != 0) { si = 0; return true; }
		else { ++si; return false; }
		return false;
	    } );
	return *this;
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    template <size_t DIR>
    GenMapIndex<OIType,Op,XSTYPE,Indices...>& GenMapIndex<OIType,Op,XSTYPE,Indices...>::down()
    {
	static_assert(DIR < sizeof...(Indices), "DIR exceeds number of sub-indices");
	IB::mPos -= sfor_p<DIR,sizeof...(Indices)>
	    ( [&](auto i) { return std::get<i>(mIPack)->max(); },
	      [&](auto a, auto b) { return a * b; } );
	sfor_m<DIR+1,0>
	    ( [&](auto i) {
		auto& si = *std::get<i>( mIPack );
		if(si.first() and i != 0) { si = si.max()-1; return true; }
		else { --si; return false; }
		return false;
	    } );
	return *this;
    }
    
    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    template <size_t N>
    auto GenMapIndex<OIType,Op,XSTYPE,Indices...>::get() const -> decltype( *std::get<N>( mIPack ) )&
    {
	return *std::get<N>(mIPack);
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    template <size_t N>
    auto GenMapIndex<OIType,Op,XSTYPE,Indices...>::getPtr() const -> decltype( std::get<N>( mIPack ) )&
    {
	return std::get<N>(mIPack);
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    auto GenMapIndex<OIType,Op,XSTYPE,Indices...>::outIndex() const -> std::shared_ptr<OIType>
    {
	return mOutIndex;
    }
    
    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    GenMapIndex<OIType,Op,XSTYPE,Indices...>& GenMapIndex<OIType,Op,XSTYPE,Indices...>::operator()(const std::shared_ptr<Indices>&... indices)
    {
	//RPackNum<sizeof...(Indices)-1>::swapIndices(mIPack, indices...);
	//RPackNum<sizeof...(Indices)-1>::setIndexPack(mIPack, IB::mPos);
	return (*this)(std::make_tuple(indices...));
	//return *this;
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    GenMapIndex<OIType,Op,XSTYPE,Indices...>& GenMapIndex<OIType,Op,XSTYPE,Indices...>::operator()(const std::tuple<std::shared_ptr<Indices>...>& indices)
    {
	sfor_pn<0,sizeof...(Indices)>
	    ( [&](auto i) { std::get<i>(mIPack) = std::get<i>(indices); return 0; } );
	//RPackNum<sizeof...(Indices)-1>::swapIndices(mIPack, indices...);
	RangeHelper::setIndexPack<sizeof...(Indices)-1>(mIPack, IB::mPos);
	return *this;
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    IndexType GenMapIndex<OIType,Op,XSTYPE,Indices...>::type() const
    {
	return IndexType::MULTI;
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    GenMapIndex<OIType,Op,XSTYPE,Indices...>& GenMapIndex<OIType,Op,XSTYPE,Indices...>::operator=(size_t pos)
    {
        (*mOutIndex) = pos;
	IB::mPos = mOutIndex->pos();
	return *this;
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    GenMapIndex<OIType,Op,XSTYPE,Indices...>& GenMapIndex<OIType,Op,XSTYPE,Indices...>::operator++()
    {
	++(*mOutIndex);
        IB::mPos = mOutIndex->pos();
	return *this;
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    GenMapIndex<OIType,Op,XSTYPE,Indices...>& GenMapIndex<OIType,Op,XSTYPE,Indices...>::operator--()
    {
        --(*mOutIndex);
        IB::mPos = mOutIndex->pos();
	return *this;
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    int GenMapIndex<OIType,Op,XSTYPE,Indices...>::pp(std::intptr_t idxPtrNum)
    {
        mOutIndex->pp(idxPtrNum);
        IB::mPos = mOutIndex->pos();
	return 1;
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    int GenMapIndex<OIType,Op,XSTYPE,Indices...>::mm(std::intptr_t idxPtrNum)
    {
        mOutIndex->mm(idxPtrNum);
        IB::mPos = mOutIndex->pos();
	return 1;
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    std::string GenMapIndex<OIType,Op,XSTYPE,Indices...>::stringMeta() const
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr )->stringMeta(IB::mPos);
    }
    
    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    typename GenMapIndex<OIType,Op,XSTYPE,Indices...>::MetaType GenMapIndex<OIType,Op,XSTYPE,Indices...>::meta() const
    {
        return mOutIndex->meta();
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    GenMapIndex<OIType,Op,XSTYPE,Indices...>& GenMapIndex<OIType,Op,XSTYPE,Indices...>::at(const MetaType& metaPos)
    {
        mOutIndex->at(metaPos);
        IB::mPos = mOutIndex->pos();
	return *this;
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    size_t GenMapIndex<OIType,Op,XSTYPE,Indices...>::posAt(const MetaType& metaPos) const
    {
        return range()->outRange()->getMeta(metaPos);
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    size_t GenMapIndex<OIType,Op,XSTYPE,Indices...>::dim() const
    {
	return sizeof...(Indices);
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    bool GenMapIndex<OIType,Op,XSTYPE,Indices...>::first() const
    {
	return IB::mPos == 0;
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    bool GenMapIndex<OIType,Op,XSTYPE,Indices...>::last() const
    {
	return IB::mPos == IB::mMax - 1;
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    std::shared_ptr<typename GenMapIndex<OIType,Op,XSTYPE,Indices...>::RangeType>
    GenMapIndex<OIType,Op,XSTYPE,Indices...>::range() const
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr );
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    template <size_t N>
    auto GenMapIndex<OIType,Op,XSTYPE,Indices...>::getPtr() -> decltype( std::get<N>( mIPack ) )&
    {
	return std::get<N>(mIPack);
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>	
    size_t GenMapIndex<OIType,Op,XSTYPE,Indices...>::getStepSize(size_t n) const
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	return mBlockSizes[n+1];
    }
    
    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    template <class Exprs>
    auto GenMapIndex<OIType,Op,XSTYPE,Indices...>::ifor(size_t step, Exprs exs) const
    {
	return RPackNum<sizeof...(Indices)-1>::mkForh
	    (0, mIPack, mBlockSizes, OpExpr<Op,GenMapIndex<OIType,Op,XSTYPE,Indices...>,Exprs,XSTYPE>
	     ( range()->map(), this, step, exs ) );
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    template <class Exprs>
    auto GenMapIndex<OIType,Op,XSTYPE,Indices...>::pifor(size_t step, Exprs exs) const
    {
	return ifor(step, exs);
    }

    template <class OIType, class Op, SpaceType XSTYPE, class... Indices>
    template <class Exprs>
    auto GenMapIndex<OIType,Op,XSTYPE,Indices...>::iforh(size_t step, Exprs exs) const
    {
	return ifor(step, exs);
    }

    /*************************
     *   MapRangeFactory   *
     *************************/

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    GenMapRangeFactory<ORType,Op,XSTYPE,Ranges...>::GenMapRangeFactory(const std::shared_ptr<ORType>& outr,
                                                                const std::tuple<Op,MA>& mapf,
                                                                const std::shared_ptr<Ranges>&... rs)
    {
	mProd = std::shared_ptr< GenMapRange<ORType,Op,XSTYPE,Ranges...> >
            ( new GenMapRange<ORType,Op,XSTYPE,Ranges...>( outr, mapf, rs... ) );
    }
    
    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    GenMapRangeFactory<ORType,Op,XSTYPE,Ranges...>::GenMapRangeFactory(const std::shared_ptr<ORType>& outr,
                                                                const std::tuple<Op,MA>& mapf,
                                                                const typename GenMapRange<ORType,Op,XSTYPE,Ranges...>::Space& st)
    {
	mProd = std::shared_ptr< GenMapRange<ORType,Op,XSTYPE,Ranges...> >
            ( new GenMapRange<ORType,Op,XSTYPE,Ranges...>( outr, mapf, st ) );
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    GenMapRangeFactory<ORType,Op,XSTYPE,Ranges...>::GenMapRangeFactory(const std::tuple<Op,MA>& mapf,
                                                                const std::shared_ptr<Ranges>&... rs)
    {
	mProd = std::shared_ptr< GenMapRange<ORType,Op,XSTYPE,Ranges...> >
            ( new GenMapRange<ORType,Op,XSTYPE,Ranges...>( mapf, rs... ) );
    }
    
    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    GenMapRangeFactory<ORType,Op,XSTYPE,Ranges...>::GenMapRangeFactory(const std::tuple<Op,MA>& mapf,
                                                                const typename GenMapRange<ORType,Op,XSTYPE,Ranges...>::Space& st)
    {
	mProd = std::shared_ptr< GenMapRange<ORType,Op,XSTYPE,Ranges...> >
            ( new GenMapRange<ORType,Op,XSTYPE,Ranges...>( mapf, st ) );
    }
    
    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    std::shared_ptr<RangeBase> GenMapRangeFactory<ORType,Op,XSTYPE,Ranges...>::create()
    {
	mProd = checkIfCreated( std::dynamic_pointer_cast<oType>( mProd )->mSpace );
	setSelf();
	return mProd;
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    std::shared_ptr<RangeBase> GenMapRangeFactory<ORType,Op,XSTYPE,Ranges...>::checkIfCreated(const std::tuple<std::shared_ptr<Ranges>...>& ptp)
    {
	std::shared_ptr<RangeBase> out;
	bool check = false;
	for(auto& x: MapRangeFactoryProductMap::mAleadyCreated){
	    if(x.second.size() == sizeof...(Ranges)){
		check = RPackNum<sizeof...(Ranges)-1>::checkIfCreated(ptp, x.second);
		if(check){
		    out = x.first;
		    break;
		}
	    }
	}
	if(not check){
	    vector<std::intptr_t> pv(sizeof...(Ranges));
	    RPackNum<sizeof...(Ranges)-1>::RangesToVec(ptp, pv);
	    pv.push_back( reinterpret_cast<std::intptr_t>
			  ( &std::dynamic_pointer_cast<oType>( mProd )->mMapf ) );
	    MapRangeFactoryProductMap::mAleadyCreated[mProd] = pv;
	    out = mProd;
	}
	return out;
    }
    
    /******************
     *   MapRange   *
     ******************/

    template <SpaceType XSTYPE>
    struct OutRangeMaker
    {};

    template <>
    struct OutRangeMaker<SpaceType::ANY>
    {
        template <class MapF, class ORType>
        static void mk(std::shared_ptr<ORType>& outRange, MultiArray<size_t,ORType>& mapMult, const MapF& mapf)
        {
            std::map<typename MapF::value_type,size_t> mult;
            for(auto ii = mapf.begin(); ii.max() != ii.pos(); ++ii) {
                mult[mapf[ii]]++;
            }
	
            vector<typename MapF::value_type> outmeta(mult.size());
            vector<size_t> outmult(mult.size());

            size_t cnt = 0;
            for(auto& x: mult){
                outmeta[cnt] = x.first;
                outmult[cnt] = x.second;
                ++cnt;
            }

            typename ORType::FType orf(outmeta);
            outRange = std::dynamic_pointer_cast<ORType>( orf.create() );
            mapMult = MultiArray<size_t,ORType>( outRange, outmult );
        }
    };

    template <>
    struct OutRangeMaker<SpaceType::NONE>
    {
        template <class MapF, class ORType>
        static void mk(std::shared_ptr<ORType>& outRange, MultiArray<size_t,ORType>& mapMult, const MapF& mapf)
        {
            static_assert( std::is_same<size_t,typename MapF::value_type>::value,
                           "out range value type for NONE must be size_t" );
            size_t max = 0;
            for(auto ii = mapf.begin(); ii.max() != ii.pos(); ++ii) {
                max = mapf[ii]+1 > max ? mapf[ii]+1 : max;
            }
            vector<size_t> mult(max,0);
            for(auto ii = mapf.begin(); ii.max() != ii.pos(); ++ii) {
                mult[mapf[ii]]++;
            }
	
            vector<size_t> outmult(mult.size());

            size_t cnt = 0;
            for(auto& x: mult){
                outmult[cnt++] = x;
            }

            typename ORType::FType orf(max);
            outRange = std::dynamic_pointer_cast<ORType>( orf.create() );
            mapMult = MultiArray<size_t,ORType>( outRange, outmult );
        }
    };

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    void GenMapRange<ORType,Op,XSTYPE,Ranges...>::mkOutRange(const MA& mapf)
    {
	//FunctionalMultiArray<typename MapF::value_type,MapF,Ranges...> fma(mSpace, mMapf);
        OutRangeMaker<XSTYPE>::mk(mOutRange,mMapMult,mapf);
        auto i = mapf.begin();
        mMapPos.resize(i.max());
        for(; i.pos() != i.max(); ++i){
            mMapPos[i.pos()] = mOutRange->getMeta( mapf[i] );
        }

    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    GenMapRange<ORType,Op,XSTYPE,Ranges...>::GenMapRange(const std::shared_ptr<ORType>& outr, const std::tuple<Op,MA>& mapf,
                                                  const std::shared_ptr<Ranges>&... rs) :
        mSpace(std::make_tuple(rs...)),
        mMapf(std::get<0>(mapf)),
        mOutRange(outr),
        mMapMult(mOutRange,0),
        mMapPos(std::get<1>(mapf).size(),mOutRange->size())
    {
        auto& ma = std::get<1>(mapf);
        auto jj = mMapMult.begin();
        for(auto ii = ma.begin(); ii.pos() != ii.max(); ++ii){
            ++mMapMult[jj.at(ma[ii])];
            mMapPos[ii.pos()] = jj.pos();
        }
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    GenMapRange<ORType,Op,XSTYPE,Ranges...>::GenMapRange(const std::shared_ptr<ORType>& outr, const std::tuple<Op,MA>& mapf,
                                                  const Space& space) :
        mSpace(space),
        mMapf(std::get<0>(mapf)),
        mOutRange(outr),
        mMapMult(mOutRange,0),
        mMapPos(std::get<1>(mapf).size(),mOutRange->size())
    {
        auto& ma = std::get<1>(mapf);
        auto jj = mMapMult.begin();
        for(auto ii = ma.begin(); ii.pos() != ii.max(); ++ii){
            ++mMapMult[jj.at(ma[ii])];
            mMapPos[ii.pos()] = jj.pos();
        }
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    GenMapRange<ORType,Op,XSTYPE,Ranges...>::GenMapRange(const std::tuple<Op,MA>& mapf,
                                                  const std::shared_ptr<Ranges>&... rs) :
	mSpace(std::make_tuple(rs...)),
	mMapf(std::get<0>(mapf))
    {
	mkOutRange(std::get<1>(mapf));
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    GenMapRange<ORType,Op,XSTYPE,Ranges...>::GenMapRange(const std::tuple<Op,MA>& mapf, const Space& space) :
	mSpace( space ),
	mMapf(std::get<0>(mapf))
    {
	mkOutRange(std::get<1>(mapf));
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    template <size_t N>
    auto GenMapRange<ORType,Op,XSTYPE,Ranges...>::get() const -> decltype( *std::get<N>( mSpace ) )&
    {
	return *std::get<N>(mSpace);
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    template <size_t N>
    auto GenMapRange<ORType,Op,XSTYPE,Ranges...>::getPtr() const -> decltype( std::get<N>( mSpace ) )&
    {
	return std::get<N>(mSpace);
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    auto GenMapRange<ORType,Op,XSTYPE,Ranges...>::outRange() const -> std::shared_ptr<ORType>
    {
	return mOutRange;
    }
    
    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    const Op& GenMapRange<ORType,Op,XSTYPE,Ranges...>::map() const
    {
	return mMapf;
    }
    
    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    size_t GenMapRange<ORType,Op,XSTYPE,Ranges...>::dim() const
    {
	return sdim;
    }
    
    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    size_t GenMapRange<ORType,Op,XSTYPE,Ranges...>::size() const
    {
	return mOutRange->size();
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    SpaceType GenMapRange<ORType,Op,XSTYPE,Ranges...>::spaceType() const
    {
	return SpaceType::ANY;
    }
    
    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    const typename GenMapRange<ORType,Op,XSTYPE,Ranges...>::Space& GenMapRange<ORType,Op,XSTYPE,Ranges...>::space() const
    {
	return mSpace;
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    vector<size_t> GenMapRange<ORType,Op,XSTYPE,Ranges...>::typeNum() const
    {
        vector<size_t> o;
	RangeHelper::getTypeNum<sizeof...(Ranges)-1>(o,mSpace);
        return o;
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    size_t GenMapRange<ORType,Op,XSTYPE,Ranges...>::cmeta(char* target, size_t pos) const
    {
        //MetaType* xtarget = reinterpret_cast<MetaType*>(target);
        assert(0);
        return 0;
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    size_t GenMapRange<ORType,Op,XSTYPE,Ranges...>::cmetaSize() const
    {
        return RPackNum<sizeof...(Ranges)-1>::getCMetaSize(mSpace);
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    std::string GenMapRange<ORType,Op,XSTYPE,Ranges...>::stringMeta(size_t pos) const
    {
	auto i = begin();
	i = pos;
        return "[ " + RPackNum<sizeof...(Ranges)-1>::getStringMeta(i) + " ]";
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    vector<char> GenMapRange<ORType,Op,XSTYPE,Ranges...>::data() const
    {
	DataHeader h = dataHeader();
	vector<char> out;
	//out.reserve(h.metaSize + sizeof(DataHeader));
	char* hcp = reinterpret_cast<char*>(&h);
	out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
	RPackNum<sizeof...(Ranges)-1>::fillRangeDataVec(out, mSpace);
	return out;
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    DataHeader GenMapRange<ORType,Op,XSTYPE,Ranges...>::dataHeader() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::ANY );
	h.metaSize = sizeof...(Ranges);
	h.multiple = 1;
        return h;
    }
    
    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    typename GenMapRange<ORType,Op,XSTYPE,Ranges...>::IndexType GenMapRange<ORType,Op,XSTYPE,Ranges...>::begin() const
    {
	GenMapIndex<typename ORType::IndexType,Op,XSTYPE,typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<GenMapRange<ORType,Op,XSTYPE,Ranges...> >
	       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    typename GenMapRange<ORType,Op,XSTYPE,Ranges...>::IndexType GenMapRange<ORType,Op,XSTYPE,Ranges...>::end() const
    {
	GenMapIndex<typename ORType::IndexType,Op,XSTYPE,typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<GenMapRange<ORType,Op,XSTYPE,Ranges...> >
	       ( std::shared_ptr<RangeBase>( RB::mThis )) );
	i = size();
	return i;
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    auto GenMapRange<ORType,Op,XSTYPE,Ranges...>::mapMultiplicity() const
	-> const MultiArray<size_t,ORType>&
    {
	return mMapMult;
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    auto GenMapRange<ORType,Op,XSTYPE,Ranges...>::explMapMultiplicity() const
	-> ConstSlice<size_t,GenMapRange>
    {
        /*
        auto tmp = mMapMult;
	return tmp.format( std::dynamic_pointer_cast<GenMapRange<ORType,Op,XSTYPE,Ranges...> >
			   ( std::shared_ptr<RangeBase>( RB::mThis )) ); 
        */
        return mMapMult.slformat(std::dynamic_pointer_cast<GenMapRange<ORType,Op,XSTYPE,Ranges...> >
                                 ( std::shared_ptr<RangeBase>( RB::mThis )));
    }

    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    vector<size_t> GenMapRange<ORType,Op,XSTYPE,Ranges...>::mapPos() const
    {
        return mMapPos;
    }
    
    /*
    template <class ORType, class Op, SpaceType XSTYPE, class... Ranges>
    template <class... ERanges>
    auto GenMapRange<ORType,Op,XSTYPE,Ranges...>::cat(const std::shared_ptr<MapRange<ERanges...> >& erange)
	-> std::shared_ptr<GenMapRange<Ranges...,ERanges...> >
    {
	auto crange = std::tuple_cat(mSpace, erange->space());
	MapRangeFactory<Ranges...,ERanges...> rf(crange);
	return std::dynamic_pointer_cast<MapRange<Ranges...,ERanges...> >(rf.create());
    }
    */
}
