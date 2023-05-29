
#ifndef __cxz_eindex_cc_h__
#define __cxz_eindex_cc_h__

#include "eindex.h"

namespace CNORXZ
{
    
    template <typename MetaT, SizeT S>
    EIndex<MetaT,S>::EIndex(const SIndex<MetaT,S>& i) :
	SIndex<MetaT,S>(i) {}

    template <typename MetaT, SizeT S>
    EIndex<MetaT,S>::EIndex(SIndex<MetaT,S>&& i) :
	SIndex<MetaT,S>(i) {}

    template <typename MetaT, SizeT S>
    template <class Xpr, class F>
    decltype(auto) EIndex<MetaT,S>::ifor(const Xpr& xpr, F&& f) const
    {
	// TODO: EFor Function argument; meanwhile ignore unused function error!!!
	return EFor<S,0,Xpr>(this->id(), xpr);
    }

}

#endif
