
#ifndef __cxz_dcontainer_index_h__
#define __cxz_dcontainer_index_h__

#include "ranges/range_base.h"
#include "ranges/index_base.h"
#include "ranges/xfor/xfor.h"
#include "ranges/xindex.h"
#include "ranges/yrange.h"

namespace CNORXZ
{

    // rename: AIndex (A = Array)
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
	
    	DConstContainerIndex& operator=(SizeT pos);
	DConstContainerIndex& operator++();
	DConstContainerIndex& operator--();
	
	int pp(PtrId idxPtrNum);
	int mm(PtrId idxPtrNum);
	SizeT dim() const;
	SizeT getStepSize(SizeT n) const;
	String stringMeta() const;
	DType meta() const;
	DType metaPtr() const;
	DConstContainerIndex& at(const DType& meta);
	//DExpr ifor(SizeT step, DExpr ex) const;
	//DExpr iforh(SizeT step, DExpr ex) const;

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
