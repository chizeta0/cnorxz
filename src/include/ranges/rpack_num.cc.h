
#include "rpack_num.h"
namespace MultiArrayHelper
{
    using namespace MultiArrayTools;

    template <class Index1>
    size_t mkTotalDim()
    {
	return Index1::totalDim();
    }
    
    template <class Index1, class Index2, class... Indices>
    size_t mkTotalDim()
    {
	return Index1::totalDim() * mkTotalDim<Index2,Indices...>();
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
    
    template <size_t N>
    template <class RangeTuple>
    size_t RPackNum<N>::getSize(const RangeTuple& rt)
    {
        return std::get<N>(rt)->size() * RPackNum<N-1>::getSize(rt);
    }
	
    template <size_t N>
    template <class... Indices>
    inline size_t RPackNum<N>::makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup,
                                              const std::array<size_t,sizeof...(Indices)+1>& blockSize)
    {
        return RPackNum<N-1>::makePos(iPtrTup, blockSize) + std::get<N>(iPtrTup)->pos() * std::get<N+1>(blockSize);
    }

    template <size_t N>
    template <class Pack, class IndexType, class... Indices>
    void RPackNum<N>::swapIndices(Pack& ipack, const std::shared_ptr<IndexType>& nind,
                                         const std::shared_ptr<Indices>&... ninds)
    {
        std::get<std::tuple_size<Pack>::value-N-1>(ipack) = nind;
        RPackNum<N-1>::swapIndices(ipack, ninds...);
    }

    template <size_t N>
    template <class Pack, class... Indices>
    void RPackNum<N>::swapIndices(Pack& ipack, const std::tuple<std::shared_ptr<Indices>...>& ninds)
    {
        std::get<N>(ipack) = std::get<N>(ninds);
        RPackNum<N-1>::swapIndices(ipack, ninds);
    }

    template <size_t N>
    template <class... Indices>
    size_t RPackNum<N>::blockSize(const std::tuple<std::shared_ptr<Indices>...>& pack)
    {
        return std::get<sizeof...(Indices)-N>(pack)->max() * RPackNum<N-1>::blockSize(pack);
    }


    template <size_t N>
    template <class... Ranges>
    inline void RPackNum<N>::RangesToVec(const std::tuple<std::shared_ptr<Ranges>...>& rst,
                                                vector<std::shared_ptr<RangeBase> >& v)
    {
        setRangeToVec(v, std::get<N>(rst));
        RPackNum<N-1>::RangesToVec(rst, v);
    }

    template <size_t N>
    template <class... Ranges>
    inline void RPackNum<N>::RangesToVec(const std::tuple<std::shared_ptr<Ranges>...>& rst,
                                                vector<std::intptr_t>& v)
    {
        v[N] = reinterpret_cast<std::intptr_t>( std::get<N>(rst).get() );
        RPackNum<N-1>::RangesToVec(rst, v);
    }

    template <size_t N>
    template <class IndexPack, class BlockArray, class Exprs>
    auto RPackNum<N>::mkFor(size_t step, const IndexPack& ipack, const BlockArray& ba, Exprs exs)
        -> decltype(std::get<std::tuple_size<IndexPack>::value-N-1>(ipack)
                    ->ifor( 0, RPackNum<N-1>::mkFor(step, ipack, ba, exs) ) )
    {
        constexpr size_t NN = std::tuple_size<IndexPack>::value-N-1;
        return std::get<NN>(ipack)
            ->ifor( step*std::get<NN+1>(ba), RPackNum<N-1>::mkFor(step, ipack, ba, exs) );
    }

    template <size_t N>
    template <class IndexPack, class BlockArray, class Exprs>
    auto RPackNum<N>::mkForh(size_t step, const IndexPack& ipack, const BlockArray& ba, Exprs exs)
        -> decltype(std::get<std::tuple_size<IndexPack>::value-N-1>(ipack)
                    ->iforh( 0, RPackNum<N-1>::mkForh(step, ipack, ba, exs) ) )
    {
        constexpr size_t NN = std::tuple_size<IndexPack>::value-N-1;
        return std::get<NN>(ipack)
            ->iforh( step*std::get<NN+1>(ba), RPackNum<N-1>::mkForh(step, ipack, ba, exs) );
    }

