
#ifndef __range_helper_h__
#define __range_helper_h__

namespace CNORXZ
{
    namespace RangeHelper
    {

	template <size_t I, class... Indices>
	int ppx(std::tuple<std::shared_ptr<Indices>...>& ip,
		std::array<size_t,sizeof...(Indices)+1>& bs,
		std::intptr_t idxPtrNum)
	{
	    auto& siPtr = std::get<I>(ip);
	    if(reinterpret_cast<std::intptr_t>(siPtr.get()) == idxPtrNum){
		if constexpr(I != 0){
		    return ppx<I-1>(ip, bs, idxPtrNum);
		}
		else {
		    return std::get<0>(bs);
		}
	    }
	    else {
		const int tmp = siPtr->pp(idxPtrNum);
		if constexpr(I != 0){
		    if(siPtr->pos() == siPtr->max()){
			(*siPtr) = 0;
			return ppx<I-1>(ip, bs, idxPtrNum) - siPtr->max() + 1;
		    }
		}
		return tmp * std::get<I+1>(bs);
	    }
	}

	template <size_t I, class... Indices>
	int mmx(std::tuple<std::shared_ptr<Indices>...>& ip,
		std::array<size_t,sizeof...(Indices)+1>& bs,
		std::intptr_t idxPtrNum)
	{
	    auto& siPtr = std::get<I>(ip);
	    if(reinterpret_cast<std::intptr_t>(siPtr.get()) == idxPtrNum){
		if constexpr(I != 0){
		    return mmx<I-1>(ip, bs, idxPtrNum);
		}
		else {
		    return std::get<0>(bs);
		}
	    }
	    else {
		const int tmp = siPtr->mm(idxPtrNum);
		if constexpr(I != 0){
		    if(siPtr->pos() == siPtr->max()){
			(*siPtr) = siPtr->max() - 1;
			return mmx<I-1>(ip, bs, idxPtrNum) - siPtr->max() + 1;
		    }
		}
		return tmp * std::get<I+1>(bs);
	    }
	}

	template <size_t N, class... Indices>
	inline size_t makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup,
			      const std::array<size_t,sizeof...(Indices)+1>& blockSize)
	{
	    if constexpr(N != 0){
		return makePos<N-1>(iPtrTup, blockSize) +
		    std::get<N>(iPtrTup)->pos() * std::get<N+1>(blockSize);
	    }
	    else {
		return std::get<0>(iPtrTup)->pos() * std::get<1>(blockSize);
	    }
	}

	template <class Range>
	inline void resolveSetRange(std::shared_ptr<Range>& rp, const vector<std::shared_ptr<RangeBase> >& orig,
				    size_t origpos, size_t size)
	{
	    assert(size == 1);
	    rp = std::dynamic_pointer_cast<Range>( orig[origpos] ); // catch bad cast here!!
	}

	template <class Range>
	inline void setRangeToVec(vector<std::shared_ptr<RangeBase> >& v,
				  std::shared_ptr<Range> r)
	{
	    v.insert(v.begin(), r);
	}

	template <size_t N, class IndexPack>
	void setIndexPack(IndexPack& iPack, size_t pos)
	{
	    auto& i = *std::get<N>(iPack).get();
	    const size_t ownPos = pos % i.max();
	    i = ownPos;
	    if constexpr(N != 0){
		setIndexPack<N-1>(iPack, (pos - ownPos) / i.max() );
	    }
	}

	template <size_t N, class... Ranges>
	inline size_t getMeta(const std::tuple<std::shared_ptr<Ranges>...>& space,
			      const std::tuple<typename Ranges::IndexType::MetaType...>& meta)
	{
	    if constexpr(N != 0){
		return getMeta<N-1>(space,meta) * std::get<N>(space)->size() +
		    std::get<N>(space)->getMeta(std::get<N>(meta));
	    }
	    else {
		return std::get<0>(space)->getMeta(std::get<0>(meta));
	    }
	}

	template <size_t N, class... Ranges>
	inline void getTypeNum(vector<size_t>& res, const std::tuple<std::shared_ptr<Ranges>...>& stp)
	{
	    auto& r = *std::get<N>(stp);
	    auto tn = r.typeNum();
	    res.insert(res.begin(), tn.begin(), tn.end());
	    if constexpr(N != 0){
		getTypeNum<N-1>(res, stp);
	    }
	}

	template <size_t N, class IndexPack, class BlockArray, class Exprs>
	auto mkFor(size_t step, const IndexPack& ipack, const BlockArray& ba, Exprs exs)
	{
	    constexpr size_t S = std::tuple_size<IndexPack>::value;
	    if constexpr(N < S-1){
		return std::get<N>(ipack)
		    ->ifor( step*std::get<N+1>(ba), mkFor<N+1>(step, ipack, ba, exs) );
	    }
	    else {
		return std::get<N>(ipack)->ifor( step*std::get<N+1>(ba), exs);
	    }
	}

	template <size_t N, class IndexPack, class BlockArray, class Exprs>
	auto mkForh(size_t step, const IndexPack& ipack, const BlockArray& ba, Exprs exs)
	{
	    constexpr size_t S = std::tuple_size<IndexPack>::value;
	    if constexpr(N < S-1){
		return std::get<N>(ipack)
		    ->iforh( step*std::get<N+1>(ba), mkForh<N+1>(step, ipack, ba, exs) );
	    }
	    else {
		return std::get<N>(ipack)->iforh( step*std::get<N+1>(ba), exs);
	    }
	}

