
#include "map_range.h"

namespace MultiArrayTools
{

    namespace
    {
	using namespace MultiArrayHelper;
    }

    /**************
     *   OpExpr   *
     **************/

    template <class MapF, class IndexPack, class Expr>
    OpExpr<MapF,IndexPack,Expr>::OpExpr(const MapF& mapf, const IndexPack& ipack,
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

    template <class MapF, class IndexPack, class Expr>
    inline void OpExpr<MapF,IndexPack,Expr>::operator()(size_t mlast,
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

    template <class MapF, class IndexPack, class Expr>
    inline void OpExpr<MapF,IndexPack,Expr>::operator()(size_t mlast) const
    {
	const ExtType last;
	constexpr size_t NEXT = Op::SIZE;
	const ExtType nxpos = last;
	const size_t pos = mIndPtr->posAt( mOp.get( nxpos ) );
	const ExtType npos = last + mExt*pos;
	const size_t mnpos = PosForward<ForType::DEFAULT>::valuex(mlast, mStep, pos);
	mExpr(mnpos, Getter<NEXT>::template getX<ExtType>( npos ));
    }
    
    template <class MapF, class IndexPack, class Expr>
    auto OpExpr<MapF,IndexPack,Expr>::rootSteps(std::intptr_t iPtrNum) const
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
    
    template <class MapF, class... Indices>
    template <class MRange>
    MapIndex<MapF,Indices...>::MapIndex(const std::shared_ptr<MRange>& range) :
	IndexInterface<MapIndex<MapF,Indices...>,std::tuple<typename Indices::MetaType...> >(range, 0)
    {
	RPackNum<sizeof...(Indices)-1>::construct(mIPack, *range);
	IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack);
	std::get<sizeof...(Indices)>(mBlockSizes) = 1;
	RPackNum<sizeof...(Indices)-1>::initBlockSizes(mBlockSizes, mIPack); // has one more element!
	mOutIndex = std::make_shared<OIType>
	    ( std::dynamic_pointer_cast<RangeType>( IB::mRangePtr )->outRange()->begin() );
    }

    template <class MapF, class... Indices>
    template <size_t DIR>
    MapIndex<MapF,Indices...>& MapIndex<MapF,Indices...>::up()
    {
	static_assert(DIR < sizeof...(Indices), "DIR exceeds number of sub-indices");
	IB::mPos += RPackNum<sizeof...(Indices)-DIR-1>::blockSize( mIPack );
	RPackNum<DIR>::pp( mIPack );
	return *this;
    }

    template <class MapF, class... Indices>
    template <size_t DIR>
    MapIndex<MapF,Indices...>& MapIndex<MapF,Indices...>::down()
    {
	static_assert(DIR < sizeof...(Indices), "DIR exceeds number of sub-indices");
	IB::mPos -= RPackNum<sizeof...(Indices)-DIR-1>::blockSize( mIPack );
	RPackNum<DIR>::mm( mIPack );
	return *this;
    }
    
    template <class MapF, class... Indices>
    template <size_t N>
    auto MapIndex<MapF,Indices...>::get() const -> decltype( *std::get<N>( mIPack ) )&
    {
	return *std::get<N>(mIPack);
    }

    template <class MapF, class... Indices>
    template <size_t N>
    auto MapIndex<MapF,Indices...>::getPtr() const -> decltype( std::get<N>( mIPack ) )&
    {
	return std::get<N>(mIPack);
    }

    template <class MapF, class... Indices>
    auto MapIndex<MapF,Indices...>::outIndex() const -> std::shared_ptr<OIType>
    {
	return mOutIndex;
    }
    
    template <class MapF, class... Indices>
    MapIndex<MapF,Indices...>& MapIndex<MapF,Indices...>::operator()(std::shared_ptr<Indices>&... indices)
    {
	RPackNum<sizeof...(Indices)-1>::swapIndices(mIPack, indices...);
	RPackNum<sizeof...(Indices)-1>::setIndexPack(mIPack, IB::mPos);
	return *this;
    }

    template <class MapF, class... Indices>
    IndexType MapIndex<MapF,Indices...>::type() const
    {
	return IndexType::MULTI;
    }

    template <class MapF, class... Indices>
    MapIndex<MapF,Indices...>& MapIndex<MapF,Indices...>::operator=(size_t pos)
    {
	IB::mPos = pos;
	RPackNum<sizeof...(Indices)-1>::setIndexPack(mIPack, pos);
	return *this;
    }

    template <class MapF, class... Indices>
    MapIndex<MapF,Indices...>& MapIndex<MapF,Indices...>::operator++()
    {
	RPackNum<sizeof...(Indices)-1>::pp( mIPack );
	++IB::mPos;
	return *this;
    }

    template <class MapF, class... Indices>
    MapIndex<MapF,Indices...>& MapIndex<MapF,Indices...>::operator--()
    {
	RPackNum<sizeof...(Indices)-1>::mm( mIPack );
	--IB::mPos;
	return *this;
    }

    template <class MapF, class... Indices>
    int MapIndex<MapF,Indices...>::pp(std::intptr_t idxPtrNum)
    {
	int tmp = RPackNum<sizeof...(Indices)-1>::pp(mIPack, mBlockSizes, idxPtrNum);
	IB::mPos += tmp;
	return tmp;
    }

    template <class MapF, class... Indices>
    int MapIndex<MapF,Indices...>::mm(std::intptr_t idxPtrNum)
    {
	int tmp = RPackNum<sizeof...(Indices)-1>::mm(mIPack, mBlockSizes, idxPtrNum);
	IB::mPos -= tmp;
	return tmp;
    }

    template <class MapF, class... Indices>
    std::string MapIndex<MapF,Indices...>::stringMeta() const
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr )->stringMeta(IB::mPos);
    }
    
    template <class MapF, class... Indices>
    typename MapIndex<MapF,Indices...>::MetaType MapIndex<MapF,Indices...>::meta() const
    {
	MetaType metaTuple;
	RPackNum<sizeof...(Indices)-1>::getMetaPos(metaTuple, mIPack);
	return metaTuple;
    }

    template <class MapF, class... Indices>
    MapIndex<MapF,Indices...>& MapIndex<MapF,Indices...>::at(const MetaType& metaPos)
    {
	RPackNum<sizeof...(Indices)-1>::setMeta(mIPack, metaPos);
	IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack);
	return *this;
    }

    template <class MapF, class... Indices>
    size_t MapIndex<MapF,Indices...>::dim() const
    {
	return sizeof...(Indices);
    }

    template <class MapF, class... Indices>
    bool MapIndex<MapF,Indices...>::first() const
    {
	return IB::mPos == 0;
    }

    template <class MapF, class... Indices>
    bool MapIndex<MapF,Indices...>::last() const
    {
	return IB::mPos == IB::mMax - 1;
    }

    template <class MapF, class... Indices>
    std::shared_ptr<typename MapIndex<MapF,Indices...>::RangeType>
    MapIndex<MapF,Indices...>::range() const
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr );
    }

    template <class MapF, class... Indices>
    template <size_t N>
    auto MapIndex<MapF,Indices...>::getPtr() -> decltype( std::get<N>( mIPack ) )&
    {
	return std::get<N>(mIPack);
    }

    template <class MapF, class... Indices>	
    size_t MapIndex<MapF,Indices...>::getStepSize(size_t n) const
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	return mBlockSizes[n+1];
    }
    
    template <class MapF, class... Indices>
    std::string MapIndex<MapF,Indices...>::id() const
    {
	return std::string("mul") + std::to_string(IB::mId);
    }

    template <class MapF, class... Indices>
    void MapIndex<MapF,Indices...>::print(size_t offset) const
    {
	if(offset == 0){
	    std::cout << " === " << std::endl;
	}
	for(size_t j = 0; j != offset; ++j) { std::cout << "\t"; }
	std::cout << id() << "[" << reinterpret_cast<std::intptr_t>(this)
		  << "]" << "(" << IB::mRangePtr << "): " << meta() << std::endl;
	RPackNum<sizeof...(Indices)-1>::printIndex(mIPack, offset+1);
    }

    template <class MapF, class... Indices>
    template <class Exprs>
    auto MapIndex<MapF,Indices...>::ifor(size_t step, Exprs exs) const
	-> decltype(RPackNum<sizeof...(Indices)-1>::mkForh
		    (step, mIPack, mBlockSizes, OpExpr<MapF,IndexPack,Exprs>
		     ( range()->map(), mIPack, mOutIndex, step, exs ) ) )
    {
	return RPackNum<sizeof...(Indices)-1>::mkForh
	    (step, mIPack, mBlockSizes, OpExpr<MapF,IndexPack,Exprs>
	     ( range()->map(), mIPack, mOutIndex, step, exs ) );
    }
    /*
    template <class MapF, class... Indices>
    template <class Exprs>
    auto MapIndex<MapF,Indices...>::iforh(Exprs exs) const
	-> decltype(RPackNum<sizeof...(Indices)-1>::mkForh(mIPack, exs))
    {
	return RPackNum<sizeof...(Indices)-1>::mkForh(mIPack, exs);
    }
    */
    /*************************
     *   MapRangeFactory   *
     *************************/

    template <class MapF, class... Ranges>
    MapRangeFactory<MapF,Ranges...>::MapRangeFactory(const MapF& mapf, const std::shared_ptr<Ranges>&... rs)
    {
	mProd = std::shared_ptr< MapRange<MapF,Ranges...> >( new MapRange<MapF,Ranges...>( mapf, rs... ) );
    }
    
    template <class MapF, class... Ranges>
    MapRangeFactory<MapF,Ranges...>::MapRangeFactory(const MapF& mapf, const typename MapRange<MapF,Ranges...>::Space& st)
    {
	mProd = std::shared_ptr< MapRange<MapF,Ranges...> >( new MapRange<MapF,Ranges...>( mapf, st ) );
    }
    
    template <class MapF, class... Ranges>
    std::shared_ptr<RangeBase> MapRangeFactory<MapF,Ranges...>::create()
    {
	mProd = checkIfCreated( std::dynamic_pointer_cast<oType>( mProd )->mSpace );
	setSelf();
	return mProd;
    }

    template <class MapF, class... Ranges>
    std::shared_ptr<RangeBase> MapRangeFactory<MapF,Ranges...>::checkIfCreated(const std::tuple<std::shared_ptr<Ranges>...>& ptp)
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

    template <class MapF, class... Ranges>
    void MapRange<MapF,Ranges...>::mkOutRange()
    {
	//FunctionalMultiArray<typename MapF::value_type,MapF,Ranges...> fma(mSpace, mMapf);
	std::map<typename MapF::value_type,size_t> mult;
	for(auto ii = mMapf.begin(); ii.max() != ii.pos(); ++ii) {
	    mult[mMapf[ii]]++;
	}
	
	std::vector<typename MapF::value_type> outmeta(mult.size());
	std::vector<size_t> outmult(mult.size());

	size_t cnt = 0;
	for(auto& x: mult){
	    outmeta[cnt] = x.first;
	    outmult[cnt] = x.second;
	    ++cnt;
	}

	ORFType orf(outmeta);
	mOutRange = std::dynamic_pointer_cast<ORType>( orf.create() );
	mMapMult = MultiArray<size_t,ORType>( mOutRange, outmult );
    }
    
    template <class MapF, class... Ranges>
    MapRange<MapF,Ranges...>::MapRange(const MapF& mapf, const std::shared_ptr<Ranges>&... rs) :
	mSpace(std::make_tuple(rs...)),
	mMapf(mapf)
    {
	mkOutRange();
    }

    template <class MapF, class... Ranges>
    MapRange<MapF,Ranges...>::MapRange(const MapF& mapf, const Space& space) :
	mSpace( space ),
	mMapf(mapf)
    {
	mkOutRange();
    }

    template <class MapF, class... Ranges>
    template <size_t N>
    auto MapRange<MapF,Ranges...>::get() const -> decltype( *std::get<N>( mSpace ) )&
    {
	return *std::get<N>(mSpace);
    }

    template <class MapF, class... Ranges>
    template <size_t N>
    auto MapRange<MapF,Ranges...>::getPtr() const -> decltype( std::get<N>( mSpace ) )&
    {
	return std::get<N>(mSpace);
    }

    template <class MapF, class... Ranges>
    auto MapRange<MapF,Ranges...>::outRange() const -> std::shared_ptr<ORType>
    {
	return mOutRange;
    }

    template <class MapF, class... Ranges>
    const MapF& MapRange<MapF,Ranges...>::map() const
    {
	return mMapf;
    }
    
    template <class MapF, class... Ranges>
    size_t MapRange<MapF,Ranges...>::dim() const
    {
	return sdim;
    }
    
    template <class MapF, class... Ranges>
    size_t MapRange<MapF,Ranges...>::size() const
    {
	return mOutRange->size();
	//return RPackNum<sizeof...(Ranges)-1>::getSize(mSpace);
    }

    template <class MapF, class... Ranges>
    SpaceType MapRange<MapF,Ranges...>::spaceType() const
    {
	return SpaceType::ANY;
    }
    
    template <class MapF, class... Ranges>
    const typename MapRange<MapF,Ranges...>::Space& MapRange<MapF,Ranges...>::space() const
    {
	return mSpace;
    }

    template <class MapF, class... Ranges>
    std::string MapRange<MapF,Ranges...>::stringMeta(size_t pos) const
    {
	auto i = begin();
	i = pos;
	return "[ " + RPackNum<sizeof...(Ranges)-1>::metaTupleToString(i.meta()) + " ]";
    }

    template <class MapF, class... Ranges>
    std::vector<char> MapRange<MapF,Ranges...>::data() const
    {
	DataHeader h;
	h.spaceType = static_cast<int>( SpaceType::ANY );
	h.metaSize = sizeof...(Ranges);
	h.multiple = 1;
	std::vector<char> out;
	//out.reserve(h.metaSize + sizeof(DataHeader));
	char* hcp = reinterpret_cast<char*>(&h);
	out.insert(out.end(), hcp, hcp + sizeof(DataHeader));
	RPackNum<sizeof...(Ranges)-1>::fillRangeDataVec(out, mSpace);
	return out;
    }
    
    template <class MapF, class... Ranges>
    typename MapRange<MapF,Ranges...>::IndexType MapRange<MapF,Ranges...>::begin() const
    {
	MapIndex<MapF,typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<MapRange<MapF,Ranges...> >
	       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }

    template <class MapF, class... Ranges>
    typename MapRange<MapF,Ranges...>::IndexType MapRange<MapF,Ranges...>::end() const
    {
	MapIndex<MapF,typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<MapRange<MapF,Ranges...> >
	       ( std::shared_ptr<RangeBase>( RB::mThis )) );
	i = size();
	return i;
    }

    template <class MapF, class... Ranges>
    auto MapRange<MapF,Ranges...>::mapMultiplicity() const
	-> const MultiArray<size_t,ORType>&
    {
	return mMapMult;
    }

    template <class MapF, class... Ranges>
    auto MapRange<MapF,Ranges...>::explMapMultiplicity() const
	-> MultiArray<size_t,MapRange>
    {
	auto tmp = mMapMult;
	return tmp.format( std::dynamic_pointer_cast<MapRange<MapF,Ranges...> >
			   ( std::shared_ptr<RangeBase>( RB::mThis )) ); 
    }
    
    template <class MapF, class... Ranges>
    template <class... ERanges>
    auto MapRange<MapF,Ranges...>::cat(const std::shared_ptr<MapRange<ERanges...> >& erange)
	-> std::shared_ptr<MapRange<Ranges...,ERanges...> >
    {
	auto crange = std::tuple_cat(mSpace, erange->space());
	MapRangeFactory<Ranges...,ERanges...> rf(crange);
	return std::dynamic_pointer_cast<MapRange<Ranges...,ERanges...> >(rf.create());
    }

}
