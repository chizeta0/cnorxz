// -*- C++ -*-

#ifndef __container_range_h__
#define __container_range_h__

#include <cstdlib>
#include <tuple>
#include <memory>

#include "ranges/range_base.h"
#include "ranges/index_base.h"

#include "rpack_num.h"

namespace MultiArrayTools
{
    
    template <typename T, class... Indices>
    class ContainerIndex : public IndexInterface<ContainerIndex<T,Indices...>,
						 std::tuple<typename Indices::MetaType...> >,
			   public std::iterator<std::random_access_iterator_tag,T>
    {
    public:

	typedef IndexInterface<ContainerIndex<T,Indices...>,
			       std::tuple<typename Indices::MetaType...> > IB;
	typedef std::tuple<typename Indices::MetaType...>  MetaType;
	typedef std::tuple<std::shared_ptr<Indices>...> IndexPack;
	typedef ContainerRange<T,typename Indices::RangeType...> RangeType;

	static constexpr IndexType sType() { return IndexType::CONT; }
	static constexpr size_t sDim() { return sizeof...(Indices); }
	static constexpr size_t totalDim() { return mkTotalDim<Indices...>(); }

	static constexpr SpaceType STYPE = SpaceType::ANY;
        static constexpr bool PARALLEL = std::tuple_element<0,std::tuple<Indices...>>::type::PARALLEL;
        
	template <typename X>
	using CIX = ContainerIndex<X,Indices...>;
	
	template <typename X>
	friend class CIX;
	
    private:
	
	ContainerIndex() = default;
	
	bool mNonTrivialBlocks = false;
	bool mExternControl = false;
	IndexPack mIPack;
	std::array<size_t,sizeof...(Indices)+1> mBlockSizes; 
	const T* mData = nullptr;
	size_t mCPos;
	//const MultiArrayBase<T,typename Indices::RangeType...>* mMa = nullptr;
	std::intptr_t mObjPtrNum;
	
    public:

	ContainerIndex(const ContainerIndex& in) = default;	
	ContainerIndex& operator=(const ContainerIndex& in) = default;

	ContainerIndex(const ContainerIndex& in, bool copy) :
	    IB(in),
	    mNonTrivialBlocks(in.mNonTrivialBlocks),
	    mExternControl(false),
	    mBlockSizes(in.mBlockSizes),
	    mData(in.mData),
	    mCPos(in.mCPos),
	    mObjPtrNum(in.mObjPtrNum)
	{
	    sfor_pn<0,sizeof...(Indices)>
		( [&](auto i)
		{
		    typedef typename std::remove_reference<decltype(*std::get<i>(mIPack))>::type
			SubType;
		    std::get<i>(mIPack) = std::make_shared<SubType>( in.template get<i>() ) ;
		    return true;
		});
	}
	
	ContainerIndex& copy(const ContainerIndex& in)
	{
	    IB::operator=(in);
	    mNonTrivialBlocks = in.mNonTrivialBlocks;
	    mExternControl = false;
	    mBlockSizes = in.mBlockSizes;
	    mData = in.mData;
	    mCPos = in.mCPos;
	    mObjPtrNum = in.mObjPtrNum;
	    sfor_pn<0,sizeof...(Indices)>
		( [&](auto i)
		{
		    typedef typename std::remove_reference<decltype(*std::get<i>(mIPack))>::type
			SubType;
		    std::get<i>(mIPack) = std::make_shared<SubType>( in.template get<i>() ) ;
		    return true;
		});
	    return *this;
	}
	
	template <typename X>
	ContainerIndex& operator=(const ContainerIndex<X,Indices...>& in);
	
	template <class MRange>
	ContainerIndex(const std::shared_ptr<MRange>& range,
		       std::intptr_t objPtrNum);

	template <class MRange>
	ContainerIndex(const std::shared_ptr<MRange>& range,
		       std::intptr_t objPtrNum,
		       const std::array<size_t,sizeof...(Indices)+1>& blockSizes);

	template <size_t N>
	auto get() const -> decltype( *std::get<N>( mIPack ) )&;

	template <size_t N>
	auto getPtr() const -> decltype( std::get<N>( mIPack ) )&;

	template <size_t N>
	size_t getBlockSize() const { return std::get<N>(mBlockSizes); }

	const IndexPack& pack() const { return mIPack; }
	
	ContainerIndex& sync(); // recalculate 'IB::mPos' when externalControl == true
	ContainerIndex& operator()(const std::shared_ptr<Indices>&... inds); // control via external indices
        ContainerIndex& operator()(const std::tuple<std::shared_ptr<Indices>...>& inds);
	ContainerIndex& operator()(); // -> sync; just to shorten the code

	// ==== >>>>> STATIC POLYMORPHISM <<<<< ====
	
	IndexType type() const;
	
	ContainerIndex& operator++();
	ContainerIndex& operator--();
	
	ContainerIndex& operator=(size_t pos);

	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);

