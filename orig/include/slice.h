
#ifndef __cxz_slice_h__
#define __cxz_slice_h__

#include "cxz_array_base.h"
#include "cxz_operation.h"

namespace CNORXZ
{
    template <typename T, class... SRanges>
    class ConstSlice : public ArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<SRanges...> CRange;
	typedef ArrayBase<T,SRanges...> MAB;
	typedef ConstContainerIndex<T,typename SRanges::IndexType...> IType;

	using ArrayBase<T,SRanges...>::operator();
        using ArrayBase<T,SRanges...>::operator[];
	
	DEFAULT_MEMBERS(ConstSlice);

	ConstSlice(const std::tuple<std::shared_ptr<SRanges>...>& ranges,
		   const T* data = nullptr);
	ConstSlice(const std::shared_ptr<SRanges>&... ranges, const T* data = nullptr);
	ConstSlice(const ArrayBase<T,AnonymousRange>& ma, SIZET<SRanges>... sizes);

	virtual const T& operator[](const IType& i) const final;
	virtual const T& at(const typename IType::MetaType& meta) const override;
	
	virtual const T* data() const override;
	
	virtual bool isSlice() const override;

	virtual auto begin() const -> IType override;
	virtual auto end() const -> IType override;

	virtual std::shared_ptr<ArrayBase<T,AnonymousRange> > anonymous(bool slice = false) const override;
	
	auto define(const std::shared_ptr<typename SRanges::IndexType>&... inds)
	    -> ConstSliceDef<T,SRanges...>;
	
    private:
	friend ConstSliceDef<T,SRanges...>;

	void format(const std::array<size_t,sizeof...(SRanges)+1>& blocks);
	
	const T* mData;
    };

    
    template <typename T, class... SRanges>
    class Slice : public MutableArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<SRanges...> CRange;
	typedef ArrayBase<T,SRanges...> MAB;
	typedef ConstContainerIndex<T,typename SRanges::IndexType...> IType;

	using ArrayBase<T,SRanges...>::operator();
	using MutableArrayBase<T,SRanges...>::operator();
        using ArrayBase<T,SRanges...>::operator[];
	using MutableArrayBase<T,SRanges...>::operator[];

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

	virtual std::shared_ptr<ArrayBase<T,AnonymousRange> > anonymous(bool slice = false) const override;
	//virtual std::shared_ptr<ArrayBase<T,AnonymousRange> > anonymousMove() override;

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
	typedef ConstContainerIndex<T,typename SRanges::IndexType...> IType;

	template <class Op>
	static Slice<T,SRanges...> mkSlice( const typename Slice<T,SRanges...>::IndexType& ind,
					    const Op& op )
	{
	    Slice<T,SRanges...> out(ind->range()->space(), &*ind);
	    std::array<size_t,sizeof...(SRanges)+1> ff;
	    sfor_pn<0,sizeof...(SRanges)>
		( [&](auto i) {
		    std::get<i+1>(ff) =
			op.rootSteps(reinterpret_cast<std::intptr_t>
				     ( ind.template getPtr<i>().get())).val();
		    return 0; } );
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
	typedef ConstContainerIndex<T,typename SRanges::IndexType...> IType;

	template <class Op>
	static ConstSlice<T,SRanges...> mkSlice( const typename ConstSlice<T,SRanges...>::IndexType& ind,
						 const Op& op )
	{
	    ConstSlice<T,SRanges...> out(ind->range()->space(), &*ind);
	    std::array<size_t,sizeof...(SRanges)+1> ff;
	    sfor_pn<0,sizeof...(SRanges)>
		( [&](auto i) {
		    std::get<i+1>(ff) =
			op.rootSteps(reinterpret_cast<std::intptr_t>
				     ( ind.template getPtr<i>().get())).val();
		    return 0; } );
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

} // end namespace CNORXZ

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */


#endif
