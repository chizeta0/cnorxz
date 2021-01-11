
#ifndef __rpack_num_h__
#define __rpack_num_h__

#include <memory>
#include <vector>
#include <cassert>
#include "ranges/rbase_def.h"
#include "ranges/index_base.h"
#include "ranges/x_to_string.h"

namespace MultiArrayHelper
{
    using namespace MultiArrayTools;

    
    template <class Index1>
    size_t mkTotalDim();
    
    template <class Index1, class Index2, class... Indices>
    size_t mkTotalDim();


    template <class RangeType, class... Ranges>
    struct SubProp
    {
	static constexpr size_t ISSTATIC = RangeType::ISSTATIC & SubProp<Ranges...>::ISSTATIC;
	static constexpr size_t SIZE = RangeType::SIZE * SubProp<Ranges...>::SIZE;
    };

    template <>
    struct SubProp<None>
    {
	static constexpr size_t ISSTATIC = 1;
	static constexpr size_t SIZE = 1;
    };

    template <class Index>
    inline size_t getStepSize(const Index& ii, std::intptr_t j);

    
    template <class Range>
    inline void resolveSetRange(std::shared_ptr<Range>& rp, const vector<std::shared_ptr<RangeBase> >& orig,
				size_t origpos, size_t size);

    template <class Range>
    inline void setRangeToVec(vector<std::shared_ptr<RangeBase> >& v,
			      std::shared_ptr<Range> r);
    
    template <size_t N>
    struct RPackNum
    {
	template <class... Indices>
	static void initBlockSizes(std::array<size_t,sizeof...(Indices)+1>& bs,
				   std::tuple<std::shared_ptr<Indices>...>& ip);
	
	template <class... Indices>
	static inline void pp(std::tuple<std::shared_ptr<Indices>...>& ip);
	
	template <class... Indices>
	static inline int pp(std::tuple<std::shared_ptr<Indices>...>& ip,
			     std::array<size_t,sizeof...(Indices)+1>& bs,
			     std::intptr_t idxPtrNum);

	template <class... Indices>
	static inline void mm(std::tuple<std::shared_ptr<Indices>...>& ip);
	
	// !!!!
	template <class... Indices>
	static inline int mm(std::tuple<std::shared_ptr<Indices>...>& ip,
			     std::array<size_t,sizeof...(Indices)+1>& bs,
			     std::intptr_t idxPtrNum);
	
	template <class RangeTuple>
	static size_t getSize(const RangeTuple& rt);
	
	template <class IndexPack, class MetaType>
	static void getMetaPos(MetaType& target,
			       const IndexPack& source);

	template <class IndexPack, typename MetaType>
	static void setMeta(IndexPack& target, const MetaType& source);

	template <class SpaceClass>
	inline static std::shared_ptr<RangeBase> getSub(const SpaceClass& space, size_t num);

	template <class IndexPack>
	static void setIndexPack(IndexPack& iPack, size_t pos);
	
	template <class MRange, class... Indices>
	static void construct(std::tuple<std::shared_ptr<Indices>...>& ip,
			      const MRange& range);

	template <class IndexType, class... Indices>
	static void copyInst(std::tuple<std::shared_ptr<Indices>...>& ip,
			     const IndexType& ind);

	template <class IndexType, class... Indices>
	static void copyIndex(std::tuple<std::shared_ptr<Indices>...>& ip,
			      const IndexType& ind);

	template <class... Indices>
	static inline size_t makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup);

