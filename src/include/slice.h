
#ifndef __slice_h__
#define __slice_h__

#include "multi_array_base.h"
#include "multi_array_operation.h"

namespace MultiArrayTools
{
    template <typename T, class... SRanges>
    class ConstSlice : public MultiArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<T,SRanges...> CRange;
	typedef MultiArrayBase<T,SRanges...> MAB;
	typedef ContainerIndex<T,typename SRanges::IndexType...> IType;

	using MultiArrayBase<T,SRanges...>::operator();
	
	DEFAULT_MEMBERS(ConstSlice);

	ConstSlice(const std::shared_ptr<SRanges>&... ranges, const T* data = nullptr);
	ConstSlice(const MultiArrayBase<T,AnonymousRange>& ma, SIZET<SRanges>... sizes);
	
	virtual const T& operator[](const IType& i) const final;
	virtual const T& at(const typename IType::MetaType& meta) const override;
	
	virtual const T* data() const override;
	
	virtual bool isSlice() const override;

	virtual auto begin() const -> IType override;
	virtual auto end() const -> IType override;

	virtual std::shared_ptr<MultiArrayBase<T,AnonymousRange> > anonymous(bool slice = false) const override;
	
	auto define(const std::shared_ptr<typename SRanges::IndexType>&... inds)
	    -> ConstSliceDef<T,SRanges...>;
	
    private:
	friend ConstSliceDef<T,SRanges...>;

	void format(const std::array<size_t,sizeof...(SRanges)+1>& blocks);
	