    template <size_t N>
    template <class IndexPack, class BlockArray, class Exprs>
    auto RPackNum<N>::mkPFor(size_t step, const IndexPack& ipack, const BlockArray& ba, Exprs exs)
        -> decltype(std::get<std::tuple_size<IndexPack>::value-N-1>(ipack)
                    ->pifor( 0, RPackNum<N-1>::mkFor(step, ipack, ba, exs) ) )
    {
        constexpr size_t NN = std::tuple_size<IndexPack>::value-N-1;
        return std::get<NN>(ipack)
            ->pifor( step*std::get<NN+1>(ba), RPackNum<N-1>::mkFor(step, ipack, ba, exs) );
        // mkFor is correct here, because we want to multithread only the FIRST index!!
    }

    template <size_t N>
    template <class Index>
    inline void RPackNum<N>::getStepSizeX(const Index& ii, std::intptr_t j, size_t& ss, size_t& sx)
    {
        //constexpr size_t DIM = Index::sDim();
        const auto& ni = ii.template get<N>();
        const size_t max = ni.max();
        const size_t tmp = getStepSize(ni, j);
        //ss += tmp * sx;
        ss += tmp * ii.template getBlockSize<N+1>();
        sx *= max;
        RPackNum<N-1>::getStepSizeX(ii, j, ss, sx);
    }

    template <size_t N>
    template <class RangeTuple, typename... SIZET>
    inline void RPackNum<N>::resolveRangeType(const vector<std::shared_ptr<RangeBase> >& orig,
                                                     RangeTuple& rtp, size_t off, size_t size, SIZET... sizes)
    {
        constexpr size_t tps = std::tuple_size<RangeTuple>::value;
        resolveSetRange(std::get<tps-N-1>(rtp), orig, off, size);
        RPackNum<N-1>::resolveRangeType(orig, rtp, off+size, sizes...);
    }

    template <size_t N>
    template <class... Ranges>
    inline bool RPackNum<N>::checkIfCreated(const std::tuple<std::shared_ptr<Ranges>...>& p,
                                                   const vector<std::intptr_t>& a)
    {
        return reinterpret_cast<std::intptr_t>( std::get<N>(p).get() ) == a[N] and
            RPackNum<N-1>::checkIfCreated(p,a);
    }

    template <size_t N>
    template <class MIndex>
    inline std::string RPackNum<N>::getStringMeta(const MIndex& mi)
    {
        return RPackNum<N-1>::getStringMeta(mi) + "," + mi.template getPtr<N>()->stringMeta();
    }

    template <size_t N>
    template <class... Ranges>
    inline void RPackNum<N>::fillRangeDataVec(vector<char>& out,
                                                     const std::tuple<std::shared_ptr<Ranges>...>& tp)
    {
        vector<char> part = std::get<sizeof...(Ranges)-N-1>(tp)->data();
        out.insert(out.end(), part.begin(), part.end());
        RPackNum<N-1>::fillRangeDataVec(out, tp);
    }

    template <size_t N>
    template <size_t SIZE, class Range, class... Ranges>
    inline bool RPackNum<N>::compareSpaceTypes(const vector<std::shared_ptr<RangeBase> >& rbvec)
    {
        return rbvec[SIZE-N-1]->spaceType() == Range::STYPE and RPackNum<N-1>::template compareSpaceTypes<SIZE,Ranges...>(rbvec);
    }

    template <size_t N>
    template <class... Ranges>
    inline void RPackNum<N>::setSpace(const vector<std::shared_ptr<RangeBase> >& rbvec,
                                             std::tuple<std::shared_ptr<Ranges>...>& stp)
    {
        typedef typename std::remove_reference<decltype(*std::get<N>( stp ))>::type RType;
        std::get<N>( stp ) = std::dynamic_pointer_cast<RType>( rbvec[N] );
        RPackNum<N-1>::setSpace(rbvec, stp);
    }

    template <size_t N>
    template <class MetaType, class... Ranges>
    inline size_t RPackNum<N>::getCMeta(MetaType* xtarget, size_t pos,
                                               const std::tuple<std::shared_ptr<Ranges>...>& stp, size_t off)
    {
        //constexpr size_t NN = sizeof...(Ranges);
        auto& r = *std::get<N>(stp);
        const size_t ownPos = pos % r.size();
        const size_t s = r.cmetaSize();
        off -= s;
        r.cmeta(reinterpret_cast<char*>(&std::get<N>(*xtarget)), ownPos);
        return s + RPackNum<N-1>::getCMeta(xtarget, (pos - ownPos) / r.size(), stp, off);
    }

    template <size_t N>
    template <class... Ranges>
    inline size_t RPackNum<N>::getCMetaSize(const std::tuple<std::shared_ptr<Ranges>...>& stp)
    {
        constexpr size_t NN = sizeof...(Ranges);
        auto& r = *std::get<NN-N-1>(stp);
        return r.cmetaSize() + RPackNum<N-1>::getCMetaSize(stp);
    }

