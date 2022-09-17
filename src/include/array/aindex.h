
#ifndef __cxz_aindex_h__
#define __cxz_aindex_h__

#include "ranges/range_base.h"
#include "ranges/index_base.h"
#include "ranges/xfor/xfor.h"
#include "ranges/xindex.h"
#include "ranges/yrange.h"

namespace CNORXZ
{

    // rename: AIndex (A = Array)
    // implementation similar to YIndex
    template <typename T>
    class AIndex : public IndexInterface<AIndex<T>,DType>
    {
    public:
	typedef IndexInterface<AIndex<T>,DType> IB;

	DEFAULT_MEMBERS(AIndex);
	AIndex(const T* data, const RangePtr& range, SizeT pos = 0);
	
	AIndex& sync(); // recalculate 'IB::mPos' when externalControl == true
	AIndex& operator()(const Vector<XIndexPtr>& inds); // control via external indice
	AIndex& operator()(); // -> sync; just to shorten the code

    	AIndex& operator=(SizeT pos);
	AIndex& operator++();
	AIndex& operator--();
	AIndex operator+(Int n) const;
	AIndex operator-(Int n) const;
	AIndex& operator+=(Int n);
	AIndex& operator-=(Int n);
	
	const T& operator*() const;
	const T* operator->() const;

	Int pp(PtrId idxPtrNum);
	Int mm(PtrId idxPtrNum);

	SizeT dim() const;
	RangePtr range() const;
	SizeT getStepSize(SizeT n) const;

	String stringMeta() const;
	DType meta() const;
	AIndex& at(const DType& meta);

	//DExpr ifor(SizeT step, DExpr ex) const;
	//DExpr iforh(SizeT step, DExpr ex) const;

    protected:
	Sptr<YRange> mRangePtr;
	Vector<XIndexPtr> mIs;
	Vector<SizeT> mBlockSizes; // dim() elements only!!!
	bool mExternalControl = false;
	const T* mCData = nullptr;

    };

    // BIndex (because B comes after A...)
    template <typename T>
    class BIndex : public AIndex<T>
    {
    public:
	typedef AIndex<T> AI;
	typedef typename AI::IB IB;
	
	DEFAULT_MEMBERS(BIndex);
	BIndex(T* data, const RangePtr& range, SizeT pos = 0);
	BIndex(T* data, const AIndex<T>& cci, SizeT pos = 0);

	BIndex operator+(Int n) const;
	BIndex operator-(Int n) const;

	T& operator*();
	T* operator->();

    private:
	T* mData = nullptr;

    };
}

#endif
