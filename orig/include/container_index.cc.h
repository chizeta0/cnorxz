
#include "container_index.h"

namespace CNORXZ
{
    
    template <typename T, class... Indices>
    ConstContainerIndex<T,Indices...>::ConstContainerIndex(const ConstContainerIndex& in, bool copy) :
	IB(in),
	mNonTrivialBlocks(in.mNonTrivialBlocks),
	mExternControl(false),
	mBlockSizes(in.mBlockSizes),
	mData(in.mData),
	mObjPtrNum(in.mObjPtrNum),
	mCPos(in.mCPos)
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
	
    template <typename T, class... Indices>
    ConstContainerIndex<T,Indices...>& ConstContainerIndex<T,Indices...>::copy(const ConstContainerIndex& in)
    {
	IB::operator=(in);
	mNonTrivialBlocks = in.mNonTrivialBlocks;
	mExternControl = false;
	mBlockSizes = in.mBlockSizes;
	mData = in.mData;
	mObjPtrNum = in.mObjPtrNum;
	mCPos = in.mCPos;
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

    template <typename T, class... Indices>
    template <class MRange>
    ConstContainerIndex<T,Indices...>::ConstContainerIndex(const std::shared_ptr<MRange>& range,
						 std::intptr_t objPtrNum) :
	IndexInterface<ConstContainerIndex<T,Indices...>,std::tuple<typename Indices::MetaType...> >(range, 0),
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
	mCPos = RangeHelper::makePos<sizeof...(Indices)-1>(mIPack, mBlockSizes);
    }

