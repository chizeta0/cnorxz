// -*- C++ -*-

#ifndef __container_index_h__
#define __container_index_h__

#include <cstdlib>
#include <tuple>
#include <memory>

#include "ranges/range_base.h"
#include "ranges/index_base.h"
#include "mbase_def.h"
#include "statics/static_for.h"
#include "ranges/range_helper.h"

namespace MultiArrayTools
{
    
    template <typename T, class... Indices>
    class ConstContainerIndex : public IndexInterface<ConstContainerIndex<T,Indices...>,
						 std::tuple<typename Indices::MetaType...> >,
			   public std::iterator<std::random_access_iterator_tag,T>
    {
    public:

	typedef IndexInterface<ConstContainerIndex<T,Indices...>,
			       std::tuple<typename Indices::MetaType...> > IB;
	typedef std::tuple<typename Indices::MetaType...>  MetaType;
	typedef std::tuple<std::shared_ptr<Indices>...> IndexPack;
	typedef ContainerRange<typename Indices::RangeType...> RangeType;

	static constexpr IndexType sType() { return IndexType::CONT; }
	static constexpr size_t sDim() { return sizeof...(Indices); }
	static constexpr size_t totalDim() { return (... * Indices::totalDim()); }

	static constexpr SpaceType STYPE = SpaceType::ANY;
        static constexpr bool PARALLEL = std::tuple_element<0,std::tuple<Indices...>>::type::PARALLEL;
        
	template <typename X>
	using CIX = ConstContainerIndex<X,Indices...>;
	
	template <typename X>
	friend class CIX;
	
    private:
	
	ConstContainerIndex() = default;
	
	bool mNonTrivialBlocks = false;
	bool mExternControl = false;
	IndexPack mIPack;
	std::array<size_t,sizeof...(Indices)+1> mBlockSizes; 
	const T* mData = nullptr;
	size_t mCPos;
	std::intptr_t mObjPtrNum;
	
    public:

	ConstContainerIndex(const ConstContainerIndex& in) = default;	
	ConstContainerIndex& operator=(const ConstContainerIndex& in) = default;

	ConstContainerIndex(const ConstContainerIndex& in, bool copy);
	
	ConstContainerIndex& copy(const ConstContainerIndex& in);
	
	template <typename X>
	ConstContainerIndex& operator=(const ConstContainerIndex<X,Indices...>& in);
	
	template <class MRange>
	ConstContainerIndex(const std::shared_ptr<MRange>& range,
		       std::intptr_t objPtrNum);

	template <class MRange>
	ConstContainerIndex(const std::shared_ptr<MRange>& range,
		       std::intptr_t objPtrNum,
		       const std::array<size_t,sizeof...(Indices)+1>& blockSizes);

	template <size_t N>
	auto get() const -> decltype( *std::get<N>( mIPack ) )&;

	template <size_t N>
	auto getPtr() const -> decltype( std::get<N>( mIPack ) )&;

	template <size_t N>
	size_t getBlockSize() const { return std::get<N>(mBlockSizes); }

	const IndexPack& pack() const { return mIPack; }
	
	ConstContainerIndex& sync(); // recalculate 'IB::mPos' when externalControl == true
	ConstContainerIndex& operator()(const std::shared_ptr<Indices>&... inds); // control via external indices
        ConstContainerIndex& operator()(const std::tuple<std::shared_ptr<Indices>...>& inds);
	ConstContainerIndex& operator()(); // -> sync; just to shorten the code

	// ==== >>>>> STATIC POLYMORPHISM <<<<< ====
	
	IndexType type() const;
	
	ConstContainerIndex& operator++();
	ConstContainerIndex& operator--();
	
	ConstContainerIndex& operator=(size_t pos);

	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);

	std::string stringMeta() const;
	MetaType meta() const;
	ConstContainerIndex& at(const MetaType& metaPos);

	size_t dim() const;
	bool first() const;
	bool last() const;
	bool sliceMode() const;
	
	std::shared_ptr<RangeType> range();
	
	template <size_t N>
	auto getPtr() -> decltype( std::get<N>( mIPack ) )&;

	size_t getStepSize(size_t n);

	template <class Exprs>
	auto ifor(size_t step, Exprs exs) const;

	template <class Exprs>
	auto iforh(size_t step, Exprs exs) const;

        template <class Exprs>
	auto pifor(size_t step, Exprs exs) const;

	std::intptr_t container() const;
	ConstContainerIndex& format(const std::array<size_t,sizeof...(Indices)+1>& blocks);
	
	// Iterator Stuff
	
	ConstContainerIndex& setData(const T* data);
	
	const T& operator*() const;
	const T* operator->() const;
	//T& operator*();
	//T* operator->();

	ConstContainerIndex operator++(int);
	ConstContainerIndex operator--(int);
	ConstContainerIndex& operator+=(int diff);
	ConstContainerIndex& operator-=(int diff);
	ConstContainerIndex operator+(int num) const;
	ConstContainerIndex operator-(int num) const;

	int operator-(const ConstContainerIndex& it) const;
	const T& operator[](int num) const;

	bool operator<(const ConstContainerIndex& it) const;
	bool operator>(const ConstContainerIndex& it) const;
	bool operator<=(const ConstContainerIndex& it) const;
	bool operator>=(const ConstContainerIndex& it) const;
	
    };
    
} // end namespace MultiArrayTools

#endif
