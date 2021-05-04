
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
        using MultiArrayBase<T,SRanges...>::operator[];
	
	DEFAULT_MEMBERS(ConstSlice);

	ConstSlice(const std::tuple<std::shared_ptr<SRanges>...>& ranges,
		   const T* data = nullptr);
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
        using MultiArrayBase<T,SRanges...>::operator[];
	using MutableMultiArrayBase<T,SRanges...>::operator[];

	DEFAULT_MEMBERS(Slice);

	Slice(const std::tuple<std::shared_ptr<SRanges>...>& ranges,
	      T* data = nullptr);
	Slice(const std::shared_ptr<SRanges>&... ranges, T* data = nullptr);

        Slice& operator=(T val);
        
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
	//virtual std::shared_ptr<MultiArrayBase<T,AnonymousRange> > anonymousMove() override;

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
	    MA_SFOR(i,0,sizeof...(SRanges),i+1,
		std::get<i+1>(ff) = op.rootSteps(reinterpret_cast<std::intptr_t>
						 ( ind.template getPtr<i>().get())).val(););
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
	    MA_SFOR(i,0,sizeof...(SRanges),i+1,
		std::get<i+1>(ff) = op.rootSteps(reinterpret_cast<std::intptr_t>
						 ( ind.template getPtr<i>().get())).val(););
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


#endif