    template <typename T, class... Indices>
    template <class MRange>
    ConstContainerIndex<T,Indices...>::ConstContainerIndex(const std::shared_ptr<MRange>& range,
						 std::intptr_t objPtrNum,
						 const std::array<size_t,sizeof...(Indices)+1>& blockSizes) :
	IndexInterface<ConstContainerIndex<T,Indices...>,std::tuple<typename Indices::MetaType...> >(range, 0),
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
	mCPos = RangeHelper::makePos<sizeof...(Indices)-1>(mIPack, mBlockSizes);
	mNonTrivialBlocks = true;
    }

    template <typename T, class... Indices>
    template <typename X>
    ConstContainerIndex<T,Indices...>&
    ConstContainerIndex<T,Indices...>::operator=(const ConstContainerIndex<X,Indices...>& in)
    {
	mIPack = in.mIPack;
	return (*this)();
    }

    
    template <typename T, class... Indices>
    ConstContainerIndex<T,Indices...>& ConstContainerIndex<T,Indices...>::sync()
    {
	if(mExternControl){
	    IB::mPos = sfor_m<sizeof...(Indices),0>
		( [&](auto i) { return std::get<i>(mIPack); },
		  [&](auto a, auto b) {return a->pos() + b*a->max();}, 0 );
	    	mCPos = RangeHelper::makePos<sizeof...(Indices)-1>(mIPack, mBlockSizes);
	}
	return *this;
    }
    
    template <typename T, class... Indices>
    template <size_t N>
    auto& ConstContainerIndex<T,Indices...>::get() const
    {
	return *std::get<N>( mIPack );
    }

    template <typename T, class... Indices>
    template <size_t N>
    auto ConstContainerIndex<T,Indices...>::getPtr() const
    {
	return std::get<N>( mIPack );
    }

    template <typename T, class... Indices>
    ConstContainerIndex<T,Indices...>& ConstContainerIndex<T,Indices...>::operator()(const std::shared_ptr<Indices>&... inds)
    {
	return (*this)(std::make_tuple(inds...));
    }    

    template <typename T, class... Indices>
    ConstContainerIndex<T,Indices...>& ConstContainerIndex<T,Indices...>::operator()(const std::tuple<std::shared_ptr<Indices>...>& inds)
    {
	sfor_pn<0,sizeof...(Indices)>
	    ( [&](auto i) { std::get<i>(mIPack) = std::get<i>(inds); return 0; } );
	mExternControl = true;
	return sync();
    }    

    template <typename T, class... Indices>
    ConstContainerIndex<T,Indices...>& ConstContainerIndex<T,Indices...>::operator()()
    {
	return sync();
    }

    template <typename T, class... Indices>
    IndexType ConstContainerIndex<T,Indices...>::type() const { return IndexType::CONT; }

    template <typename T, class... Indices>
    ConstContainerIndex<T,Indices...>& ConstContainerIndex<T,Indices...>::operator++()
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
	mCPos = RangeHelper::makePos<sizeof...(Indices)-1>(mIPack, mBlockSizes);
	++IB::mPos;
	return *this;
    }

    template <typename T, class... Indices>
    ConstContainerIndex<T,Indices...>& ConstContainerIndex<T,Indices...>::operator--()
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
	mCPos = RangeHelper::makePos<sizeof...(Indices)-1>(mIPack, mBlockSizes);
	--IB::mPos;
	return *this;

    }

    template <typename T, class... Indices>
    ConstContainerIndex<T,Indices...>& ConstContainerIndex<T,Indices...>::operator=(size_t pos)
    {
	IB::mPos = pos;
	RangeHelper::setIndexPack<sizeof...(Indices)-1>(mIPack, pos);
	mCPos = RangeHelper::makePos<sizeof...(Indices)-1>(mIPack, mBlockSizes);
	return *this;
    }

    template <typename T, class... Indices>
    int ConstContainerIndex<T,Indices...>::pp(std::intptr_t idxPtrNum)
    {
	const int tmp = RangeHelper::ppx<sizeof...(Indices)-1>(mIPack, mBlockSizes, idxPtrNum);
	IB::mPos += tmp;
	return tmp;
    }

    template <typename T, class... Indices>
    int ConstContainerIndex<T,Indices...>::mm(std::intptr_t idxPtrNum)
    {
	const int tmp = RangeHelper::mmx<sizeof...(Indices)-1>(mIPack, mBlockSizes, idxPtrNum);
	IB::mPos -= tmp;
	return tmp;
    }

    template <typename T, class... Indices>
    std::string ConstContainerIndex<T,Indices...>::stringMeta() const
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr )->stringMeta(IB::mPos);
    }
    
    template <typename T, class... Indices>
    typename ConstContainerIndex<T,Indices...>::MetaType ConstContainerIndex<T,Indices...>::meta() const
    {
	MetaType metaTuple;
	sfor_pn<0,sizeof...(Indices)>
	    ( [&](auto i) { std::get<i>(metaTuple) = std::get<i>(mIPack)->meta(); return 0; } );
	return metaTuple;
    }

    template <typename T, class... Indices>
    ConstContainerIndex<T,Indices...>& ConstContainerIndex<T,Indices...>::at(const MetaType& metaPos)
    {
	sfor_pn<0,sizeof...(Indices)>
	    ( [&](auto i) { std::get<i>(mIPack)->at( std::get<i>(metaPos) ); return 0; } );
	IB::mPos = RangeHelper::makePos<sizeof...(Indices)-1>(mIPack, mBlockSizes);
	return *this;
    }

    template <typename T, class... Indices>
    size_t ConstContainerIndex<T,Indices...>::dim() const
    {
	return sizeof...(Indices);
    }

    template <typename T, class... Indices>
    bool ConstContainerIndex<T,Indices...>::first() const
    {
	return IB::pos() == 0;
    }

    template <typename T, class... Indices>
    bool ConstContainerIndex<T,Indices...>::last() const
    {
	return IB::pos() == IB::mMax - 1;
    }

    template <typename T, class... Indices>
    bool ConstContainerIndex<T,Indices...>::sliceMode() const
    {
	return mNonTrivialBlocks;
    }

    template <typename T, class... Indices>
    std::shared_ptr<typename ConstContainerIndex<T,Indices...>::RangeType>
    ConstContainerIndex<T,Indices...>::range()
    {
	return std::dynamic_pointer_cast<RangeType>( IB::mRangePtr );
    }

    template <typename T, class... Indices>
    size_t ConstContainerIndex<T,Indices...>::getStepSize(size_t n)
    {
	if(n >= sizeof...(Indices)){
	    assert(0);
	    // throw !!
	}
	return mBlockSizes[n+1];
    }

    template <typename T, class... Indices>
    template <class Exprs>
    auto ConstContainerIndex<T,Indices...>::ifor(size_t step, Exprs exs) const
    {
	return RangeHelper::mkFor<0>(step, mIPack, mBlockSizes, exs);
    }

    template <typename T, class... Indices>
    template <class Exprs>
    auto ConstContainerIndex<T,Indices...>::iforh(size_t step, Exprs exs) const
    {
	return RangeHelper::mkForh<0>(step, mIPack, mBlockSizes, exs);
    }

    template <typename T, class... Indices>
    template <class Exprs>
    auto ConstContainerIndex<T,Indices...>::pifor(size_t step, Exprs exs) const
    {
	return RangeHelper::mkPFor<0>(step, mIPack, mBlockSizes, exs);
    }

    template <typename T, class... Indices>
    std::intptr_t ConstContainerIndex<T,Indices...>::container() const
    {
	return mObjPtrNum;
    }

    template <typename T, class... Indices>
    ConstContainerIndex<T,Indices...>& ConstContainerIndex<T,Indices...>::
    format(const std::array<size_t,sizeof...(Indices)+1>& blocks)
    {
	mBlockSizes = blocks;
        mNonTrivialBlocks = true;
	return *this;
    }
    
    template <typename T, class... Indices>
    ConstContainerIndex<T,Indices...>& ConstContainerIndex<T,Indices...>::setData(const T* data)
    {
	mData = data;
	return *this;
    }

    template <typename T, class... Indices>
    const T& ConstContainerIndex<T,Indices...>::operator*() const
    {
	return mData[mCPos];
    }

    template <typename T, class... Indices>
    const T* ConstContainerIndex<T,Indices...>::operator->() const
    {
	return &mData[mCPos];
    }

    template <typename T, class... Indices>
    ConstContainerIndex<T,Indices...> ConstContainerIndex<T,Indices...>::operator++(int)
    {
	auto tmp = *this;
	++(*this);
	return tmp;
    }

    template <typename T, class... Indices>
    ConstContainerIndex<T,Indices...> ConstContainerIndex<T,Indices...>::operator--(int)
    {
	auto tmp = *this;
	--(*this);
	return tmp;
    }

    template <typename T, class... Indices>
    ConstContainerIndex<T,Indices...>& ConstContainerIndex<T,Indices...>::operator+=(int diff)
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
    ConstContainerIndex<T,Indices...>& ConstContainerIndex<T,Indices...>::operator-=(int diff)
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
    ConstContainerIndex<T,Indices...> ConstContainerIndex<T,Indices...>::operator+(int num) const
    {
	auto tmp = *this;
	return tmp += num;
    }

    template <typename T, class... Indices>
    ConstContainerIndex<T,Indices...> ConstContainerIndex<T,Indices...>::operator-(int num) const
    {
	auto tmp = *this;
	return tmp -= num;
    }

    template <typename T, class... Indices>
    int ConstContainerIndex<T,Indices...>::operator-(const ConstContainerIndex<T,Indices...>& it) const
    {
	return static_cast<int>( IB::mPos ) - static_cast<int>( it.pos() );
    }

    template <typename T, class... Indices>
    const T& ConstContainerIndex<T,Indices...>::operator[](int num) const
    {
	return mData[IB::mPos + num];
    }

    template <typename T, class... Indices>
    bool ConstContainerIndex<T,Indices...>::operator<(const ConstContainerIndex<T,Indices...>& it) const
    {
	return IB::mPos < it.pos();
    }

    template <typename T, class... Indices>
    bool ConstContainerIndex<T,Indices...>::operator>(const ConstContainerIndex<T,Indices...>& it) const
    {
	return IB::mPos > it.pos();
    }

    template <typename T, class... Indices>
    bool ConstContainerIndex<T,Indices...>::operator<=(const ConstContainerIndex<T,Indices...>& it) const
    {
	return IB::mPos <= it.pos();
    }

    template <typename T, class... Indices>
    bool ConstContainerIndex<T,Indices...>::operator>=(const ConstContainerIndex<T,Indices...>& it) const
    {
	return IB::mPos >= it.pos();
    }

} // namespace CNORXZ