    template <size_t N>
    template <class... Ranges>
    inline void RPackNum<N>::getTypeNum(vector<size_t>& res, const std::tuple<std::shared_ptr<Ranges>...>& stp)
    {
        auto& r = *std::get<N>(stp);
        auto tn = r.typeNum();
        res.insert(res.begin(), tn.begin(), tn.end());
        RPackNum<N-1>::getTypeNum(res, stp);
    }

    template <size_t N>
    template <class... Ranges>
    inline size_t RPackNum<N>::getMeta(const std::tuple<std::shared_ptr<Ranges>...>& space,
				       const std::tuple<typename Ranges::IndexType::MetaType...>& meta)
    {
	return RPackNum<N-1>::getMeta(space,meta) * std::get<N>(space)->size() +
	    std::get<N>(space)->getMeta(std::get<N>(meta));
    }

    

    template <class RangeTuple>
    size_t RPackNum<0>::getSize(const RangeTuple& rt)
    {
        return std::get<0>(rt)->size();
    }
	
    template <class... Indices>
    inline size_t RPackNum<0>::makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup,
                                              const std::array<size_t,sizeof...(Indices)+1>& blockSize)
    {
        return std::get<0>(iPtrTup)->pos() * std::get<1>(blockSize);
    }

    template <class Pack, class IndexType>
    void RPackNum<0>::swapIndices(Pack& ipack, const std::shared_ptr<IndexType>& nind)
    {
        std::get<std::tuple_size<Pack>::value-1>(ipack) = nind;
    }

    template <class Pack, class... Indices>
    void RPackNum<0>::swapIndices(Pack& ipack, const std::tuple<std::shared_ptr<Indices>...>& ninds)
    {
        std::get<0>(ipack) = std::get<0>(ninds);
    }

    template <class... Indices>
    size_t RPackNum<0>::blockSize(const std::tuple<std::shared_ptr<Indices>...>& pack)
    {
        return 1;
    }

    template <class... Ranges>
    inline void RPackNum<0>::RangesToVec(const std::tuple<std::shared_ptr<Ranges>...>& rst,
                                                vector<std::intptr_t>& v)
    {
        v[0] = reinterpret_cast<std::intptr_t>( std::get<0>(rst).get() );;
    }
	
    template <class... Ranges>
    inline void RPackNum<0>::RangesToVec(const std::tuple<std::shared_ptr<Ranges>...>& rst,
                                                vector<std::shared_ptr<RangeBase> >& v)
    {
        setRangeToVec(v, std::get<0>(rst));
    }

    template <class IndexPack, class BlockArray, class Exprs>
    auto RPackNum<0>::mkFor(size_t step, const IndexPack& ipack, const BlockArray& ba, Exprs exs)
        -> decltype(std::get<std::tuple_size<IndexPack>::value-1>(ipack)
                    ->ifor(0,exs) )
    {
        constexpr size_t NN = std::tuple_size<IndexPack>::value-1;
        return std::get<NN>(ipack)
            ->ifor( step*std::get<NN+1>(ba), exs);
    }

    template <class IndexPack, class BlockArray, class Exprs>
    auto RPackNum<0>::mkForh(size_t step, const IndexPack& ipack, const BlockArray& ba, Exprs exs)
        -> decltype(std::get<std::tuple_size<IndexPack>::value-1>(ipack)
                    ->iforh(0,exs) )
    {
        constexpr size_t NN = std::tuple_size<IndexPack>::value-1;
        return std::get<NN>(ipack)
            ->iforh( step*std::get<NN+1>(ba), exs);
    }

    template <class IndexPack, class BlockArray, class Exprs>
    auto RPackNum<0>::mkPFor(size_t step, const IndexPack& ipack, const BlockArray& ba, Exprs exs)
        -> decltype(std::get<std::tuple_size<IndexPack>::value-1>(ipack)
                    ->pifor(0,exs) )
    {
        constexpr size_t NN = std::tuple_size<IndexPack>::value-1;
        return std::get<NN>(ipack)
            ->pifor( step*std::get<NN+1>(ba), exs);
    }

    template <class Index>
    inline void RPackNum<0>::getStepSizeX(const Index& ii, std::intptr_t j, size_t& ss, size_t& sx)
    {
        //constexpr size_t DIM = Index::sDim();
        const auto& ni = ii.template get<0>();
        const size_t max = ni.max();
        const size_t tmp = getStepSize(ni, j);
        //ss += tmp * sx;
        ss += tmp * ii.template getBlockSize<1>();
        sx *= max;
    }

    template <class RangeTuple, typename... SIZET>
    inline void RPackNum<0>::resolveRangeType(const vector<std::shared_ptr<RangeBase> >& orig,
                                                     RangeTuple& rtp, size_t off, size_t size)
    {
        constexpr size_t tps = std::tuple_size<RangeTuple>::value;
        resolveSetRange(std::get<tps-1>(rtp), orig, off, size);
    }

    template <class... Ranges>
    inline bool RPackNum<0>::checkIfCreated(const std::tuple<std::shared_ptr<Ranges>...>& p,
                                                   const vector<std::intptr_t>& a)
    {
        return reinterpret_cast<std::intptr_t>( std::get<0>(p).get() ) == a[0];
    }

    template <class MIndex>
    inline std::string RPackNum<0>::getStringMeta(const MIndex& mi)
    {
        return mi.template getPtr<0>()->stringMeta();
    }

    template <class... Ranges>
    inline void RPackNum<0>::fillRangeDataVec(vector<char>& out,
                                                     const std::tuple<std::shared_ptr<Ranges>...>& tp)
    {
        vector<char> part = std::get<sizeof...(Ranges)-1>(tp)->data();
        out.insert(out.end(), part.begin(), part.end());
    }

    template <size_t SIZE, class Range>
    inline bool RPackNum<0>::compareSpaceTypes(const vector<std::shared_ptr<RangeBase> >& rbvec)
    {
        return rbvec[SIZE-1]->spaceType() == Range::STYPE;
    }

    template <class... Ranges>
    inline void RPackNum<0>::setSpace(const vector<std::shared_ptr<RangeBase> >& rbvec,
                                             std::tuple<std::shared_ptr<Ranges>...>& stp)
    {
        typedef typename std::remove_reference<decltype(*std::get<0>( stp ))>::type RType;
        std::get<0>( stp ) = std::dynamic_pointer_cast<RType>( rbvec[0] );
    }

    template <class MetaType, class... Ranges>
    inline size_t RPackNum<0>::getCMeta(MetaType* xtarget, size_t pos,
                                               const std::tuple<std::shared_ptr<Ranges>...>& stp, size_t off)
    {
        //constexpr size_t NN = sizeof...(Ranges);
        auto& r = *std::get<0>(stp);
        const size_t ownPos = pos % r.size();
        const size_t s = r.cmetaSize();
        off -= s;
        assert(off == 0);
        r.cmeta(reinterpret_cast<char*>(&std::get<0>(*xtarget)), ownPos);
        return s;
    }

    template <class... Ranges>
    inline size_t RPackNum<0>::getCMetaSize(const std::tuple<std::shared_ptr<Ranges>...>& stp)
    {
        constexpr size_t NN = sizeof...(Ranges);
        auto& r = *std::get<NN-1>(stp);
        return r.cmetaSize();
    }

    template <class... Ranges>
    inline void RPackNum<0>::getTypeNum(vector<size_t>& res, const std::tuple<std::shared_ptr<Ranges>...>& stp)
    {
        auto& r = *std::get<0>(stp);
        auto tn = r.typeNum();
        res.insert(res.begin(), tn.begin(), tn.end());
    }

    template <class... Ranges>
    inline size_t RPackNum<0>::getMeta(const std::tuple<std::shared_ptr<Ranges>...>& space,
				       const std::tuple<typename Ranges::IndexType::MetaType...>& meta)
    {
	return std::get<0>(space)->getMeta(std::get<0>(meta));
    }

    
    template <IndexType IT>
    template <class Index>
    inline size_t SSG<IT>::getStepSize(const Index& ii, std::intptr_t j)
    {
        size_t ss = 0;
        size_t sx = 1;
        constexpr size_t DIM = Index::sDim();
        RPackNum<DIM-1>::getStepSizeX(ii, j, ss, sx);
        return ss;
    }

    template <class Index>
    inline size_t SSG<IndexType::SINGLE>::getStepSize(const Index& ii, std::intptr_t j)
    {
        const std::intptr_t ip = reinterpret_cast<std::intptr_t>(&ii);
        return ip == j ? 1 : 0;
        //return ii.ptrNum() == j ? 1 : 0;
    }
    
    template <class Index>
    inline size_t getStepSize(const Index& ii, std::intptr_t j)
    {
	constexpr IndexType IT = Index::sType();
	return SSG<IT>::getStepSize(ii, j);
    }

} //  end namespace MultiArrayHelper
