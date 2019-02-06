
#include "map_range.h"
//#include <type_traits>

namespace MultiArrayTools
{

    namespace
    {
	using namespace MultiArrayHelper;
    }

    /**************
     *   OpExpr   *
     **************/

    template <class MapF, class IndexPack, class Expr, SpaceType STYPE>
    OpExpr<MapF,IndexPack,Expr,STYPE>::OpExpr(const MapF& mapf, const IndexPack& ipack,
					const std::shared_ptr<OIType>& oind, size_t step, Expr ex) :
	mIndPtr(oind.get()), mSPos(mIndPtr->pos()), mMax(mIndPtr->max()),
	mStep(step), mExpr( std::forward<Expr>(ex) ),
	mOp(mkMapOp(mapf, ipack)),
	//mExt(ex.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr )))
	mExt( mOp.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr ) ).extend
	      ( ex.rootSteps( reinterpret_cast<std::intptr_t>( mIndPtr ) ) ) )
    {
	assert(mIndPtr != nullptr);
    }

    template <class MapF, class IndexPack, class Expr, SpaceType STYPE>
    inline void OpExpr<MapF,IndexPack,Expr,STYPE>::operator()(size_t mlast,
							 ExtType last) const
    {
	constexpr size_t NEXT = Op::SIZE;
	const ExtType nxpos = last;
	const size_t pos = mIndPtr->posAt( mOp.get( nxpos ) );
	//VCHECK(pos);
	const ExtType npos = last + mExt*pos;
	//VCHECK(npos.next().next().val());
	const size_t mnpos = PosForward<ForType::DEFAULT>::valuex(mlast, mStep, pos);
	mExpr(mnpos, Getter<NEXT>::template getX<ExtType>( npos ) );
    }

    template <class MapF, class IndexPack, class Expr, SpaceType STYPE>
    inline void OpExpr<MapF,IndexPack,Expr,STYPE>::operator()(size_t mlast) const
    {
	const ExtType last;
	constexpr size_t NEXT = Op::SIZE;
	const ExtType nxpos = last;
	const size_t pos = mIndPtr->posAt( mOp.get( nxpos ) );
	const ExtType npos = last + mExt*pos;
	const size_t mnpos = PosForward<ForType::DEFAULT>::valuex(mlast, mStep, pos);
	mExpr(mnpos, Getter<NEXT>::template getX<ExtType>( npos ));
    }
    
    template <class MapF, class IndexPack, class Expr, SpaceType STYPE>
    auto OpExpr<MapF,IndexPack,Expr,STYPE>::rootSteps(std::intptr_t iPtrNum) const
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
    
    template <class MapF, SpaceType XSTYPE, class... Indices>
    template <class MRange>
    GenMapIndex<MapF,XSTYPE,Indices...>::GenMapIndex(const std::shared_ptr<MRange>& range) :
	IndexInterface<GenMapIndex<MapF,XSTYPE,Indices...>,std::tuple<typename Indices::MetaType...> >(range, 0)
    {
	RPackNum<sizeof...(Indices)-1>::construct(mIPack, *range);
	IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack);
	std::get<sizeof...(Indices)>(mBlockSizes) = 1;
	RPackNum<sizeof...(Indices)-1>::initBlockSizes(mBlockSizes, mIPack); // has one more element!
	mOutIndex = std::make_shared<OIType>
	    ( std::dynamic_pointer_cast<RangeType>( IB::mRangePtr )->outRange()->begin() );
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    template <size_t DIR>
    GenMapIndex<MapF,XSTYPE,Indices...>& GenMapIndex<MapF,XSTYPE,Indices...>::up()
    {
	static_assert(DIR < sizeof...(Indices), "DIR exceeds number of sub-indices");
	IB::mPos += RPackNum<sizeof...(Indices)-DIR-1>::blockSize( mIPack );
	RPackNum<DIR>::pp( mIPack );
	return *this;
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    template <size_t DIR>
    GenMapIndex<MapF,XSTYPE,Indices...>& GenMapIndex<MapF,XSTYPE,Indices...>::down()
    {
	static_assert(DIR < sizeof...(Indices), "DIR exceeds number of sub-indices");
	IB::mPos -= RPackNum<sizeof...(Indices)-DIR-1>::blockSize( mIPack );
	RPackNum<DIR>::mm( mIPack );
	return *this;
    }
    
    template <class MapF, SpaceType XSTYPE, class... Indices>
    template <size_t N>
    auto GenMapIndex<MapF,XSTYPE,Indices...>::get() const -> decltype( *std::get<N>( mIPack ) )&
    {
	return *std::get<N>(mIPack);
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    template <size_t N>
    auto GenMapIndex<MapF,XSTYPE,Indices...>::getPtr() const -> decltype( std::get<N>( mIPack ) )&
    {
	return std::get<N>(mIPack);
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    auto GenMapIndex<MapF,XSTYPE,Indices...>::outIndex() const -> std::shared_ptr<OIType>
    {
	return mOutIndex;
    }
    
    template <class MapF, SpaceType XSTYPE, class... Indices>
    GenMapIndex<MapF,XSTYPE,Indices...>& GenMapIndex<MapF,XSTYPE,Indices...>::operator()(std::shared_ptr<Indices>&... indices)
    {
	RPackNum<sizeof...(Indices)-1>::swapIndices(mIPack, indices...);
	RPackNum<sizeof...(Indices)-1>::setIndexPack(mIPack, IB::mPos);
	return *this;
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    IndexType GenMapIndex<MapF,XSTYPE,Indices...>::type() const
    {
	return IndexType::MULTI;
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    GenMapIndex<MapF,XSTYPE,Indices...>& GenMapIndex<MapF,XSTYPE,Indices...>::operator=(size_t pos)
    {
	IB::mPos = pos;
	RPackNum<sizeof...(Indices)-1>::setIndexPack(mIPack, pos);
	return *this;
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    GenMapIndex<MapF,XSTYPE,Indices...>& GenMapIndex<MapF,XSTYPE,Indices...>::operator++()
    {
	RPackNum<sizeof...(Indices)-1>::pp( mIPack );
	++IB::mPos;
	return *this;
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    GenMapIndex<MapF,XSTYPE,Indices...>& GenMapIndex<MapF,XSTYPE,Indices...>::operator--()
    {
	RPackNum<sizeof...(Indices)-1>::mm( mIPack );
	--IB::mPos;
	return *this;
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    int GenMapIndex<MapF,XSTYPE,Indices...>::pp(std::intptr_t idxPtrNum)
    {
	int tmp = RPackNum<sizeof...(Indices)-1>::pp(mIPack, mBlockSizes, idxPtrNum);
	IB::mPos += tmp;
	return tmp;
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    int GenMapIndex<MapF,XSTYPE,Indices...>::mm(std::intptr_t idxPtrNum)
    {
	int tmp = RPackNum<sizeof...(Indices)-1>::mm(mIPack, mBlockSizes, idxPtrNum);
	IB::mPos -= tmp;
	return tmp;
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    std::string GenMapIndex<MapF,XSTYPE,Indices...>::stringMeta() const
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr )->stringMeta(IB::mPos);
    }
    
    template <class MapF, SpaceType XSTYPE, class... Indices>
    typename GenMapIndex<MapF,XSTYPE,Indices...>::MetaType GenMapIndex<MapF,XSTYPE,Indices...>::meta() const
    {
	MetaType metaTuple;
	RPackNum<sizeof...(Indices)-1>::getMetaPos(metaTuple, mIPack);
	return metaTuple;
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    GenMapIndex<MapF,XSTYPE,Indices...>& GenMapIndex<MapF,XSTYPE,Indices...>::at(const MetaType& metaPos)
    {
	RPackNum<sizeof...(Indices)-1>::setMeta(mIPack, metaPos);
	IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack);
	return *this;
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    size_t GenMapIndex<MapF,XSTYPE,Indices...>::dim() const
    {
	return sizeof...(Indices);
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    bool GenMapIndex<MapF,XSTYPE,Indices...>::first() const
    {
	return IB::mPos == 0;
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    bool GenMapIndex<MapF,XSTYPE,Indices...>::last() const
    {
	return IB::mPos == IB::mMax - 1;
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    std::shared_ptr<typename GenMapIndex<MapF,XSTYPE,Indices...>::RangeType>
    GenMapIndex<MapF,XSTYPE,Indices...>::range() const
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr );
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    template <size_t N>
    auto GenMapIndex<MapF,XSTYPE,Indices...>::getPtr() -> decltype( std::get<N>( mIPack ) )&
    {
	return std::get<N>(mIPack);
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>	
    size_t GenMapIndex<MapF,XSTYPE,Indices...>::getStepSize(size_t n) const
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	return mBlockSizes[n+1];
    }
    
    template <class MapF, SpaceType XSTYPE, class... Indices>
    std::string GenMapIndex<MapF,XSTYPE,Indices...>::id() const
    {
	return std::string("mul") + std::to_string(IB::mId);
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    void GenMapIndex<MapF,XSTYPE,Indices...>::print(size_t offset) const
    {
	if(offset == 0){
	    std::cout << " === " << std::endl;
	}
	for(size_t j = 0; j != offset; ++j) { std::cout << "\t"; }
	std::cout << id() << "[" << reinterpret_cast<std::intptr_t>(this)
		  << "]" << "(" << IB::mRangePtr << "): " << meta() << std::endl;
	RPackNum<sizeof...(Indices)-1>::printIndex(mIPack, offset+1);
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    template <class Exprs>
    auto GenMapIndex<MapF,XSTYPE,Indices...>::ifor(size_t step, Exprs exs) const
	-> decltype(RPackNum<sizeof...(Indices)-1>::mkForh
		    (step, mIPack, mBlockSizes, OpExpr<MapF,IndexPack,Exprs,XSTYPE>
		     ( range()->map(), mIPack, mOutIndex, step, exs ) ) )
    {
	return RPackNum<sizeof...(Indices)-1>::mkForh
	    (step, mIPack, mBlockSizes, OpExpr<MapF,IndexPack,Exprs,XSTYPE>
	     ( range()->map(), mIPack, mOutIndex, step, exs ) );
    }

    template <class MapF, SpaceType XSTYPE, class... Indices>
    template <class Exprs>
    auto GenMapIndex<MapF,XSTYPE,Indices...>::pifor(size_t step, Exprs exs) const
	-> decltype(ifor(step, exs))
    {
	return ifor(step, exs);
    }

    /*
    template <class MapF, SpaceType XSTYPE, class... Indices>
    template <class Exprs>
    auto GenMapIndex<MapF,XSTYPE,Indices...>::iforh(Exprs exs) const
	-> decltype(RPackNum<sizeof...(Indices)-1>::mkForh(mIPack, exs))
    {
	return RPackNum<sizeof...(Indices)-1>::mkForh(mIPack, exs);
    }
    */
    /*************************
     *   MapRangeFactory   *
     *************************/

    template <class MapF, SpaceType XSTYPE, class... Ranges>
    GenMapRangeFactory<MapF,XSTYPE,Ranges...>::GenMapRangeFactory(const MapF& mapf, const std::shared_ptr<Ranges>&... rs)
    {
	mProd = std::shared_ptr< GenMapRange<MapF,XSTYPE,Ranges...> >
            ( new GenMapRange<MapF,XSTYPE,Ranges...>( mapf, rs... ) );
    }
    
    template <class MapF, SpaceType XSTYPE, class... Ranges>
    GenMapRangeFactory<MapF,XSTYPE,Ranges...>::GenMapRangeFactory(const MapF& mapf, const typename GenMapRange<MapF,XSTYPE,Ranges...>::Space& st)
    {
	mProd = std::shared_ptr< GenMapRange<MapF,XSTYPE,Ranges...> >
            ( new GenMapRange<MapF,XSTYPE,Ranges...>( mapf, st ) );
    }
    
    template <class MapF, SpaceType XSTYPE, class... Ranges>
    std::shared_ptr<RangeBase> GenMapRangeFactory<MapF,XSTYPE,Ranges...>::create()
    {
	mProd = checkIfCreated( std::dynamic_pointer_cast<oType>( mProd )->mSpace );
	setSelf();
	return mProd;
    }

    template <class MapF, SpaceType XSTYPE, class... Ranges>
    std::shared_ptr<RangeBase> GenMapRangeFactory<MapF,XSTYPE,Ranges...>::checkIfCreated(const std::tuple<std::shared_ptr<Ranges>...>& ptp)
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
	    std::vector<std::intptr_t> pv(sizeof...(Ranges));
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
	
            std::vector<typename MapF::value_type> outmeta(mult.size());
            std::vector<size_t> outmult(mult.size());

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
            //static_assert( std::is_same<size_t,typename MapF::value_type>::value,
            //               "out range value type for NONE must be size_t" );
            size_t max = 0;
            for(auto ii = mapf.begin(); ii.max() != ii.pos(); ++ii) {
                max = mapf[ii] > max ? mapf[ii]+1 : max;
            }
            std::vector<size_t> mult(max,0);
            for(auto ii = mapf.begin(); ii.max() != ii.pos(); ++ii) {
                mult[mapf[ii]]++;
            }
	
            std::vector<size_t> outmult(mult.size());

            size_t cnt = 0;
            for(auto& x: mult){
                outmult[cnt++] = x;
            }

            typename ORType::FType orf(max);
            outRange = std::dynamic_pointer_cast<ORType>( orf.create() );
            mapMult = MultiArray<size_t,ORType>( outRange, outmult );
        }
    };

    template <class MapF, SpaceType XSTYPE, class... Ranges>
    void GenMapRange<MapF,XSTYPE,Ranges...>::mkOutRange()
    {
	//FunctionalMultiArray<typename MapF::value_type,MapF,Ranges...> fma(mSpace, mMapf);
        OutRangeMaker<XSTYPE>::mk(mOutRange,mMapMult,mMapf);
    }
    
    template <class MapF, SpaceType XSTYPE, class... Ranges>
    GenMapRange<MapF,XSTYPE,Ranges...>::GenMapRange(const MapF& mapf, const std::shared_ptr<Ranges>&... rs) :
	mSpace(std::make_tuple(rs...)),
	mMapf(mapf)
    {
	mkOutRange();
    }

    template <class MapF, SpaceType XSTYPE, class... Ranges>
    GenMapRange<MapF,XSTYPE,Ranges...>::GenMapRange(const MapF& mapf, const Space& space) :
	mSpace( space ),
	mMapf(mapf)
    {
	mkOutRange();
    }

    template <class MapF, SpaceType XSTYPE, class... Ranges>
    template <size_t N>
    auto GenMapRange<MapF,XSTYPE,Ranges...>::get() const -> decltype( *std::get<N>( mSpace ) )&
    {
	return *std::get<N>(mSpace);
    }

    template <class MapF, SpaceType XSTYPE, class... Ranges>
    template <size_t N>
    auto GenMapRange<MapF,XSTYPE,Ranges...>::getPtr() const -> decltype( std::get<N>( mSpace ) )&
    {
	return std::get<N>(mSpace);
    }

    template <class MapF, SpaceType XSTYPE, class... Ranges>
    auto GenMapRange<MapF,XSTYPE,Ranges...>::outRange() const -> std::shared_ptr<ORType>
    {
	return mOutRange;
    }

    template <class MapF, SpaceType XSTYPE, class... Ranges>
    const MapF& GenMapRange<MapF,XSTYPE,Ranges...>::map() const
    {
	return mMapf;
    }
    
    template <class MapF, SpaceType XSTYPE, class... Ranges>
    size_t GenMapRange<MapF,XSTYPE,Ranges...>::dim() const
    {
	return sdim;
    }
    
    template <class MapF, SpaceType XSTYPE, class... Ranges>
    size_t GenMapRange<MapF,XSTYPE,Ranges...>::size() const
    {
	return mOutRange->size();
	//return RPackNum<sizeof...(Ranges)-1>::getSize(mSpace);
    }

    template <class MapF, SpaceType XSTYPE, class... Ranges>
    SpaceType GenMapRange<MapF,XSTYPE,Ranges...>::spaceType() const
    {
	return SpaceType::ANY;
    }
    
    template <class MapF, SpaceType XSTYPE, class... Ranges>
    const typename GenMapRange<MapF,XSTYPE,Ranges...>::Space& GenMapRange<MapF,XSTYPE,Ranges...>::space() const
    {
	return mSpace;
    }

    template <class MapF, SpaceType XSTYPE, class... Ranges>
    std::string GenMapRange<MapF,XSTYPE,Ranges...>::stringMeta(size_t pos) const
    {
	auto i = begin();
	i = pos;
	return "[ " + RPackNum<sizeof...(Ranges)-1>::metaTupleToString(i.meta()) + " ]";
    }

    template <class MapF, SpaceType XSTYPE, class... Ranges>
    std::vector<char> GenMapRange<MapF,XSTYPE,Ranges...>::data() const
    {
	DataHeader h = dataHeader();
	std::vector<char> out;
	//out.reserve(h.metaSize + sizeof(DataHeader));
	char* hcp = reinterpret_cast<char*>(&h);
	out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
	RPackNum<sizeof...(Ranges)-1>::fillRangeDataVec(out, mSpace);
	return out;
    }

    template <class MapF, SpaceType XSTYPE, class... Ranges>
    DataHeader GenMapRange<MapF,XSTYPE,Ranges...>::dataHeader() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::ANY );
	h.metaSize = sizeof...(Ranges);
	h.multiple = 1;
        return h;
    }
    
    template <class MapF, SpaceType XSTYPE, class... Ranges>
    typename GenMapRange<MapF,XSTYPE,Ranges...>::IndexType GenMapRange<MapF,XSTYPE,Ranges...>::begin() const
    {
	GenMapIndex<MapF,XSTYPE,typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<GenMapRange<MapF,XSTYPE,Ranges...> >
	       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }

    template <class MapF, SpaceType XSTYPE, class... Ranges>
    typename GenMapRange<MapF,XSTYPE,Ranges...>::IndexType GenMapRange<MapF,XSTYPE,Ranges...>::end() const
    {
	GenMapIndex<MapF,XSTYPE,typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<GenMapRange<MapF,XSTYPE,Ranges...> >
	       ( std::shared_ptr<RangeBase>( RB::mThis )) );
	i = size();
	return i;
    }

    template <class MapF, SpaceType XSTYPE, class... Ranges>
    auto GenMapRange<MapF,XSTYPE,Ranges...>::mapMultiplicity() const
	-> const MultiArray<size_t,ORType>&
    {
	return mMapMult;
    }

    template <class MapF, SpaceType XSTYPE, class... Ranges>
    auto GenMapRange<MapF,XSTYPE,Ranges...>::explMapMultiplicity() const
	-> MultiArray<size_t,GenMapRange>
    {
	auto tmp = mMapMult;
	return tmp.format( std::dynamic_pointer_cast<GenMapRange<MapF,XSTYPE,Ranges...> >
			   ( std::shared_ptr<RangeBase>( RB::mThis )) ); 
    }

    /*
    template <class MapF, SpaceType XSTYPE, class... Ranges>
    template <class... ERanges>
    auto GenMapRange<MapF,XSTYPE,Ranges...>::cat(const std::shared_ptr<MapRange<ERanges...> >& erange)
	-> std::shared_ptr<GenMapRange<Ranges...,ERanges...> >
    {
	auto crange = std::tuple_cat(mSpace, erange->space());
	MapRangeFactory<Ranges...,ERanges...> rf(crange);
	return std::dynamic_pointer_cast<MapRange<Ranges...,ERanges...> >(rf.create());
    }
    */
}
