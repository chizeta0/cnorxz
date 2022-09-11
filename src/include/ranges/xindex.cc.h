
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
    XIndex<Index,Meta>& XIndex<Index,Meta>::operator=(size_t pos)
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
    int XIndex<Index,Meta>::pp(std::intptr_t idxPtrNum)
    {
	return mI->pp(idxPtrNum);
    }

    template <class Index, typename Meta>
    int XIndex<Index,Meta>::mm(std::intptr_t idxPtrNum)
    {
	return mI->mm(idxPtrNum);
    }

    template <class Index, typename Meta>
    size_t XIndex<Index,Meta>::dim() const
    {
	return mI->dim();
    }

    template <class Index, typename Meta>
    size_t XIndex<Index,Meta>::getStepSize(size_t n) const
    {
	return mI->getStepSize(n);
    }

    template <class Index, typename Meta>
    std::string XIndex<Index,Meta>::stringMeta() const
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

    template <class Index, typename Meta>
    DynamicExpression XIndex<Index,Meta>::ifor(size_t step, DynamicExpression ex) const
    {
	return mI->ifor(step, ex);
    }

    template <class Index, typename Meta>
    DynamicExpression XIndex<Index,Meta>::iforh(size_t step, DynamicExpression ex) const
    {
	return mI->iforh(step, ex);
    }

}

#endif