	std::string stringMeta() const;
	MetaType meta() const;
	ContainerIndex& at(const MetaType& metaPos);

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
	ContainerIndex& format(const std::array<size_t,sizeof...(Indices)+1>& blocks);
	
	// Iterator Stuff
	
	ContainerIndex& setData(const T* data);
	
	const T& operator*() const;
	const T* operator->() const;
	//T& operator*();
	//T* operator->();

	ContainerIndex operator++(int);
	ContainerIndex operator--(int);
	ContainerIndex& operator+=(int diff);
	ContainerIndex& operator-=(int diff);
	ContainerIndex operator+(int num) const;
	ContainerIndex operator-(int num) const;

	int operator-(const ContainerIndex& it) const;
	const T& operator[](int num) const;

	bool operator<(const ContainerIndex& it) const;
	bool operator>(const ContainerIndex& it) const;
	bool operator<=(const ContainerIndex& it) const;
	bool operator>=(const ContainerIndex& it) const;
	
    };
    
} // end namespace MultiArrayTools

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{
    namespace
    {
	using namespace MultiArrayHelper;
    }
    
    /**********************
     *   ContainerIndex   *
     **********************/

    template <typename T, class... Indices>
    template <class MRange>
    ContainerIndex<T,Indices...>::ContainerIndex(const std::shared_ptr<MRange>& range,
						 std::intptr_t objPtrNum) :
	IndexInterface<ContainerIndex<T,Indices...>,std::tuple<typename Indices::MetaType...> >(range, 0),
	mObjPtrNum(objPtrNum)
    {
	std::get<sizeof...(Indices)>(mBlockSizes) = 1;
	sfor_mn<sizeof...(Indices),0>
	    ( [&](auto i) {
		  auto r = range->template getPtr<i>();
		  std::get<i>(mIPack) = r->beginPtr();
		  *std::get<i>(mIPack) = 0;
		  
		  std::get<i>(mBlockSizes) = sfor_p<i,sizeof...(Indices)>
		      ( [&](auto j) { return std::get<j>(mIPack)->max(); } ,
			[&](auto a, auto b) { return a * b; });
		  return 0;
	      });
	
	IB::mPos = sfor_m<sizeof...(Indices),0>
	    ( [&](auto i) { return std::get<i>(mIPack); },
	      [&](auto a, auto b) {return a->pos() + b*a->max();}, 0 );
	mCPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack, mBlockSizes);
    }

    template <typename T, class... Indices>
    template <class MRange>
    ContainerIndex<T,Indices...>::ContainerIndex(const std::shared_ptr<MRange>& range,
						 std::intptr_t objPtrNum,
						 const std::array<size_t,sizeof...(Indices)+1>& blockSizes) :
	IndexInterface<ContainerIndex<T,Indices...>,std::tuple<typename Indices::MetaType...> >(range, 0),
	mObjPtrNum(objPtrNum)
    {
	sfor_mn<sizeof...(Indices),0>
	    ( [&](auto i) {
		  auto r = range->template getPtr<i>();
		  std::get<i>(mIPack) = r->beginPtr();
		  *std::get<i>(mIPack) = 0;
		  return 0;
	      });
	IB::mPos = sfor_m<sizeof...(Indices),0>
	    ( [&](auto i) { return std::get<i>(mIPack); },
	      [&](auto a, auto b) {return a->pos() + b*a->max();}, 0 );
	mCPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack, mBlockSizes);
	mNonTrivialBlocks = true;
    }

    template <typename T, class... Indices>
    template <typename X>
    ContainerIndex<T,Indices...>&
    ContainerIndex<T,Indices...>::operator=(const ContainerIndex<X,Indices...>& in)
    {
	mIPack = in.mIPack;
	return (*this)();
    }

    
    template <typename T, class... Indices>
    ContainerIndex<T,Indices...>& ContainerIndex<T,Indices...>::sync()
    {
	if(mExternControl){
	    IB::mPos = sfor_m<sizeof...(Indices),0>
		( [&](auto i) { return std::get<i>(mIPack); },
		  [&](auto a, auto b) {return a->pos() + b*a->max();}, 0 );
	    mCPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack, mBlockSizes);
	}
	return *this;
    }
    
    template <typename T, class... Indices>
    template <size_t N>
    auto ContainerIndex<T,Indices...>::get() const -> decltype( *std::get<N>( mIPack ) )&
    {
	return *std::get<N>( mIPack );
    }

    template <typename T, class... Indices>
    template <size_t N>
    auto ContainerIndex<T,Indices...>::getPtr() const -> decltype( std::get<N>( mIPack ) )&
    {
	return std::get<N>( mIPack );
    }

    template <typename T, class... Indices>
    ContainerIndex<T,Indices...>& ContainerIndex<T,Indices...>::operator()(const std::shared_ptr<Indices>&... inds)
    {
	return (*this)(std::make_tuple(inds...));
    }    

    template <typename T, class... Indices>
    ContainerIndex<T,Indices...>& ContainerIndex<T,Indices...>::operator()(const std::tuple<std::shared_ptr<Indices>...>& inds)
    {
	sfor_pn<0,sizeof...(Indices)>
	    ( [&](auto i) { std::get<i>(mIPack) = std::get<i>(inds); return 0; } );
	mExternControl = true;
	return sync();
    }    

    template <typename T, class... Indices>
    ContainerIndex<T,Indices...>& ContainerIndex<T,Indices...>::operator()()
    {
	return sync();
    }

    template <typename T, class... Indices>
    IndexType ContainerIndex<T,Indices...>::type() const { return IndexType::CONT; }

    template <typename T, class... Indices>
    ContainerIndex<T,Indices...>& ContainerIndex<T,Indices...>::operator++()
    {
	if(mExternControl){
	    IB::mPos = sfor_m<sizeof...(Indices),0>
		( [&](auto i) { return std::get<i>(mIPack); },
		  [&](auto a, auto b) {return a->pos() + b*a->max();}, 0 );
	}
	sfor_m<sizeof...(Indices),0>
	    ( [&](auto i) {
		auto& si = *std::get<i>( mIPack );
		if(si.last() and i != 0) { si = 0; return true; }
		else { ++si; return false; }
		return false;
	    } );
	mCPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack, mBlockSizes);
	++IB::mPos;
	return *this;
    }

    template <typename T, class... Indices>
    ContainerIndex<T,Indices...>& ContainerIndex<T,Indices...>::operator--()
    {
	if(mExternControl){
	    IB::mPos = sfor_m<sizeof...(Indices),0>
		( [&](auto i) { return std::get<i>(mIPack); },
		  [&](auto a, auto b) {return a->pos() + b*a->max();}, 0 );
	}
	sfor_m<sizeof...(Indices),0>
	    ( [&](auto i) {
		auto& si = *std::get<i>( mIPack );
		if(si.first() and i != 0) { si = si.max()-1; return true; }
		else { --si; return false; }
		return false;
	    } );
	mCPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack, mBlockSizes);
	--IB::mPos;
	return *this;

    }

    template <typename T, class... Indices>
    ContainerIndex<T,Indices...>& ContainerIndex<T,Indices...>::operator=(size_t pos)
    {
	IB::mPos = pos;
	RangeHelper::setIndexPack<sizeof...(Indices)-1>(mIPack, pos);
	mCPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack, mBlockSizes);
	return *this;
    }

    template <typename T, class... Indices>
    int ContainerIndex<T,Indices...>::pp(std::intptr_t idxPtrNum)
    {
	const int tmp = ppx<sizeof...(Indices)-1>(mIPack, mBlockSizes, idxPtrNum);
	IB::mPos += tmp;
	return tmp;
    }

    template <typename T, class... Indices>
    int ContainerIndex<T,Indices...>::mm(std::intptr_t idxPtrNum)
    {
	const int tmp = mmx<sizeof...(Indices)-1>(mIPack, mBlockSizes, idxPtrNum);
	IB::mPos -= tmp;
	return tmp;
    }

    template <typename T, class... Indices>
    std::string ContainerIndex<T,Indices...>::stringMeta() const
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr )->stringMeta(IB::mPos);
    }
    
    template <typename T, class... Indices>
    typename ContainerIndex<T,Indices...>::MetaType ContainerIndex<T,Indices...>::meta() const
    {
	MetaType metaTuple;
	sfor_pn<0,sizeof...(Indices)>
	    ( [&](auto i) { std::get<i>(metaTuple) = std::get<i>(mIPack)->meta(); return 0; } );
	return metaTuple;
    }

    template <typename T, class... Indices>
    ContainerIndex<T,Indices...>& ContainerIndex<T,Indices...>::at(const MetaType& metaPos)
    {
	sfor_pn<0,sizeof...(Indices)>
	    ( [&](auto i) { std::get<i>(mIPack)->at( std::get<i>(metaPos) ); return 0; } );
	IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack, mBlockSizes);
	return *this;
    }

    template <typename T, class... Indices>
    size_t ContainerIndex<T,Indices...>::dim() const
    {
	return sizeof...(Indices);
    }

    template <typename T, class... Indices>
    bool ContainerIndex<T,Indices...>::first() const
    {
	return IB::pos() == 0;
    }

    template <typename T, class... Indices>
    bool ContainerIndex<T,Indices...>::last() const
    {
	return IB::pos() == IB::mMax - 1;
    }

    template <typename T, class... Indices>
    bool ContainerIndex<T,Indices...>::sliceMode() const
    {
	return mNonTrivialBlocks;
    }

    template <typename T, class... Indices>
    std::shared_ptr<typename ContainerIndex<T,Indices...>::RangeType>
    ContainerIndex<T,Indices...>::range()
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr );
    }

    template <typename T, class... Indices>
    template <size_t N>
    auto ContainerIndex<T,Indices...>::getPtr() -> decltype( std::get<N>( mIPack ) )&
    {
	return std::get<N>( mIPack );
    }

    template <typename T, class... Indices>
    size_t ContainerIndex<T,Indices...>::getStepSize(size_t n)
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	return mBlockSizes[n+1];
    }

    template <typename T, class... Indices>
    template <class Exprs>
    auto ContainerIndex<T,Indices...>::ifor(size_t step, Exprs exs) const
    {
	return RangeHelper::mkFor<0>(step, mIPack, mBlockSizes, exs);
    }

    template <typename T, class... Indices>
    template <class Exprs>
    auto ContainerIndex<T,Indices...>::iforh(size_t step, Exprs exs) const
    {
	return RangeHelper::mkForh<0>(step, mIPack, mBlockSizes, exs);
    }

    template <typename T, class... Indices>
    template <class Exprs>
    auto ContainerIndex<T,Indices...>::pifor(size_t step, Exprs exs) const
    {
	return RangeHelper::mkPFor<0>(step, mIPack, mBlockSizes, exs);
    }

    template <typename T, class... Indices>
    std::intptr_t ContainerIndex<T,Indices...>::container() const
    {
	return mObjPtrNum;
    }

    template <typename T, class... Indices>
    ContainerIndex<T,Indices...>& ContainerIndex<T,Indices...>::
    format(const std::array<size_t,sizeof...(Indices)+1>& blocks)
    {
	mBlockSizes = blocks;
        mNonTrivialBlocks = true;
	return *this;
    }
    
    template <typename T, class... Indices>
    ContainerIndex<T,Indices...>& ContainerIndex<T,Indices...>::setData(const T* data)
    {
	mData = data;
	return *this;
    }

    template <typename T, class... Indices>
    const T& ContainerIndex<T,Indices...>::operator*() const
    {
	//return mMa[*this];
	return mData[mCPos];
	//return mData[IB::mPos];
    }

    template <typename T, class... Indices>
    const T* ContainerIndex<T,Indices...>::operator->() const
    {
	//return &mMa[*this];
	return &mData[mCPos];
    }
    /*
    template <typename T, class... Indices>
    T& ContainerIndex<T,Indices...>::operator*()
    {
	//return mMa[*this];
	return mData[IB::mPos];
    }

    template <typename T, class... Indices>
    T* ContainerIndex<T,Indices...>::operator->()
    {
	//return &mMa[*this];
	return &mData[IB::mPos];
    }
    */
    template <typename T, class... Indices>
    ContainerIndex<T,Indices...> ContainerIndex<T,Indices...>::operator++(int)
    {
	auto tmp = *this;
	++(*this);
	return tmp;
    }

    template <typename T, class... Indices>
    ContainerIndex<T,Indices...> ContainerIndex<T,Indices...>::operator--(int)
    {
	auto tmp = *this;
	--(*this);
	return tmp;
    }

    template <typename T, class... Indices>
    ContainerIndex<T,Indices...>& ContainerIndex<T,Indices...>::operator+=(int diff)
    {
	if(diff < 0){
	    for(int i = 0; i != diff; ++i){
		(*this)--;
	    }
	}
	else {
	    for(int i = 0; i != diff; ++i){
		(*this)++;
	    }
	}
	return *this;
    }

    template <typename T, class... Indices>
    ContainerIndex<T,Indices...>& ContainerIndex<T,Indices...>::operator-=(int diff)
    {
	if(diff < 0){
	    for(int i = 0; i != diff; ++i){
		(*this)++;
	    }
	}
	else {
	    for(int i = 0; i != diff; ++i){
		(*this)--;
	    }
	}
	return *this;
    }

    template <typename T, class... Indices>
    ContainerIndex<T,Indices...> ContainerIndex<T,Indices...>::operator+(int num) const
    {
	auto tmp = *this;
	return tmp += num;
    }

    template <typename T, class... Indices>
    ContainerIndex<T,Indices...> ContainerIndex<T,Indices...>::operator-(int num) const
    {
	auto tmp = *this;
	return tmp -= num;
    }

    template <typename T, class... Indices>
    int ContainerIndex<T,Indices...>::operator-(const ContainerIndex<T,Indices...>& it) const
    {
	return static_cast<int>( IB::mPos ) - static_cast<int>( it.pos() );
    }

    template <typename T, class... Indices>
    const T& ContainerIndex<T,Indices...>::operator[](int num) const
    {
	return mData[IB::mPos + num];
    }

    template <typename T, class... Indices>
    bool ContainerIndex<T,Indices...>::operator<(const ContainerIndex<T,Indices...>& it) const
    {
	return IB::mPos < it.pos();
    }

    template <typename T, class... Indices>
    bool ContainerIndex<T,Indices...>::operator>(const ContainerIndex<T,Indices...>& it) const
    {
	return IB::mPos > it.pos();
    }

    template <typename T, class... Indices>
    bool ContainerIndex<T,Indices...>::operator<=(const ContainerIndex<T,Indices...>& it) const
    {
	return IB::mPos <= it.pos();
    }

    template <typename T, class... Indices>
    bool ContainerIndex<T,Indices...>::operator>=(const ContainerIndex<T,Indices...>& it) const
    {
	return IB::mPos >= it.pos();
    }

} // end namespace MultiArrayTools


#endif