	template <size_t N, class IndexPack, class BlockArray, class Exprs>
	auto mkPFor(size_t step, const IndexPack& ipack, const BlockArray& ba, Exprs exs)
	{
	    constexpr size_t S = std::tuple_size<IndexPack>::value;
	    if constexpr(N < S-1){
		return std::get<N>(ipack)
		    ->pifor( step*std::get<N+1>(ba), mkFor<N+1>(step, ipack, ba, exs) );
		// mkFor is correct here, because we want to multithread only the FIRST index!!
	    }
	    else {
		return std::get<N>(ipack)->pifor( step*std::get<N+1>(ba), exs);
	    }
	}

	template <size_t N, class RangeTuple, typename... SIZET>
	inline void resolveRangeType(const vector<std::shared_ptr<RangeBase> >& orig,
				     RangeTuple& rtp, size_t off, size_t size, SIZET... sizes)
	{
	    constexpr size_t tps = std::tuple_size<RangeTuple>::value;
	    ::CNORXZ::RangeHelper::resolveSetRange(std::get<N>(rtp), orig, off, size);
	    if constexpr(N < tps-1){
		resolveRangeType<N+1>(orig, rtp, off+size, sizes...);
	    }
	}

	template <size_t N, class RangeTuple>
	inline void resolveRangeType(const vector<std::shared_ptr<RangeBase> >& orig,
				     RangeTuple& rtp, size_t off, size_t size)
	{
	    constexpr size_t tps = std::tuple_size<RangeTuple>::value;
	    ::CNORXZ::RangeHelper::resolveSetRange(std::get<N>(rtp), orig, off, size);
	}

	template <size_t N, class MetaType, class... Ranges>
	inline size_t getCMeta(MetaType* xtarget, size_t pos,
			       const std::tuple<std::shared_ptr<Ranges>...>& stp, size_t off)
	{
	    //constexpr size_t NN = sizeof...(Ranges);
	    auto& r = *std::get<N>(stp);
	    const size_t ownPos = pos % r.size();
	    const size_t s = r.cmetaSize();
	    off -= s;
	    r.cmeta(reinterpret_cast<char*>(&std::get<N>(*xtarget)), ownPos);
	    if constexpr(N != 0){
		return s + getCMeta<N-1>(xtarget, (pos - ownPos) / r.size(), stp, off);
	    }
	    else {
		assert(off == 0);
		return s;
	    }
	}

	template <size_t N, class... Ranges>
	inline size_t getCMetaSize(const std::tuple<std::shared_ptr<Ranges>...>& stp)
	{
	    auto& r = *std::get<N>(stp);
	    if constexpr(N < sizeof...(Ranges)-1){
		return r.cmetaSize() + getCMetaSize<N+1>(stp);
	    }
	    else {
		return r.cmetaSize();
	    }
	}

	template <size_t N, class MIndex>
	inline std::string getStringMeta(const MIndex& mi)
	{
	    if constexpr(N < MIndex::sDim()-1){
		return mi.template getPtr<N>()->stringMeta() + "," + getStringMeta<N+1>(mi);
	    }
	    else {
		return mi.template getPtr<N>()->stringMeta();
	    }
	}

	template <class Index>
	inline size_t getStepSize(const Index& ii, std::intptr_t j);

	template <size_t N, class Index>
	inline void getStepSizeX(const Index& ii, std::intptr_t j, size_t& ss, size_t& sx)
	{
	    const auto& ni = ii.template get<N>();
	    const size_t max = ni.max();
	    const size_t tmp = getStepSize(ni, j);
	    ss += tmp * ii.template getBlockSize<N+1>();
	    sx *= max;
	    if constexpr(N != 0){
		getStepSizeX<N-1>(ii, j, ss, sx);
	    }
	}

	template <class Index>
	inline size_t getStepSize(const Index& ii, std::intptr_t j)
	{
	    constexpr IndexType IT = Index::sType();
	    if constexpr(IT == IndexType::SINGLE){
		const std::intptr_t ip = reinterpret_cast<std::intptr_t>(&ii);
		return ip == j ? 1 : 0;
	    }
	    else {
		size_t ss = 0;
		size_t sx = 1;
		constexpr size_t DIM = Index::sDim();
		getStepSizeX<DIM-1>(ii, j, ss, sx);
		return ss;
	    }
	}


	template <size_t N, size_t SIZE, class Range, class... Ranges>
	inline bool compareSpaceTypes(const vector<std::shared_ptr<RangeBase> >& rbvec)
	{
	    if constexpr(N != 0){
		return rbvec[SIZE-N-1]->spaceType() == Range::STYPE and compareSpaceTypes<N-1,SIZE,Ranges...>(rbvec);
	    }
	    else {
		return rbvec[SIZE-N-1]->spaceType() == Range::STYPE;
	    }
	}

	template <size_t N, class... Ranges>
	inline void setSpace(const vector<std::shared_ptr<RangeBase> >& rbvec,
			     std::tuple<std::shared_ptr<Ranges>...>& stp)
	{
	    typedef typename std::remove_reference<decltype(*std::get<N>( stp ))>::type RType;
	    std::get<N>( stp ) = std::dynamic_pointer_cast<RType>( rbvec[N] );
	    if constexpr(N != 0){
		setSpace<N-1>(rbvec, stp);
	    }
	}

    } // namespace RangeHelper
} // namespace CNORXZ

#endif
