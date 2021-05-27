
#ifndef __range_helper_h__
#define __range_helper_h__

namespace MultiArrayTools
{
    namespace RangeHelper
    {
	template <class Range>
	inline void resolveSetRange(std::shared_ptr<Range>& rp, const vector<std::shared_ptr<RangeBase> >& orig,
				    size_t origpos, size_t size)
	{
	    assert(size == 1);
	    rp = std::dynamic_pointer_cast<Range>( orig[origpos] ); // catch bad cast here!!
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
	    ::MultiArrayTools::RangeHelper::resolveSetRange(std::get<N>(rtp), orig, off, size);
	    if constexpr(N < tps-1){
		resolveRangeType<N+1>(orig, rtp, off+size, sizes...);
	    }
	}

	template <size_t N, class RangeTuple>
	inline void resolveRangeType(const vector<std::shared_ptr<RangeBase> >& orig,
				     RangeTuple& rtp, size_t off, size_t size)
	{
	    constexpr size_t tps = std::tuple_size<RangeTuple>::value;
	    ::MultiArrayTools::RangeHelper::resolveSetRange(std::get<N>(rtp), orig, off, size);
	}

	
    } // namespace RangeHelper
} // namespace MultiArrayTools

#endif
