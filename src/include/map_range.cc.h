
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
    inline void OpExpr<Op,Index,Expr,STYPE>::operator()(size_t mlast,
                                                            ExtType last)
    {
	constexpr size_t NEXT = Op::SIZE;
	const ExtType nxpos = last;
	const size_t pos = mIndPtr->posAt( mOp.get( nxpos ) );
        if(pos != mIndPtr->max()){
            const ExtType npos = last + mExt*pos;
            const size_t mnpos = PosForward<ForType::DEFAULT>::valuex(mlast, mStep, pos);
            mExpr(mnpos, Getter<NEXT>::template getX<ExtType>( npos ) );
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
            mExpr(mnpos, Getter<NEXT>::template getX<ExtType>( npos ));
        }
    }
    
    template <class Op, class Index, class Expr, SpaceType STYPE>
    auto OpExpr<Op,Index,Expr,STYPE>::rootSteps(std::intptr_t iPtrNum) const
	-> ExtType
    {
	return mOp.rootSteps(iPtrNum).extend( mExpr.rootSteps(iPtrNum) );
	//return mExpr.rootSteps(iPtrNum).extend( mOp.rootSteps(iPtrNum) );
    }

    
    // -> define in range_base.cc
    //std::shared_ptr<RangeFactoryBase> mkMULTI(const char** dp);
    
    /******************
     *   MapIndex   *
     ******************/

    /*
    template <class MapF, class... Indices>
    MapIndex<MapF,Indices...>::MapIndex(const MapIndex<MapF,Indices...>& in) :
	IndexInterface<std::tuple<typename Indices::MetaType...> >(in)
    {
	RPackNum<sizeof...(Indices)-1>::copy(mIPack, in);
	IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack);
    }
	
    template <class MapF, class... Indices>
    MapIndex<MapF,Indices...>& MapIndex<MapF,Indices...>::operator=(const MapIndex<MapF,Indices...>& in)
    {
	IndexI::operator=(in);
	RPackNum<sizeof...(Indices)-1>::copy(mIPack, in);
	IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack);
	return *this;
    }
    */
    
    template <class Op, SpaceType XSTYPE, class... Indices>
    template <class MRange>
    GenMapIndex<Op,XSTYPE,Indices...>::GenMapIndex(const std::shared_ptr<MRange>& range) :
	IndexInterface<GenMapIndex<Op,XSTYPE,Indices...>,typename Op::value_type>(range, 0)
    {
	RPackNum<sizeof...(Indices)-1>::construct(mIPack, *range);
	IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack);
	std::get<sizeof...(Indices)>(mBlockSizes) = 1;
	RPackNum<sizeof...(Indices)-1>::initBlockSizes(mBlockSizes, mIPack); // has one more element!
	mOutIndex = std::make_shared<OIType>
	    ( std::dynamic_pointer_cast<RangeType>( IB::mRangePtr )->outRange()->begin() );
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    template <size_t DIR>
    GenMapIndex<Op,XSTYPE,Indices...>& GenMapIndex<Op,XSTYPE,Indices...>::up()
    {
	static_assert(DIR < sizeof...(Indices), "DIR exceeds number of sub-indices");
	IB::mPos += RPackNum<sizeof...(Indices)-DIR-1>::blockSize( mIPack );
	RPackNum<DIR>::pp( mIPack );
	return *this;
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    template <size_t DIR>
    GenMapIndex<Op,XSTYPE,Indices...>& GenMapIndex<Op,XSTYPE,Indices...>::down()
    {
	static_assert(DIR < sizeof...(Indices), "DIR exceeds number of sub-indices");
	IB::mPos -= RPackNum<sizeof...(Indices)-DIR-1>::blockSize( mIPack );
	RPackNum<DIR>::mm( mIPack );
	return *this;
    }
    
    template <class Op, SpaceType XSTYPE, class... Indices>
    template <size_t N>
    auto GenMapIndex<Op,XSTYPE,Indices...>::get() const -> decltype( *std::get<N>( mIPack ) )&
    {
	return *std::get<N>(mIPack);
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    template <size_t N>
    auto GenMapIndex<Op,XSTYPE,Indices...>::getPtr() const -> decltype( std::get<N>( mIPack ) )&
    {
	return std::get<N>(mIPack);
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    auto GenMapIndex<Op,XSTYPE,Indices...>::outIndex() const -> std::shared_ptr<OIType>
    {
	return mOutIndex;
    }
    
    template <class Op, SpaceType XSTYPE, class... Indices>
    GenMapIndex<Op,XSTYPE,Indices...>& GenMapIndex<Op,XSTYPE,Indices...>::operator()(const std::shared_ptr<Indices>&... indices)
    {
	RPackNum<sizeof...(Indices)-1>::swapIndices(mIPack, indices...);
	RPackNum<sizeof...(Indices)-1>::setIndexPack(mIPack, IB::mPos);
	return *this;
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    IndexType GenMapIndex<Op,XSTYPE,Indices...>::type() const
    {
	return IndexType::MULTI;
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    GenMapIndex<Op,XSTYPE,Indices...>& GenMapIndex<Op,XSTYPE,Indices...>::operator=(size_t pos)
    {
        (*mOutIndex) = pos;
	IB::mPos = mOutIndex->pos();
	//RPackNum<sizeof...(Indices)-1>::setIndexPack(mIPack, pos);
	return *this;
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    GenMapIndex<Op,XSTYPE,Indices...>& GenMapIndex<Op,XSTYPE,Indices...>::operator++()
    {
	//RPackNum<sizeof...(Indices)-1>::pp( mIPack );
	++(*mOutIndex);
        IB::mPos = mOutIndex->pos();
	return *this;
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    GenMapIndex<Op,XSTYPE,Indices...>& GenMapIndex<Op,XSTYPE,Indices...>::operator--()
    {
	//RPackNum<sizeof...(Indices)-1>::mm( mIPack );
        --(*mOutIndex);
        IB::mPos = mOutIndex->pos();
	return *this;
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    int GenMapIndex<Op,XSTYPE,Indices...>::pp(std::intptr_t idxPtrNum)
    {
	//int tmp = RPackNum<sizeof...(Indices)-1>::pp(mIPack, mBlockSizes, idxPtrNum);
        mOutIndex->pp(idxPtrNum);
        IB::mPos = mOutIndex->pos();
	//IB::mPos += tmp;
	return 1;
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    int GenMapIndex<Op,XSTYPE,Indices...>::mm(std::intptr_t idxPtrNum)
    {
	//int tmp = RPackNum<sizeof...(Indices)-1>::mm(mIPack, mBlockSizes, idxPtrNum);
        mOutIndex->mm(idxPtrNum);
        IB::mPos = mOutIndex->pos();
	//IB::mPos -= tmp;
	return 1;
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    std::string GenMapIndex<Op,XSTYPE,Indices...>::stringMeta() const
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr )->stringMeta(IB::mPos);
    }
    
    template <class Op, SpaceType XSTYPE, class... Indices>
    typename GenMapIndex<Op,XSTYPE,Indices...>::MetaType GenMapIndex<Op,XSTYPE,Indices...>::meta() const
    {
	//MetaType metaTuple;
	//RPackNum<sizeof...(Indices)-1>::getMetaPos(metaTuple, mIPack);
        //assert(0);
        return mOutIndex->meta();
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    GenMapIndex<Op,XSTYPE,Indices...>& GenMapIndex<Op,XSTYPE,Indices...>::at(const MetaType& metaPos)
    {
	//RPackNum<sizeof...(Indices)-1>::setMeta(mIPack, metaPos);
	//IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack);
        mOutIndex->at(metaPos);
        IB::mPos = mOutIndex->pos();
	return *this;
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    size_t GenMapIndex<Op,XSTYPE,Indices...>::posAt(const MetaType& metaPos) const
    {
        return range()->outRange()->getMeta(metaPos);
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    size_t GenMapIndex<Op,XSTYPE,Indices...>::dim() const
    {
	return sizeof...(Indices);
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    bool GenMapIndex<Op,XSTYPE,Indices...>::first() const
    {
	return IB::mPos == 0;
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    bool GenMapIndex<Op,XSTYPE,Indices...>::last() const
    {
	return IB::mPos == IB::mMax - 1;
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    std::shared_ptr<typename GenMapIndex<Op,XSTYPE,Indices...>::RangeType>
    GenMapIndex<Op,XSTYPE,Indices...>::range() const
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr );
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    template <size_t N>
    auto GenMapIndex<Op,XSTYPE,Indices...>::getPtr() -> decltype( std::get<N>( mIPack ) )&
    {
	return std::get<N>(mIPack);
    }

    template <class Op, SpaceType XSTYPE, class... Indices>	
    size_t GenMapIndex<Op,XSTYPE,Indices...>::getStepSize(size_t n) const
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	return mBlockSizes[n+1];
    }
    
    template <class Op, SpaceType XSTYPE, class... Indices>
    std::string GenMapIndex<Op,XSTYPE,Indices...>::id() const
    {
	return std::string("mul") + std::to_string(IB::mId);
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    void GenMapIndex<Op,XSTYPE,Indices...>::print(size_t offset) const
    {
	if(offset == 0){
	    std::cout << " === " << std::endl;
	}
	for(size_t j = 0; j != offset; ++j) { std::cout << "\t"; }
	std::cout << id() << "[" << reinterpret_cast<std::intptr_t>(this)
		  << "]" << "(" << IB::mRangePtr << "): " << meta() << std::endl;
	RPackNum<sizeof...(Indices)-1>::printIndex(mIPack, offset+1);
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    template <class Exprs>
    auto GenMapIndex<Op,XSTYPE,Indices...>::ifor(size_t step, Exprs exs) const
	-> decltype(RPackNum<sizeof...(Indices)-1>::mkForh
		    (step, mIPack, mBlockSizes, OpExpr<Op,GenMapIndex<Op,XSTYPE,Indices...>,Exprs,XSTYPE>
		     ( range()->map(), this, step, exs ) ) )
    {
	return RPackNum<sizeof...(Indices)-1>::mkForh
	    (0, mIPack, mBlockSizes, OpExpr<Op,GenMapIndex<Op,XSTYPE,Indices...>,Exprs,XSTYPE>
	     ( range()->map(), this, step, exs ) );
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    template <class Exprs>
    auto GenMapIndex<Op,XSTYPE,Indices...>::pifor(size_t step, Exprs exs) const
	-> decltype(ifor(step, exs))
    {
	return ifor(step, exs);
    }

    template <class Op, SpaceType XSTYPE, class... Indices>
    template <class Exprs>
    auto GenMapIndex<Op,XSTYPE,Indices...>::iforh(size_t step, Exprs exs) const
	-> decltype(ifor(step, exs))
    {
	return ifor(step, exs);
    }

    /*************************
     *   MapRangeFactory   *
     *************************/

    template <class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    GenMapRangeFactory<Op,XSTYPE,Ranges...>::GenMapRangeFactory(const std::shared_ptr<ORType>& outr,
                                                                const std::tuple<Op,MA>& mapf,
                                                                const std::shared_ptr<Ranges>&... rs)
    {
	mProd = std::shared_ptr< GenMapRange<Op,XSTYPE,Ranges...> >
            ( new GenMapRange<Op,XSTYPE,Ranges...>( outr, mapf, rs... ) );
    }
    
    template <class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    GenMapRangeFactory<Op,XSTYPE,Ranges...>::GenMapRangeFactory(const std::shared_ptr<ORType>& outr,
                                                                const std::tuple<Op,MA>& mapf,
                                                                const typename GenMapRange<Op,XSTYPE,Ranges...>::Space& st)
    {
	mProd = std::shared_ptr< GenMapRange<Op,XSTYPE,Ranges...> >
            ( new GenMapRange<Op,XSTYPE,Ranges...>( outr, mapf, st ) );
    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    GenMapRangeFactory<Op,XSTYPE,Ranges...>::GenMapRangeFactory(const std::tuple<Op,MA>& mapf,
                                                                const std::shared_ptr<Ranges>&... rs)
    {
	mProd = std::shared_ptr< GenMapRange<Op,XSTYPE,Ranges...> >
            ( new GenMapRange<Op,XSTYPE,Ranges...>( mapf, rs... ) );
    }
    
    template <class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    GenMapRangeFactory<Op,XSTYPE,Ranges...>::GenMapRangeFactory(const std::tuple<Op,MA>& mapf,
                                                                const typename GenMapRange<Op,XSTYPE,Ranges...>::Space& st)
    {
	mProd = std::shared_ptr< GenMapRange<Op,XSTYPE,Ranges...> >
            ( new GenMapRange<Op,XSTYPE,Ranges...>( mapf, st ) );
    }
    
    template <class Op, SpaceType XSTYPE, class... Ranges>
    std::shared_ptr<RangeBase> GenMapRangeFactory<Op,XSTYPE,Ranges...>::create()
    {
	mProd = checkIfCreated( std::dynamic_pointer_cast<oType>( mProd )->mSpace );
	setSelf();
	return mProd;
    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    std::shared_ptr<RangeBase> GenMapRangeFactory<Op,XSTYPE,Ranges...>::checkIfCreated(const std::tuple<std::shared_ptr<Ranges>...>& ptp)
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

    template <class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    void GenMapRange<Op,XSTYPE,Ranges...>::mkOutRange(const MA& mapf)
    {
	//FunctionalMultiArray<typename MapF::value_type,MapF,Ranges...> fma(mSpace, mMapf);
        OutRangeMaker<XSTYPE>::mk(mOutRange,mMapMult,mapf);
        auto i = mapf.begin();
        mMapPos.resize(i.max());
        for(; i.pos() != i.max(); ++i){
            mMapPos[i.pos()] = mOutRange->getMeta( mapf[i] );
        }

    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    GenMapRange<Op,XSTYPE,Ranges...>::GenMapRange(const std::shared_ptr<ORType>& outr, const std::tuple<Op,MA>& mapf,
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

    template <class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    GenMapRange<Op,XSTYPE,Ranges...>::GenMapRange(const std::shared_ptr<ORType>& outr, const std::tuple<Op,MA>& mapf,
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

    template <class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    GenMapRange<Op,XSTYPE,Ranges...>::GenMapRange(const std::tuple<Op,MA>& mapf,
                                                  const std::shared_ptr<Ranges>&... rs) :
	mSpace(std::make_tuple(rs...)),
	mMapf(std::get<0>(mapf))
    {
	mkOutRange(std::get<1>(mapf));
    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    template <class MA>
    GenMapRange<Op,XSTYPE,Ranges...>::GenMapRange(const std::tuple<Op,MA>& mapf, const Space& space) :
	mSpace( space ),
	mMapf(std::get<0>(mapf))
    {
	mkOutRange(std::get<1>(mapf));
    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    template <size_t N>
    auto GenMapRange<Op,XSTYPE,Ranges...>::get() const -> decltype( *std::get<N>( mSpace ) )&
    {
	return *std::get<N>(mSpace);
    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    template <size_t N>
    auto GenMapRange<Op,XSTYPE,Ranges...>::getPtr() const -> decltype( std::get<N>( mSpace ) )&
    {
	return std::get<N>(mSpace);
    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    auto GenMapRange<Op,XSTYPE,Ranges...>::outRange() const -> std::shared_ptr<ORType>
    {
	return mOutRange;
    }
    
    template <class Op, SpaceType XSTYPE, class... Ranges>
    const Op& GenMapRange<Op,XSTYPE,Ranges...>::map() const
    {
	return mMapf;
    }
    
    template <class Op, SpaceType XSTYPE, class... Ranges>
    size_t GenMapRange<Op,XSTYPE,Ranges...>::dim() const
    {
	return sdim;
    }
    
    template <class Op, SpaceType XSTYPE, class... Ranges>
    size_t GenMapRange<Op,XSTYPE,Ranges...>::size() const
    {
	return mOutRange->size();
	//return RPackNum<sizeof...(Ranges)-1>::getSize(mSpace);
    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    SpaceType GenMapRange<Op,XSTYPE,Ranges...>::spaceType() const
    {
	return SpaceType::ANY;
    }
    
    template <class Op, SpaceType XSTYPE, class... Ranges>
    const typename GenMapRange<Op,XSTYPE,Ranges...>::Space& GenMapRange<Op,XSTYPE,Ranges...>::space() const
    {
	return mSpace;
    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    vector<size_t> GenMapRange<Op,XSTYPE,Ranges...>::typeNum() const
    {
        vector<size_t> o;
        RPackNum<sizeof...(Ranges)-1>::getTypeNum(o,mSpace);
        return o;
    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    size_t GenMapRange<Op,XSTYPE,Ranges...>::cmeta(char* target, size_t pos) const
    {
        return RPackNum<sizeof...(Ranges)-1>::getCMeta(target,pos,mSpace,cmetaSize());
    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    size_t GenMapRange<Op,XSTYPE,Ranges...>::cmetaSize() const
    {
        return RPackNum<sizeof...(Ranges)-1>::getCMetaSize(mSpace);
    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    std::string GenMapRange<Op,XSTYPE,Ranges...>::stringMeta(size_t pos) const
    {
	auto i = begin();
	i = pos;
        return "[ " + RPackNum<sizeof...(Ranges)-1>::getStringMeta(i) + " ]";
    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    vector<char> GenMapRange<Op,XSTYPE,Ranges...>::data() const
    {
	DataHeader h = dataHeader();
	vector<char> out;
	//out.reserve(h.metaSize + sizeof(DataHeader));
	char* hcp = reinterpret_cast<char*>(&h);
	out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
	RPackNum<sizeof...(Ranges)-1>::fillRangeDataVec(out, mSpace);
	return out;
    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    DataHeader GenMapRange<Op,XSTYPE,Ranges...>::dataHeader() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::ANY );
	h.metaSize = sizeof...(Ranges);
	h.multiple = 1;
        return h;
    }
    
    template <class Op, SpaceType XSTYPE, class... Ranges>
    typename GenMapRange<Op,XSTYPE,Ranges...>::IndexType GenMapRange<Op,XSTYPE,Ranges...>::begin() const
    {
	GenMapIndex<Op,XSTYPE,typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<GenMapRange<Op,XSTYPE,Ranges...> >
	       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    typename GenMapRange<Op,XSTYPE,Ranges...>::IndexType GenMapRange<Op,XSTYPE,Ranges...>::end() const
    {
	GenMapIndex<Op,XSTYPE,typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<GenMapRange<Op,XSTYPE,Ranges...> >
	       ( std::shared_ptr<RangeBase>( RB::mThis )) );
	i = size();
	return i;
    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    auto GenMapRange<Op,XSTYPE,Ranges...>::mapMultiplicity() const
	-> const MultiArray<size_t,ORType>&
    {
	return mMapMult;
    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    auto GenMapRange<Op,XSTYPE,Ranges...>::explMapMultiplicity() const
	-> ConstSlice<size_t,GenMapRange>
    {
        /*
        auto tmp = mMapMult;
	return tmp.format( std::dynamic_pointer_cast<GenMapRange<Op,XSTYPE,Ranges...> >
			   ( std::shared_ptr<RangeBase>( RB::mThis )) ); 
        */
        return mMapMult.slformat(std::dynamic_pointer_cast<GenMapRange<Op,XSTYPE,Ranges...> >
                                 ( std::shared_ptr<RangeBase>( RB::mThis )));
    }

    template <class Op, SpaceType XSTYPE, class... Ranges>
    vector<size_t> GenMapRange<Op,XSTYPE,Ranges...>::mapPos() const
    {
        return mMapPos;
    }
    
    /*
    template <class Op, SpaceType XSTYPE, class... Ranges>
    template <class... ERanges>
    auto GenMapRange<Op,XSTYPE,Ranges...>::cat(const std::shared_ptr<MapRange<ERanges...> >& erange)
	-> std::shared_ptr<GenMapRange<Ranges...,ERanges...> >
    {
	auto crange = std::tuple_cat(mSpace, erange->space());
	MapRangeFactory<Ranges...,ERanges...> rf(crange);
	return std::dynamic_pointer_cast<MapRange<Ranges...,ERanges...> >(rf.create());
    }
    */
}
