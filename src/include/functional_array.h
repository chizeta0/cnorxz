
#ifndef __functional_array__
#define __functional_array__

#include "cxz_array_base.h"
#include "slice.h"

namespace CNORXZ
{
    
    template <typename T, class Function, class... SRanges>
    class FunctionalMultiArray : public MultiArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<SRanges...> CRange;
	typedef MultiArrayBase<T,SRanges...> MAB;
	typedef ConstContainerIndex<T,typename SRanges::IndexType...> IndexType;
	typedef MultiArray<T,SRanges...> MAType;
	typedef T value_type;
	
    private:
	mutable T mVal;
	std::shared_ptr<Function> mFunc;

	mutable std::shared_ptr<MAType> mMaPtr;

    public:
	
	DEFAULT_MEMBERS(FunctionalMultiArray);
	FunctionalMultiArray(const std::shared_ptr<SRanges>&... ranges, const std::shared_ptr<Function>& func);
	FunctionalMultiArray(const std::shared_ptr<SRanges>&... ranges);
	FunctionalMultiArray(const typename CRange::Space& space);
	FunctionalMultiArray(const typename CRange::Space& space, const std::shared_ptr<Function>& func);
	
	virtual const T& operator[](const IndexType& i) const override;
	virtual const T& at(const typename CRange::IndexType::MetaType& meta) const override;
	virtual const T* data() const override;
	
	virtual bool isConst() const override;	
	virtual bool isSlice() const override;

	virtual std::shared_ptr<MultiArrayBase<T,AnonymousRange> > anonymous(bool slice = false) const override;

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
