
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
    template <SizeT L1>
    auto LIndex<Index,L>::getStaticStepSize(PtrId iptr) const
    {
	if constexpr(L == 0 or L1 == 0){
	    return this->getStepSize(iptr);
	}
	else {
	    if constexpr(L == L1){
		CXZ_ASSERT(iptr == this->ptrId(),
			   "got two different indices with the same static label " << L);
		return SPos<1>();
	    }
	    else {
		CXZ_ASSERT(iptr == this->ptrId(),
			   "got two equal indices with the different static label " << L
			   << " and " << L1);
		return SPos<0>();
	    }
	}
    }

}
