
#ifndef __cxz_index_id_cc_h__
#define __cxz_index_id_cc_h__

#include "index_id.h"

namespace CNORXZ
{

    template <SizeT I>
    constexpr IndexId<I>::IndexId(PtrId id) :
	mId(id)
    {}

    template <SizeT I>
    constexpr const PtrId& IndexId<I>::id() const
    {
	return mId;
    }

    template <SizeT I>
    template <SizeT J>
    constexpr decltype(auto) IndexId<I>::operator==(const IndexId<J>& a) const
    {
	if constexpr(I != 0 and J != 0){
	    if constexpr(I == J){
		CXZ_ASSERT(mId == a.id(),
			   "got different index ids for equal static index label");
		return true;
	    }
	    else {
		CXZ_ASSERT(mId == a.id(),
			   "got equal index ids for different static index label");
		return false;
	    }
	}
	else {
	    return mId == a.id();
	}
    }

    template <SizeT I>
    template <SizeT J>
    constexpr decltype(auto) IndexId<I>::operator!=(const IndexId<J>& a) const
    {
	if constexpr(I != 0 and J != 0){
	    if constexpr(I == J){
		CXZ_ASSERT(mId == a.id(),
			   "got different index ids for equal static index label");
		return false;
	    }
	    else {
		CXZ_ASSERT(mId == a.id(),
			   "got equal index ids for different static index label");
		return true;
	    }
	}
	else {
	    return mId != a.id();
	}
    }
    
}

#endif