	template <class... Indices>
	static inline size_t makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup,
				     const std::array<size_t,sizeof...(Indices)+1>& blockSize);

	template <class Pack, class IndexType, class... Indices>
	static void swapIndices(Pack& ipack, const std::shared_ptr<IndexType>& nind,
				const std::shared_ptr<Indices>&... ninds);

        template <class Pack, class... Indices>
	static void swapIndices(Pack& ipack, const std::tuple<std::shared_ptr<Indices>...>& ninds);

	template <class... Indices>
	static size_t blockSize(const std::tuple<std::shared_ptr<Indices>...>& pack);


	template <class... Ranges>
	static inline void RangesToVec(const std::tuple<std::shared_ptr<Ranges>...>& rst,
				       vector<std::shared_ptr<RangeBase> >& v);

	template <class... Ranges>
	static inline void RangesToVec(const std::tuple<std::shared_ptr<Ranges>...>& rst,
				       vector<std::intptr_t>& v);

	template <class... Indices>
	static void printIndex(const std::tuple<std::shared_ptr<Indices>...>& ip, size_t offset);

	template <class Range, class... Ranges>
	static void checkDefaultable();

	template <class IndexPack, class BlockArray, class Exprs>
	static auto mkFor(size_t step, const IndexPack& ipack, const BlockArray& ba, Exprs exs)
	    -> decltype(std::get<std::tuple_size<IndexPack>::value-N-1>(ipack)
			->ifor( 0, RPackNum<N-1>::mkFor(step, ipack, ba, exs) ) );

	template <class IndexPack, class BlockArray, class Exprs>
	static auto mkForh(size_t step, const IndexPack& ipack, const BlockArray& ba, Exprs exs)
	    -> decltype(std::get<std::tuple_size<IndexPack>::value-N-1>(ipack)
			->iforh( 0, RPackNum<N-1>::mkForh(step, ipack, ba, exs) ) );

        template <class IndexPack, class BlockArray, class Exprs>
	static auto mkPFor(size_t step, const IndexPack& ipack, const BlockArray& ba, Exprs exs)
	    -> decltype(std::get<std::tuple_size<IndexPack>::value-N-1>(ipack)
			->pifor( 0, RPackNum<N-1>::mkFor(step, ipack, ba, exs) ) );

	template <class Index>
	static inline void getStepSizeX(const Index& ii, std::intptr_t j, size_t& ss, size_t& sx);

	template <class RangeTuple, typename... SIZET>
	static inline void resolveRangeType(const vector<std::shared_ptr<RangeBase> >& orig,
					    RangeTuple& rtp, size_t off, size_t size, SIZET... sizes);

	template <class... Ranges>
	static inline bool checkIfCreated(const std::tuple<std::shared_ptr<Ranges>...>& p,
					  const vector<std::intptr_t>& a);

	template <class MIndex>
	static inline std::string getStringMeta(const MIndex& mi);

	template <class... Ranges>
	static inline void fillRangeDataVec(vector<char>& out,
					    const std::tuple<std::shared_ptr<Ranges>...>& tp);

	template <size_t SIZE, class Range, class... Ranges>
	static inline bool compareSpaceTypes(const vector<std::shared_ptr<RangeBase> >& rbvec);

	template <class... Ranges>
	static inline void setSpace(const vector<std::shared_ptr<RangeBase> >& rbvec,
				    std::tuple<std::shared_ptr<Ranges>...>& stp);

        template <class MetaType, class... Ranges>
        static inline size_t getCMeta(MetaType* xtarget, size_t pos,
                                      const std::tuple<std::shared_ptr<Ranges>...>& stp, size_t off);

        template <class... Ranges>
        static inline size_t getCMetaSize(const std::tuple<std::shared_ptr<Ranges>...>& stp);

        template <class... Ranges>
        static inline void getTypeNum(vector<size_t>& res, const std::tuple<std::shared_ptr<Ranges>...>& stp);

	template <class... Ranges>
	static inline size_t getMeta(const std::tuple<std::shared_ptr<Ranges>...>& space,
				     const std::tuple<typename Ranges::IndexType::MetaType...>& meta);
    };

    
    template<>
    struct RPackNum<0>
    {

	template <class... Indices>
	static void initBlockSizes(std::array<size_t,sizeof...(Indices)+1>& bs,
				   std::tuple<std::shared_ptr<Indices>...>& ip);

	template <class... Indices>
	static inline void pp(std::tuple<std::shared_ptr<Indices>...>& ip);
	
	template <class... Indices>
	static inline int pp(std::tuple<std::shared_ptr<Indices>...>& ip,
			     std::array<size_t,sizeof...(Indices)+1>& bs,
			     std::intptr_t idxPtrNum);

	template <class... Indices>
	static inline void mm(std::tuple<std::shared_ptr<Indices>...>& ip);
	
	template <class... Indices>
	static inline int mm(std::tuple<std::shared_ptr<Indices>...>& ip,
			     std::array<size_t,sizeof...(Indices)+1>& bs,
			     std::intptr_t idxPtrNum);
	
	template <class RangeTuple>
	static size_t getSize(const RangeTuple& rt);
	
	template <class IndexPack, class MetaType>
	static void getMetaPos(MetaType& target,
			       const IndexPack& source);
            
	template <class IndexPack, typename MetaType>
	static void setMeta(IndexPack& target, const MetaType& source);

	template <class SpaceClass>
	inline static std::shared_ptr<RangeBase> getSub(const SpaceClass& space, size_t num);

	template <class IndexPack>
	static void setIndexPack(IndexPack& iPack, size_t pos);
	
	template <class MRange, class... Indices>
	static void construct(std::tuple<std::shared_ptr<Indices>...>& ip,
			      const MRange& range);

	template <class IndexType, class... Indices>
	static void copyInst(std::tuple<std::shared_ptr<Indices>...>& ip,
			     const IndexType& ind);

	template <class IndexType, class... Indices>
	static void copyIndex(std::tuple<std::shared_ptr<Indices>...>& ip,
			      const IndexType& ind);

	template <class... Indices>
	static inline size_t makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup);

	template <class... Indices>
	static inline size_t makePos(const std::tuple<std::shared_ptr<Indices>...>& iPtrTup,
				     const std::array<size_t,sizeof...(Indices)+1>& blockSize);

	template <class Pack, class IndexType>
	static void swapIndices(Pack& ipack, const std::shared_ptr<IndexType>& nind);

        template <class Pack, class... Indices>
	static void swapIndices(Pack& ipack, const std::tuple<std::shared_ptr<Indices>...>& ninds);

	template <class... Indices>
	static size_t blockSize(const std::tuple<std::shared_ptr<Indices>...>& pack);

	template <class... Ranges>
	static inline void RangesToVec(const std::tuple<std::shared_ptr<Ranges>...>& rst,
				       vector<std::intptr_t>& v);
	
	template <class... Ranges>
	static inline void RangesToVec(const std::tuple<std::shared_ptr<Ranges>...>& rst,
				       vector<std::shared_ptr<RangeBase> >& v);

	template <class... Indices>
	static void printIndex(const std::tuple<std::shared_ptr<Indices>...>& ip, size_t offset);

	template <class Range>
	static void checkDefaultable();

	template <class IndexPack, class BlockArray, class Exprs>
	static auto mkFor(size_t step, const IndexPack& ipack, const BlockArray& ba, Exprs exs)
	    -> decltype(std::get<std::tuple_size<IndexPack>::value-1>(ipack)
			->ifor(0,exs) );

	template <class IndexPack, class BlockArray, class Exprs>
	static auto mkForh(size_t step, const IndexPack& ipack, const BlockArray& ba, Exprs exs)
	    -> decltype(std::get<std::tuple_size<IndexPack>::value-1>(ipack)
			->iforh(0,exs) );

        template <class IndexPack, class BlockArray, class Exprs>
	static auto mkPFor(size_t step, const IndexPack& ipack, const BlockArray& ba, Exprs exs)
	    -> decltype(std::get<std::tuple_size<IndexPack>::value-1>(ipack)
			->pifor(0,exs) );

	template <class Index>
	static inline void getStepSizeX(const Index& ii, std::intptr_t j, size_t& ss, size_t& sx);

	template <class RangeTuple, typename... SIZET>
	static inline void resolveRangeType(const vector<std::shared_ptr<RangeBase> >& orig,
					    RangeTuple& rtp, size_t off, size_t size);

	template <class... Ranges>
	static inline bool checkIfCreated(const std::tuple<std::shared_ptr<Ranges>...>& p,
					  const vector<std::intptr_t>& a);

        template <class MIndex>
	static inline std::string getStringMeta(const MIndex& mi);

	template <class... Ranges>
	static inline void fillRangeDataVec(vector<char>& out,
					    const std::tuple<std::shared_ptr<Ranges>...>& tp);

	template <size_t SIZE, class Range>
	static inline bool compareSpaceTypes(const vector<std::shared_ptr<RangeBase> >& rbvec);

	template <class... Ranges>
	static inline void setSpace(const vector<std::shared_ptr<RangeBase> >& rbvec,
				    std::tuple<std::shared_ptr<Ranges>...>& stp);

        template <class MetaType, class... Ranges>
        static inline size_t getCMeta(MetaType* xtarget, size_t pos,
                                      const std::tuple<std::shared_ptr<Ranges>...>& stp, size_t off);

        template <class... Ranges>
        static inline size_t getCMetaSize(const std::tuple<std::shared_ptr<Ranges>...>& stp);

        template <class... Ranges>
        static inline void getTypeNum(vector<size_t>& res, const std::tuple<std::shared_ptr<Ranges>...>& stp);

	template <class... Ranges>
	static inline size_t getMeta(const std::tuple<std::shared_ptr<Ranges>...>& space,
				     const std::tuple<typename Ranges::IndexType::MetaType...>& meta);
    };
    
    template <IndexType IT>
    struct SSG
    {
	template <class Index>
	static inline size_t getStepSize(const Index& ii, std::intptr_t j);
    };

    template <>
    struct SSG<IndexType::SINGLE>
    {
	template <class Index>
	static inline size_t getStepSize(const Index& ii, std::intptr_t j);
    };
    

} //  end namespace MultiArrayHelper

//#include "rpack_num.cc.h"

#endif
