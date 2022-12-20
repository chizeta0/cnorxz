
#ifndef __cxz_lindex_cc_h__
#define __cxz_lindex_cc_h__

#include "lindex.h"

namespace CNORXZ
{
    template <class Index, SizeT L>
    LIndex<Index,L>::LIndex(const Index& i) :
	Index(i) {}

    template <class Index, SizeT L>
    LIndex<Index,L>::LIndex(Index&& i) :
	Index(i) {}

    template <class Index, SizeT L>
    IndexId<L> LIndex<Index,L>::id() const
    {
	return IndexId<L>(this->ptrId());
    }

    template <class Index, SizeT L>
    template <SizeT I>
    decltype(auto) LIndex<Index,L>::stepSize(const IndexId<I>& id) const
    {
	if constexpr(L == 0 or I == 0){
	    return UPos(this->id() == id ? 1 : 0);
	}
	else {
	    return this->id() == id ? SPos<1>() : SPos<0>();
	}
    }

}

#endif
