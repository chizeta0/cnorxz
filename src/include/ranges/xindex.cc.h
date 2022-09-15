
#ifndef __cxz_xindex_cc_h__
#define __cxz_xindex_cc_h__

#include "xindex.h"

namespace CNORXZ
{
    /**************
     *   XIndex   *
     **************/
    
    template <class Index, typename Meta>
    XIndex<Index,Meta>::XIndex(const IndexPtr<Index,Meta>& i) : mI(i) {}

    template <class Index, typename Meta>
    XIndex<Index,Meta>::XIndex(const IndexInterface<Index,Meta>& i) :
	mI(std::make_shared<Index>(i.THIS())) {}

    template <class Index, typename Meta>
    XIndex<Index,Meta>& XIndex<Index,Meta>::operator=(SizeT pos)
    {
	*mI = pos;
	return *this;
    }

    template <class Index, typename Meta>
    XIndex<Index,Meta>& XIndex<Index,Meta>::operator++()
    {
	++(*mI);
	return *this;
    }

    template <class Index, typename Meta>
    XIndex<Index,Meta>& XIndex<Index,Meta>::operator--()
    {
	--(*mI);
	return *this;
    }

    template <class Index, typename Meta>
    Int XIndex<Index,Meta>::pp(PtrId idxPtrNum)
    {
	return mI->pp(idxPtrNum);
    }

    template <class Index, typename Meta>
    Int XIndex<Index,Meta>::mm(PtrId idxPtrNum)
    {
	return mI->mm(idxPtrNum);
    }

    template <class Index, typename Meta>
    SizeT XIndex<Index,Meta>::dim() const
    {
	return mI->dim();
    }

    template <class Index, typename Meta>
    SizeT XIndex<Index,Meta>::getStepSize(SizeT n) const
    {
	return mI->getStepSize(n);
    }

    template <class Index, typename Meta>
    String XIndex<Index,Meta>::stringMeta() const
    {
	return mI->stringMeta();
    }

    template <class Index, typename Meta>
    DType XIndex<Index,Meta>::meta() const
    {
	return DType(mI->meta());
    }

    template <class Index, typename Meta>
    XIndexBase& XIndex<Index,Meta>::at(const DType& meta)
    {
	// check!!!
	mI->at(std::any_cast<const Meta&>(meta.get()));
	return *this;
    }
    /*
    template <class Index, typename Meta>
    DExpr XIndex<Index,Meta>::ifor(SizeT step, DExpr ex) const
    {
	return mI->ifor(step, ex);
    }

    template <class Index, typename Meta>
    DExpr XIndex<Index,Meta>::iforh(SizeT step, DExpr ex) const
    {
	return mI->iforh(step, ex);
    }
    */
}

#endif
