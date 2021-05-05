
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
    template <class... Indices>
    void RPackNum<N>::initBlockSizes(std::array<size_t,sizeof...(Indices)+1>& bs,
                                            std::tuple<std::shared_ptr<Indices>...>& ip)
    {
        std::get<N>(bs) = RPackNum<sizeof...(Indices)-N>::blockSize(ip);
        RPackNum<N-1>::initBlockSizes(bs, ip);
    }
	
    template <size_t N>
    template <class... Indices>
    inline void RPackNum<N>::pp(std::tuple<std::shared_ptr<Indices>...>& ip)
    {
        auto& si = *std::get<N>(ip);
        if(si.last()){
            si = 0;
            RPackNum<N-1>::pp(ip);
        }
        else {
            ++si;
        }
    }
	
    template <size_t N>
    template <class... Indices>
    inline int RPackNum<N>::pp(std::tuple<std::shared_ptr<Indices>...>& ip,
                                      std::array<size_t,sizeof...(Indices)+1>& bs,
                                      std::intptr_t idxPtrNum)
    {
        auto& siPtr = std::get<N>(ip);
        if(reinterpret_cast<std::intptr_t>(siPtr.get()) == idxPtrNum){
            return RPackNum<N-1>::pp(ip, bs, idxPtrNum);
        }
        else {
            int tmp = siPtr->pp(idxPtrNum);
            if(siPtr->pos() == siPtr->max()){
                (*siPtr) = 0;
                return RPackNum<N-1>::pp(ip, bs, idxPtrNum) - siPtr->max() + 1;
            }
            else {
                return tmp * std::get<N+1>(bs);
            }
        }
    }

    template <size_t N>
    template <class... Indices>
    inline void RPackNum<N>::mm(std::tuple<std::shared_ptr<Indices>...>& ip)
    {
        auto& si = *std::get<N>(ip);
        if(si.first()){
            si = si.max() - 1;
            RPackNum<N-1>::mm(ip);
        }
        else {
            --si;
        }
    }
	
    // !!!!
    template <size_t N>
    template <class... Indices>
    inline int RPackNum<N>::mm(std::tuple<std::shared_ptr<Indices>...>& ip,
                                      std::array<size_t,sizeof...(Indices)+1>& bs,
                                      std::intptr_t idxPtrNum)
    {
        auto& siPtr = std::get<N>(ip);
        if(reinterpret_cast<std::intptr_t>(siPtr.get()) == idxPtrNum){
            return std::get<N>(bs) + RPackNum<N-1>::mm(ip, bs, idxPtrNum);
        }
        else {
            if(siPtr->first()){
                (*siPtr) = siPtr->max() - 1;
                return RPackNum<N-1>::mm(ip, bs, idxPtrNum) - siPtr->max() + 1;
            }
            else {
                return siPtr->mm(idxPtrNum);
            }
        }
    }
	
    template <size_t N>
    template <class RangeTuple>
    size_t RPackNum<N>::getSize(const RangeTuple& rt)
    {
        return std::get<N>(rt)->size() * RPackNum<N-1>::getSize(rt);
    }
	
    template <size_t N>
    template <class IndexPack, class MetaType>
    void RPackNum<N>::getMetaPos(MetaType& target,
                                        const IndexPack& source)
    {
        std::get<N>(target) = std::get<N>(source)->meta();
        RPackNum<N-1>::getMetaPos(target, source);
    }

    template <size_t N>
    template <class IndexPack, typename MetaType>
    void RPackNum<N>::setMeta(IndexPack& target, const MetaType& source)
    {
        std::get<N>(target)->at( std::get<N>(source) );
        RPackNum<N-1>::setMeta(target, source);
    }

    template <size_t N>
    template <class SpaceClass>
    inline std::shared_ptr<RangeBase> RPackNum<N>::getSub(const SpaceClass& space, size_t num)
    {
        if(num == N){
            return std::get<N>(space);
        }
        else {
            return RPackNum<N-1>::getSub(space, num);
        }
    }

    template <size_t N>
    template <class IndexPack>
    void RPackNum<N>::setIndexPack(IndexPack& iPack, size_t pos)
    {
        auto& i = *std::get<N>(iPack).get();
        const size_t ownPos = pos % i.max(); 
        i = ownPos;
        RPackNum<N-1>::setIndexPack(iPack, (pos - ownPos) / i.max() );
    }
	
    template <size_t N>
    template <class MRange, class... Indices>
    void RPackNum<N>::construct(std::tuple<std::shared_ptr<Indices>...>& ip,
                                       const MRange& range)
    {
        typedef typename std::remove_reference<decltype(range.template get<N>())>::type SubRangeType;
        typedef typename SubRangeType::IndexType SubIndexType;
        typedef typename std::remove_reference<decltype(*std::get<N>(ip).get())>::type TypeFromIndexPack;
	    
        static_assert(std::is_same<SubIndexType,TypeFromIndexPack>::value,
                      "inconsistent types");
	    
        std::get<N>(ip) = std::shared_ptr<SubIndexType>( new SubIndexType( range.template getPtr<N>() ) );
        *std::get<N>(ip) = 0;
	//VCHECK(std::get<N>(ip)->max());
        RPackNum<N-1>::construct(ip, range);
    }

    template <size_t N>
    template <class IndexType, class... Indices>
    void RPackNum<N>::copyInst(std::tuple<std::shared_ptr<Indices>...>& ip,
                                      const IndexType& ind)
    {
        std::get<N>(ip) = ind.template getPtr<N>() ;
        RPackNum<N-1>::copyInst(ip, ind);
    }

    template <size_t N>
    template <class IndexType, class... Indices>
    void RPackNum<N>::copyIndex(std::tuple<std::shared_ptr<Indices>...>& ip,
                                       const IndexType& ind)
    {
        typedef typename std::remove_reference<decltype(*std::get<N>(ip))>::type SubType;
        std::get<N>(ip) = std::make_shared<SubType>( ind.template get<N>() ) ;
        RPackNum<N-1>::copyIndex(ip, ind);
    }

    template <size_t N>
    template <class... Indices>
    inline size_t RPackNum<N>::makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup)
    {
        //const auto& idx = *std::get<N>(iPtrTup);
        return std::get<N>(iPtrTup)->pos() + RPackNum<N-1>::makePos(iPtrTup) * std::get<N>(iPtrTup)->max();
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
    template <class... Indices>
    void RPackNum<N>::printIndex(const std::tuple<std::shared_ptr<Indices>...>& ip, size_t offset)
    {
        std::get<N>(ip)->print(offset);
        RPackNum<N-1>::printIndex(ip, offset);
    }

    template <size_t N>
    template <class Range, class... Ranges>
    void RPackNum<N>::checkDefaultable()
    {
        static_assert( Range::defaultable, "not defaultable" );
        RPackNum<N-1>::template checkDefaultable<Ranges...>();
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

    

    template <class... Indices>
    void RPackNum<0>::initBlockSizes(std::array<size_t,sizeof...(Indices)+1>& bs,
                                            std::tuple<std::shared_ptr<Indices>...>& ip)
    {
        std::get<0>(bs) = RPackNum<sizeof...(Indices)>::blockSize(ip);
    }

    template <class... Indices>
    inline void RPackNum<0>::pp(std::tuple<std::shared_ptr<Indices>...>& ip)
    {
        auto& si = *std::get<0>(ip);
        ++si;
    }
	
    template <class... Indices>
    inline int RPackNum<0>::pp(std::tuple<std::shared_ptr<Indices>...>& ip,
                                      std::array<size_t,sizeof...(Indices)+1>& bs,
                                      std::intptr_t idxPtrNum)
    {
        auto& siPtr = std::get<0>(ip);
        if(reinterpret_cast<std::intptr_t>(siPtr.get()) == idxPtrNum){
            return std::get<0>(bs);
        }
        else {
            int tmp = siPtr->pp(idxPtrNum);
            return tmp * std::get<1>(bs);
        }
    }

    template <class... Indices>
    inline void RPackNum<0>::mm(std::tuple<std::shared_ptr<Indices>...>& ip)
    {
        auto& si = *std::get<0>(ip);
        --si;
    }
	
    template <class... Indices>
    inline int RPackNum<0>::mm(std::tuple<std::shared_ptr<Indices>...>& ip,
                                      std::array<size_t,sizeof...(Indices)+1>& bs,
                                      std::intptr_t idxPtrNum)
    {
        auto& siPtr = std::get<0>(ip);
        if(reinterpret_cast<std::intptr_t>(siPtr.get()) == idxPtrNum){
            return std::get<0>(bs);
            //return 1;
        }
        else {
            return siPtr->mm(idxPtrNum);
        }
    }
	
    template <class RangeTuple>
    size_t RPackNum<0>::getSize(const RangeTuple& rt)
    {
        return std::get<0>(rt)->size();
    }
	
    template <class IndexPack, class MetaType>
    void RPackNum<0>::getMetaPos(MetaType& target,
                                        const IndexPack& source)
    {
        std::get<0>(target) = std::get<0>(source)->meta();
    }
	
    template <class IndexPack, typename MetaType>
    void RPackNum<0>::setMeta(IndexPack& target, const MetaType& source)
    {
        std::get<0>(target)->at( std::get<0>( source ) );
    }

    template <class SpaceClass>
    inline std::shared_ptr<RangeBase> RPackNum<0>::getSub(const SpaceClass& space, size_t num)
    {
        if(num == 0){
            return std::get<0>(space);
        }
        else {
            assert(0);
            return std::shared_ptr<RangeBase>();
        }
    }

    template <class IndexPack>
    void RPackNum<0>::setIndexPack(IndexPack& iPack, size_t pos)
    {
        auto& i = *std::get<0>(iPack);
        const size_t ownPos = pos % i.max(); 
        i = ownPos;
    }
	
    template <class MRange, class... Indices>
    void RPackNum<0>::construct(std::tuple<std::shared_ptr<Indices>...>& ip,
                                       const MRange& range)
    {
        typedef typename std::remove_reference<decltype(range.template get<0>())>::type SubRangeType;
        typedef typename SubRangeType::IndexType SubIndexType;
        typedef typename std::remove_reference<decltype(*std::get<0>(ip).get())>::type TypeFromIndexPack;
	    
        static_assert(std::is_same<SubIndexType,TypeFromIndexPack>::value,
                      "inconsistent types");
	    
        std::get<0>(ip) = std::shared_ptr<SubIndexType>( new SubIndexType( range.template getPtr<0>() ) );
        *std::get<0>(ip) = 0;
    }

    template <class IndexType, class... Indices>
    void RPackNum<0>::copyInst(std::tuple<std::shared_ptr<Indices>...>& ip,
                                      const IndexType& ind)
    {
        std::get<0>(ip) = ind.template getPtr<0>();
    }

    template <class IndexType, class... Indices>
    void RPackNum<0>::copyIndex(std::tuple<std::shared_ptr<Indices>...>& ip,
                                       const IndexType& ind)
    {
        typedef typename std::remove_reference<decltype(*std::get<0>(ip))>::type SubType;
        std::get<0>(ip) = std::make_shared<SubType>( ind.template get<0>() ) ;
    }

    template <class... Indices>
    inline size_t RPackNum<0>::makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup)
    {
        return std::get<0>(iPtrTup)->pos();
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

    template <class... Indices>
    void RPackNum<0>::printIndex(const std::tuple<std::shared_ptr<Indices>...>& ip, size_t offset)
    {
        std::get<0>(ip)->print(offset);
    }

    template <class Range>
    void RPackNum<0>::checkDefaultable()
    {
        static_assert( Range::defaultable, "not defaultable" );
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
