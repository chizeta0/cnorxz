
#ifndef __cxz_dcontainer_index_h__
#define __cxz_dcontainer_index_h__

#include "range_base.h"
#include "index_base.h"
#include "statics/static_for.h"
#include "xfor/xfor.h"
#include "ranges/xindex.h"
#include "ranges/yindex.h"

namespace CNORXZ
{

    
    template <typename T>
    class DConstContainerIndex : public IndexInterface<DConstContainerIndex<T>,DType>
    {
    public:
	typedef IndexInterface<DConstContainerIndex<T>,DType> IB;

    protected:
	YIndexPtr mI;
	const T* mCData = nullptr;

    public:
	DEFAULT_MEMBERS(DConstContainerIndex);
	DConstContainerIndex(const T* data, const RangePtr& range);
	
    	DConstContainerIndex& operator=(size_t pos);
	DConstContainerIndex& operator++();
	DConstContainerIndex& operator--();

	template <class I, typename M> // fast but unsave
	DConstContainerIndex operator+(const IndexInterface<I,M>& i);
	
	template <class I, typename M> // fast but unsave
	DConstContainerIndex operator-(const IndexInterface<I,M>& i);
	
	template <class I, typename M> // save version of operator+
	DConstContainerIndex plus(const IndexInterface<I,M>& i);
	
	template <class I, typename M> // save version of operator-
	DConstContainerIndex minus(const IndexInterface<I,M>& i);
	
	int pp(std::intptr_t idxPtrNum);
	int mm(std::intptr_t idxPtrNum);
	size_t dim() const;
	size_t getStepSize(size_t n) const;
	std::string stringMeta() const;
	DType meta() const;
	DType metaPtr() const;
	DConstContainerIndex& at(const DType& meta);
	DynamicExpression ifor(size_t step, DynamicExpression ex) const;
	DynamicExpression iforh(size_t step, DynamicExpression ex) const;

	const T& operator*() const;
	const T* operator->() const;

	DConstContainerIndex& sync(); // recalculate 'IB::mPos' when externalControl == true
	DConstContainerIndex& operator()(const std::vector<XIndexPtr>& inds); // control via external indice
	DConstContainerIndex& operator()(); // -> sync; just to shorten the code
	
    };

    template <typename T>
    class DContainerIndex : public DConstContainerIndex<T>
    {
    public:
	typedef DConstContainerIndex<T> CCI;
	typedef typename CCI::IB IB;
	
    private:
	T* mData = nullptr;

    public:
	
	DEFAULT_MEMBERS(DContainerIndex);
	DContainerIndex(T* data, const RangePtr& range);
	DContainerIndex(T* data, const DConstContainerIndex<T>& cci);

	T& operator*();
	T* operator->();
    };
}

#endif