	const T* mData;
    };

    
    template <typename T, class... SRanges>
    class Slice : public MutableMultiArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<T,SRanges...> CRange;
	typedef MultiArrayBase<T,SRanges...> MAB;
	typedef ContainerIndex<T,typename SRanges::IndexType...> IType;

	using MultiArrayBase<T,SRanges...>::operator();
	using MutableMultiArrayBase<T,SRanges...>::operator();
	
	DEFAULT_MEMBERS(Slice);

	Slice(const std::shared_ptr<SRanges>&... ranges, T* data = nullptr);
	    
	virtual const T& operator[](const IType& i) const final;
	virtual T& operator[](const IType& i) final;
	virtual const T& at(const typename IType::MetaType& meta) const override;
	virtual T& at(const typename IType::MetaType& meta) override;

	virtual const T* data() const override;
	virtual T* data() override;

	virtual bool isSlice() const override;

	virtual auto begin() const -> IType override;
	virtual auto end() const -> IType override;

	virtual std::shared_ptr<MultiArrayBase<T,AnonymousRange> > anonymous(bool slice = false) const override;
	virtual std::shared_ptr<MultiArrayBase<T,AnonymousRange> > anonymousMove() override;

	auto define(const std::shared_ptr<typename SRanges::IndexType>&... inds)
	    -> SliceDef<T,SRanges...>;
	
    private:
	friend SliceDef<T,SRanges...>;

	void format(const std::array<size_t,sizeof...(SRanges)+1>& blocks);
	
	T* mData;
    };


    template <typename T, class... SRanges>
    class SliceDef
    {
    public:
	typedef ContainerIndex<T,typename SRanges::IndexType...> IType;

	template <class Op>
	static Slice<T,SRanges...> mkSlice( const typename Slice<T,SRanges...>::IndexType& ind,
						 const Op& op )
	{
	    Slice<T,SRanges...> out(ind->range()->space(), &*ind);
	    std::array<size_t,sizeof...(SRanges)+1> ff;
	    for(size_t i = 0; i != sizeof...(SRanges)+1; ++i){
		PackNum<sizeof...(SRanges)-1>::mkSliceBlocks(ff, ind, op);
	    }
	    out.format(ff);
	    return out;
	}
	
    private:
	IType mIndex;
	Slice<T,SRanges...>& mSl;
	
	SliceDef() = default;
    public:

	SliceDef(Slice<T,SRanges...>& sl,
		 const std::shared_ptr<typename SRanges::IndexType>&... inds);

	template <class... ORanges>
	SliceDef& operator=(const OperationRoot<T,ORanges...>& op);
    };
    
    template <typename T, class... SRanges>
    class ConstSliceDef
    {
    public:
	typedef ContainerIndex<T,typename SRanges::IndexType...> IType;

	template <class Op>
	static ConstSlice<T,SRanges...> mkSlice( const typename ConstSlice<T,SRanges...>::IndexType& ind,
						 const Op& op )
	{
	    ConstSlice<T,SRanges...> out(ind->range()->space(), &*ind);
	    std::array<size_t,sizeof...(SRanges)+1> ff;
	    for(size_t i = 0; i != sizeof...(SRanges)+1; ++i){
		PackNum<sizeof...(SRanges)-1>::mkSliceBlocks(ff, ind, op);
	    }
	    out.format(ff);
	    return out;
	}
	
    private:
	IType mIndex;
	ConstSlice<T,SRanges...>& mSl;
	
	ConstSliceDef() = default;
    public:
	ConstSliceDef(ConstSlice<T,SRanges...>& csl,
		      const std::shared_ptr<typename SRanges::IndexType>&... inds);

	template <class... ORanges>
	ConstSliceDef& operator=(const ConstOperationRoot<T,ORanges...>& op);

    	template <class... ORanges>
	ConstSliceDef& operator=(const OperationRoot<T,ORanges...>& op);
    };

    template <typename T, class Op, class... Ranges>
    ConstSlice<T,Ranges...> mkSlice( const typename ConstSlice<T,Ranges...>::IndexType& ind,
				     const Op& op )
    {
	return ConstSliceDef<T,Ranges...>::mkSlice(ind, op);
    }

    template <typename T, class Op, class... Ranges>
    Slice<T,Ranges...> mkSlice( const typename Slice<T,Ranges...>::IndexType& ind,
				const Op& op )
    {
	return SliceDef<T,Ranges...>::mkSlice(ind, op);
    }

} // end namespace MultiArrayTools

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{

    /*******************
     *   ConstSlice    *
     *******************/

    template <typename T, class... SRanges>
    void ConstSlice<T,SRanges...>::format(const std::array<size_t,sizeof...(SRanges)+1>& blocks)
    {
	MAB::mProtoI->format(blocks);
    }
    
    template <typename T, class... SRanges>
    ConstSlice<T,SRanges...>::ConstSlice(const std::shared_ptr<SRanges>&... ranges, const T* data) :
	MultiArrayBase<T,SRanges...>(ranges...),
	mData(data)
    {
	MAB::mInit = true;
    }

    template <typename T, class... SRanges>
    ConstSlice<T,SRanges...>::ConstSlice(const MultiArrayBase<T,AnonymousRange>& ma, SIZET<SRanges>... sizes) :
	MultiArrayBase<T,SRanges...>
	( ma.range()->template get<0>().template scast<SRanges...>(sizes...)->space() ),
	mData( ma.data() )
    {
	MAB::mInit = true;
    }
    
    template <typename T, class... SRanges>
    const T& ConstSlice<T,SRanges...>::operator[](const IType& i) const
    {
	//assert(i.sliceMode()); // -> compare objects !!!!!
	assert(i.container() == reinterpret_cast<std::intptr_t>(this));
	return mData[ i.pos() ];
    }
    
    template <typename T, class... SRanges>
    const T& ConstSlice<T,SRanges...>::at(const typename IType::MetaType& meta) const
    {
	//auto x = begin().at(meta);
	//VCHECK(x.pos());
	return mData[ begin().at(meta).pos() ];
    }
    
    template <typename T, class... SRanges>
    const T* ConstSlice<T,SRanges...>::data() const
    {
	return mData;
    }

    template <typename T, class... SRanges>
    bool ConstSlice<T,SRanges...>::isSlice() const
    {
	return true;
    }

    template <typename T, class... SRanges>
    auto ConstSlice<T,SRanges...>::begin() const -> ConstSlice<T,SRanges...>::IType 
    {
	IType i(*MAB::mProtoI);
	i = 0;
	//i = mStartPos;
	return i.setData(data());
    }

    template <typename T, class... SRanges>
    auto ConstSlice<T,SRanges...>::end() const -> ConstSlice<T,SRanges...>::IType 
    {
	IType i(*MAB::mProtoI);
	i = i.max(); // CHECK !!!
	//i = std::get<sizeof...(SRanges)>(mBlockSizes);
	return i.setData(data());
    }

    template <typename T, class... SRanges>
    std::shared_ptr<MultiArrayBase<T,AnonymousRange> > ConstSlice<T,SRanges...>::anonymous(bool slice) const
    {
	assert(0); // think about carefully!!!!
	return nullptr;
    }
    
    template <typename T, class... SRanges>
    auto ConstSlice<T,SRanges...>::define(const std::shared_ptr<typename SRanges::IndexType>&... inds)
	-> ConstSliceDef<T,SRanges...>
    {
	return ConstSliceDef<T,SRanges...>(*this, inds...);
    }

    
    /**************
     *   Slice    *
     **************/

    template <typename T, class... SRanges>
    void Slice<T,SRanges...>::format(const std::array<size_t,sizeof...(SRanges)+1>& blocks)
    {
	MAB::mProtoI->format(blocks);
    }
    
    template <typename T, class... SRanges>
    Slice<T,SRanges...>::Slice(const std::shared_ptr<SRanges>&... ranges, T* data) :
	MutableMultiArrayBase<T,SRanges...>(ranges...),
	mData(data) {}
    
    template <typename T, class... SRanges>
    const T& Slice<T,SRanges...>::operator[](const IType& i) const
    {
	//assert(i.sliceMode()); // -> compare objects !!!!!
	assert(i.container() == reinterpret_cast<std::intptr_t>(this));
	return mData[ i.pos() ];
    }
    
    template <typename T, class... SRanges>
    T& Slice<T,SRanges...>::operator[](const IType& i)
    {
	//assert(i.sliceMode());
	assert(i.container() == reinterpret_cast<std::intptr_t>(this));
	return mData[ i.pos() ];
    }
    
    template <typename T, class... SRanges>
    const T& Slice<T,SRanges...>::at(const typename IType::MetaType& meta) const
    {
	//auto x = begin().at(meta);
	//VCHECK(x.pos());
	return mData[ begin().at(meta).pos() ];
    }
    
    template <typename T, class... SRanges>
    T& Slice<T,SRanges...>::at(const typename IType::MetaType& meta)
    {
	//auto x = begin().at(meta);
	//VCHECK(x.pos());
	return mData[ begin().at(meta).pos() ];
    }
    
    template <typename T, class... SRanges>
    const T* Slice<T,SRanges...>::data() const
    {
	return mData;
    }

    template <typename T, class... SRanges>
    T* Slice<T,SRanges...>::data()
    {
	return mData;
    }

    template <typename T, class... SRanges>
    bool Slice<T,SRanges...>::isSlice() const
    {
	return true;
    }

    template <typename T, class... SRanges>
    auto Slice<T,SRanges...>::begin() const -> Slice<T,SRanges...>::IType 
    {
	IType i(*MAB::mProtoI);
	i = 0;
	//i = mStartPos;
	return i.setData(data());
    }

    template <typename T, class... SRanges>
    auto Slice<T,SRanges...>::end() const -> Slice<T,SRanges...>::IType 
    {
	IType i(*MAB::mProtoI);
	i = i.max(); // CHECK !!!
	//i = std::get<sizeof...(SRanges)>(mBlockSizes);
	return i.setData(data());
    }

    template <typename T, class... SRanges>
    std::shared_ptr<MultiArrayBase<T,AnonymousRange> > Slice<T,SRanges...>::anonymous(bool slice) const
    {
	assert(0); // think about carefully!!!!
	return nullptr;
    }
    
    template <typename T, class... SRanges>
    std::shared_ptr<MultiArrayBase<T,AnonymousRange> > Slice<T,SRanges...>::anonymousMove()
    {
	assert(0); // think about carefully!!!!
	return nullptr;
    }
    
    template <typename T, class... SRanges>
    auto Slice<T,SRanges...>::define(const std::shared_ptr<typename SRanges::IndexType>&... inds)
	-> SliceDef<T,SRanges...>
    {
	return SliceDef<T,SRanges...>(*this, inds...);
    }
    

    template <typename T, class... SRanges>
    SliceDef<T,SRanges...>::SliceDef(Slice<T,SRanges...>& sl,
				     const std::shared_ptr<typename SRanges::IndexType>&... inds) :
	mIndex(sl.begin()),
	mSl(sl)
    {
	mIndex(inds...);
    }

    template <typename T, class... SRanges>
    template <class... ORanges>
    SliceDef<T,SRanges...>& SliceDef<T,SRanges...>::operator=(const OperationRoot<T,ORanges...>& op)
    {
	std::array<size_t,sizeof...(SRanges)+1> blocks;
	PackNum<sizeof...(SRanges)-1>::
	    template mkSliceBlocks<T,OperationRoot<T,ORanges...>,SRanges...>(blocks, mIndex, op);
	mSl.format(blocks);
	mSl.mData = op.data();
	return *this;
    }

    template <typename T, class... SRanges>
    ConstSliceDef<T,SRanges...>::ConstSliceDef(ConstSlice<T,SRanges...>& sl,
					       const std::shared_ptr<typename SRanges::IndexType>&... inds) :
	mIndex(sl.begin()),
	mSl(sl)
    {
	mIndex(inds...);
    }

    template <typename T, class... SRanges>
    template <class... ORanges>
    ConstSliceDef<T,SRanges...>& ConstSliceDef<T,SRanges...>::operator=(const ConstOperationRoot<T,ORanges...>& op)
    {
	std::array<size_t,sizeof...(SRanges)+1> blocks;
	PackNum<sizeof...(SRanges)-1>::
	    template mkSliceBlocks<T,ConstOperationRoot<T,ORanges...>,SRanges...>(blocks, mIndex, op);
	mSl.format(blocks);
	mSl.mData = op.data();
	return *this;
    }

    template <typename T, class... SRanges>
    template <class... ORanges>
    ConstSliceDef<T,SRanges...>& ConstSliceDef<T,SRanges...>::operator=(const OperationRoot<T,ORanges...>& op)
    {
	std::array<size_t,sizeof...(SRanges)+1> blocks;
	PackNum<sizeof...(SRanges)-1>::
	    template mkSliceBlocks<T,OperationRoot<T,ORanges...>,SRanges...>(blocks, mIndex, op);
	mSl.format(blocks);
	mSl.mData = op.data();
	return *this;
    }


} // end namespace MultiArrayTools

#endif
