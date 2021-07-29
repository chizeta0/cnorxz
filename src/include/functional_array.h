
#ifndef __cxz_functional_array__
#define __cxz_functional_array__

#include "cxz_array_base.h"
#include "slice.h"

namespace CNORXZ
{
    
    template <typename T, class Function, class... SRanges>
    class FunctionalArray : public ArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<SRanges...> CRange;
	typedef ArrayBase<T,SRanges...> MAB;
	typedef ConstContainerIndex<T,typename SRanges::IndexType...> IndexType;
	typedef Array<T,SRanges...> MAType;
	typedef T value_type;
	
    private:
	mutable T mVal;
	std::shared_ptr<Function> mFunc;

	mutable std::shared_ptr<MAType> mMaPtr;

    public:
	
	DEFAULT_MEMBERS(FunctionalArray);
	FunctionalArray(const std::shared_ptr<SRanges>&... ranges, const std::shared_ptr<Function>& func);
	FunctionalArray(const std::shared_ptr<SRanges>&... ranges);
	FunctionalArray(const typename CRange::Space& space);
	FunctionalArray(const typename CRange::Space& space, const std::shared_ptr<Function>& func);
	
	virtual const T& operator[](const IndexType& i) const override;
	virtual const T& at(const typename CRange::IndexType::MetaType& meta) const override;
	virtual const T* data() const override;
	
	virtual bool isConst() const override;	
	virtual bool isSlice() const override;

	virtual std::shared_ptr<ArrayBase<T,AnonymousRange> > anonymous(bool slice = false) const override;

	auto exec(const std::shared_ptr<typename SRanges::IndexType>&... inds) const
            -> Operation<T,Function,MetaOperationRoot<SRanges>...>;
            
	virtual ConstOperationRoot<T,SRanges...>
	operator()(const std::shared_ptr<typename SRanges::IndexType>&... inds) const override;
	
    };

  
} // namespace CNORXZ

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */


#endif
