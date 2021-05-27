
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


    /*
    template <class Range>
    inline void resolveSetRange(std::shared_ptr<Range>& rp, const vector<std::shared_ptr<RangeBase> >& orig,
				size_t origpos, size_t size)
    {
	assert(size == 1);
	rp = std::dynamic_pointer_cast<Range>( orig[origpos] ); // catch bad cast here!!
    }
    */

    template <class Range>
    inline void setRangeToVec(vector<std::shared_ptr<RangeBase> >& v,
			      std::shared_ptr<Range> r)
    {
	v.insert(v.begin(), r);
    }
    
    template <size_t N>
    template <class... Indices>
    inline size_t RPackNum<N>::makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup,
                                              const std::array<size_t,sizeof...(Indices)+1>& blockSize)
    {
        return RPackNum<N-1>::makePos(iPtrTup, blockSize) + std::get<N>(iPtrTup)->pos() * std::get<N+1>(blockSize);
    }

    template <size_t N>
    template <class... Indices>
    size_t RPackNum<N>::blockSize(const std::tuple<std::shared_ptr<Indices>...>& pack)
    {
        return std::get<sizeof...(Indices)-N>(pack)->max() * RPackNum<N-1>::blockSize(pack);
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
    template <class MIndex>
    inline std::string RPackNum<N>::getStringMeta(const MIndex& mi)
    {
        return RPackNum<N-1>::getStringMeta(mi) + "," + mi.template getPtr<N>()->stringMeta();
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


    template <class... Indices>
    inline size_t RPackNum<0>::makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup,
                                              const std::array<size_t,sizeof...(Indices)+1>& blockSize)
    {
        return std::get<0>(iPtrTup)->pos() * std::get<1>(blockSize);
    }

    template <class... Indices>
    size_t RPackNum<0>::blockSize(const std::tuple<std::shared_ptr<Indices>...>& pack)
    {
        return 1;
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

    template <class MIndex>
    inline std::string RPackNum<0>::getStringMeta(const MIndex& mi)
    {
        return mi.template getPtr<0>()->stringMeta();
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
