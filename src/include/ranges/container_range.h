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
	
    private:
	
	ContainerIndex() = default;
	
	bool mNonTrivialBlocks = false;
	bool mExternControl = false;
	IndexPack mIPack;
	std::array<size_t,sizeof...(Indices)+1> mBlockSizes; 
	const T* mData = nullptr;
	//const MultiArrayBase<T,typename Indices::RangeType...>* mMa = nullptr;
	std::intptr_t mObjPtrNum;
	
    public:

	ContainerIndex(const ContainerIndex& in) = default;
	ContainerIndex& operator=(const ContainerIndex& in) = default;
	
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

	const IndexPack& pack() const { return mIPack; }
	
	ContainerIndex& sync(); // recalculate 'IB::mPos' when externalControl == true
	ContainerIndex& operator()(const std::shared_ptr<Indices>&... inds); // control via external indices
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

	std::string id() const;
	void print(size_t offset);
	
	template <class Exprs>
	auto ifor(Exprs exs) const
	    -> decltype(RPackNum<sizeof...(Indices)-1>::mkFor(mIPack, exs));

	template <class Exprs>
	auto iforh(Exprs exs) const
	    -> decltype(RPackNum<sizeof...(Indices)-1>::mkForh(mIPack, exs));

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

    /*
    template <typename T, class... Ranges>
    class ContainerRangeFactory : public RangeFactoryBase
    {
    public:

	typedef ContainerRange<T,Ranges...> oType;
	
	ContainerRangeFactory();
	ContainerRangeFactory(const std::shared_ptr<Ranges>&... rs);
	ContainerRangeFactory(const typename ContainerRange<T,Ranges...>::SpaceType& space);
	
	virtual std::shared_ptr<RangeBase> create() override; 
	
    protected:
	
    };*/
    /*
    template <typename T, class... Ranges>
    class ContainerRange : public RangeInterface<ContainerIndex<T,typename Ranges::IndexType...> >
    {
    public:

	typedef RangeBase RB;
	typedef std::tuple<std::shared_ptr<Ranges>...> SpaceType;
	typedef ContainerIndex<T,typename Ranges::IndexType...> IndexType;
	
    protected:
	ContainerRange() = default;
	ContainerRange(const ContainerRange& in) = delete;
	ContainerRange& operator=(const ContainerRange& in) = delete;
	
	ContainerRange(const std::shared_ptr<Ranges>&... rs);
	ContainerRange(const SpaceType& space);

	SpaceType mSpace;	

    public:	
	static const size_t sdim = sizeof...(Ranges);

	virtual size_t dim() const override;
	virtual size_t size() const override;

	template <size_t N>
	auto get() const -> decltype( *std::get<N>( mSpace ) )&;

	template <size_t N>
	auto getPtr() const -> decltype( std::get<N>( mSpace ) )&;

	const SpaceType& space() const;
	
	virtual IndexType begin() const override;
	virtual IndexType end() const override;

	friend ContainerRangeFactory<T,Ranges...>;

	static constexpr bool defaultable = false;
	static constexpr size_t ISSTATIC = SubProp<Ranges...>::ISSTATIC;
	static constexpr size_t SIZE = SubProp<Ranges...>::SIZE;
    };
    */

    //template <typename T, class... Ranges>
    //using ContainerRange = MultiRange<Ranges...>;
    
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
	RPackNum<sizeof...(Indices)-1>::construct(mIPack, *range);
	std::get<sizeof...(Indices)>(mBlockSizes) = 1;
	RPackNum<sizeof...(Indices)-1>::initBlockSizes(mBlockSizes, mIPack);
	IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack, mBlockSizes);
    }

    template <typename T, class... Indices>
    template <class MRange>
    ContainerIndex<T,Indices...>::ContainerIndex(const std::shared_ptr<MRange>& range,
						 std::intptr_t objPtrNum,
						 const std::array<size_t,sizeof...(Indices)+1>& blockSizes) :
	IndexInterface<ContainerIndex<T,Indices...>,std::tuple<typename Indices::MetaType...> >(range, 0),
	mObjPtrNum(objPtrNum)
    {
	RPackNum<sizeof...(Indices)-1>::construct(mIPack, *range);
	mBlockSizes = blockSizes;
	IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack, mBlockSizes);
	mNonTrivialBlocks = true;
    }
    
    
    template <typename T, class... Indices>
    ContainerIndex<T,Indices...>& ContainerIndex<T,Indices...>::sync()
    {
	if(mExternControl){
	    IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack);
	    //VCHECK(id());
	    //VCHECK(sizeof...(Indices));
	    //assert(IB::mPos < IB::max());
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
	RPackNum<sizeof...(Indices)-1>::swapIndices(mIPack, inds...);
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
	    IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack);
	}
	RPackNum<sizeof...(Indices)-1>::pp( mIPack );
	++IB::mPos;
	return *this;
    }

    template <typename T, class... Indices>
    ContainerIndex<T,Indices...>& ContainerIndex<T,Indices...>::operator--()
    {
	if(mExternControl){
	    IB::mPos = RPackNum<sizeof...(Indices)-1>::makePos(mIPack);
	}
	RPackNum<sizeof...(Indices)-1>::mm( mIPack );
	--IB::mPos;
	return *this;

    }

    template <typename T, class... Indices>
    ContainerIndex<T,Indices...>& ContainerIndex<T,Indices...>::operator=(size_t pos)
    {
	IB::mPos = pos;
	RPackNum<sizeof...(Indices)-1>::setIndexPack(mIPack, pos);
	return *this;
    }

    template <typename T, class... Indices>
    int ContainerIndex<T,Indices...>::pp(std::intptr_t idxPtrNum)
    {
	int tmp = RPackNum<sizeof...(Indices)-1>::pp(mIPack, mBlockSizes, idxPtrNum);
	IB::mPos += tmp;
	return tmp;
    }

    template <typename T, class... Indices>
    int ContainerIndex<T,Indices...>::mm(std::intptr_t idxPtrNum)
    {
	int tmp = RPackNum<sizeof...(Indices)-1>::mm(mIPack, mBlockSizes, idxPtrNum);
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
	RPackNum<sizeof...(Indices)-1>::getMetaPos(metaTuple, mIPack);
	return metaTuple;
    }

    template <typename T, class... Indices>
    ContainerIndex<T,Indices...>& ContainerIndex<T,Indices...>::at(const MetaType& metaPos)
    {
	RPackNum<sizeof...(Indices)-1>::setMeta(mIPack, metaPos);
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
    std::string ContainerIndex<T,Indices...>::id() const
    {
	return std::string("con") + std::to_string(IB::mId);
    }

    template <typename T, class... Indices>
    void ContainerIndex<T,Indices...>::print(size_t offset)
    {
	if(offset == 0){
	    std::cout << " === " << std::endl;
	}
	for(size_t j = 0; j != offset; ++j) { std::cout << "\t"; }
	std::cout << id() << "[" << reinterpret_cast<std::intptr_t>(this) << "]"
		  << "(" << IB::mRangePtr << "): " << meta() << std::endl;
	RPackNum<sizeof...(Indices)-1>::printIndex(mIPack, offset+1);
    }

    template <typename T, class... Indices>
    template <class Exprs>
    auto ContainerIndex<T,Indices...>::ifor(Exprs exs) const
	-> decltype(RPackNum<sizeof...(Indices)-1>::mkFor(mIPack, exs))
    {
	return RPackNum<sizeof...(Indices)-1>::mkFor(mIPack, exs);
    }

    template <typename T, class... Indices>
    template <class Exprs>
    auto ContainerIndex<T,Indices...>::iforh(Exprs exs) const
	-> decltype(RPackNum<sizeof...(Indices)-1>::mkForh(mIPack, exs))
    {
	return RPackNum<sizeof...(Indices)-1>::mkForh(mIPack, exs);
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
	return mData[IB::mPos];
    }

    template <typename T, class... Indices>
    const T* ContainerIndex<T,Indices...>::operator->() const
    {
	//return &mMa[*this];
	return &mData[IB::mPos];
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

    /*****************************
     *   ContainerRangeFactory   *
     *****************************/
    /*
    template <typename T, class... Ranges>
    ContainerRangeFactory<T,Ranges...>::ContainerRangeFactory(const std::shared_ptr<Ranges>&... rs)
    {
	mProd = std::shared_ptr<ContainerRange<T,Ranges...> >( new ContainerRange<T,Ranges...>( rs... ) );
    }
   
    template <typename T, class... Ranges>
    ContainerRangeFactory<T,Ranges...>::
    ContainerRangeFactory(const typename ContainerRange<T,Ranges...>::SpaceType& space)
    {
	mProd = std::shared_ptr<ContainerRange<T,Ranges...> >( new ContainerRange<T,Ranges...>( space ) );
    }

    template <typename T, class... Ranges>
    std::shared_ptr<RangeBase> ContainerRangeFactory<T,Ranges...>::create()
    {
	setSelf();
	return mProd;
    }
    */
    /**********************
     *   ContainerRange   *
     **********************/
    /*
    template <typename T, class... Ranges>
    ContainerRange<T,Ranges...>::ContainerRange(const std::shared_ptr<Ranges>&... rs) :
	mSpace( std::make_tuple( rs... ) ) {}

    template <typename T, class... Ranges>
    ContainerRange<T,Ranges...>::ContainerRange(const SpaceType& space) : mSpace( space ) {}

    template <typename T, class... Ranges>
    size_t ContainerRange<T,Ranges...>::dim() const
    {
	return sizeof...(Ranges);
    }

    template <typename T, class... Ranges>
    size_t ContainerRange<T,Ranges...>::size() const
    {
	return RPackNum<sizeof...(Ranges)-1>::getSize(mSpace);
    }

    template <typename T, class... Ranges>
    template <size_t N>
    auto ContainerRange<T,Ranges...>::get() const -> decltype( *std::get<N>( mSpace ) )&
    {
	return *std::get<N>( mSpace );
    }

    template <typename T, class... Ranges>
    template <size_t N>
    auto ContainerRange<T,Ranges...>::getPtr() const -> decltype( std::get<N>( mSpace ) )&
    {
	return std::get<N>( mSpace );
    }

    template <typename T, class... Ranges>
    const typename ContainerRange<T,Ranges...>::SpaceType& ContainerRange<T,Ranges...>::space() const
    {
	return mSpace;
    }
        
    template <typename T, class... Ranges>
    typename ContainerRange<T,Ranges...>::IndexType ContainerRange<T,Ranges...>::begin() const
    {
	ContainerIndex<T,typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<ContainerRange<T,Ranges...> >
	       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = 0;
	return i;
    }

    template <typename T, class... Ranges>
    typename ContainerRange<T,Ranges...>::IndexType ContainerRange<T,Ranges...>::end() const
    {
	ContainerIndex<T,typename Ranges::IndexType...>
	    i( std::dynamic_pointer_cast<ContainerRange<T,Ranges...> >
	       ( std::shared_ptr<RangeBase>( RB::mThis ) ) );
	i = size();
	return i;
    }
    */
} // end namespace MultiArrayTools


#endif
