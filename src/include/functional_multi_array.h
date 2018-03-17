
#ifndef __functional_multi_array__
#define __functional_multi_array__

#include "multi_array_base.h"

namespace MultiArrayTools
{
    
    template <typename T, class Function, class... SRanges>
    class FunctionalMultiArray : public MultiArrayBase<T,SRanges...>
    {
    public:

	typedef ContainerRange<T,SRanges...> CRange;
	typedef MultiArrayBase<T,CRange> MAB;
	typedef typename MultiArrayBase<T,CRange>::const_iterator const_iterator;
	typedef typename CRange::IndexType IndexType;
	
	DEFAULT_MEMBERS(FunctionalMultiArray);
	FunctionalMultiArray(const std::shared_ptr<SRanges>&... ranges, const Function& func);

	virtual const T& operator[](const IndexType& i) const override;

	virtual bool isConst() const override;	
	virtual bool isSlice() const override;

	// EVALUTAION CLASS ??!!!!
	
	//virtual ConstOperationRoot<T,SRanges...>
	//operator()(std::shared_ptr<typename SRanges::IndexType>&... inds) const override;
	
    private:
	mutable T mVal;
	Function mFunc;
    };

    
} // namespace MultiArrayTools

/* ========================= *
 * ---   TEMPLATE CODE   --- *
 * ========================= */

namespace MultiArrayTools
{

    /****************************
     *   FunctionalMultiArray   *	     
     ****************************/

    /*
    template <typename T, class Range, class Function>
    FunctionalMultiArray<T,Range,Function>::FunctionalMultiArray(const Range& range) :
	MultiArrayBase<T,SRanges...>(range), mFunc() {}
    */
    template <typename T, class Function, class... SRanges>
    FunctionalMultiArray<T,Function,SRanges...>::FunctionalMultiArray(const std::shared_ptr<SRanges>&... ranges,
								  const Function& func) :
	MultiArrayBase<T,SRanges...>(ranges...), mFunc(func) {}

    template <typename T, class Function, class... SRanges>
    const T& FunctionalMultiArray<T,Function,SRanges...>::operator[](const typename CRange::IndexType& i) const
    {
	mVal = mFunc(i.meta());
	return mVal;
    }

    template <typename T, class Function, class... SRanges>
    bool FunctionalMultiArray<T,Function,SRanges...>::isConst() const
    {
	return true;
    }

    template <typename T, class Function, class... SRanges>
    bool FunctionalMultiArray<T,Function,SRanges...>::isSlice() const
    {
	return false;
    }

} // namespace MultiArrayTools

#endif
