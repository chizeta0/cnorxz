
#ifndef __cxz_isq_h__
#define __cxz_isq_h__

#include <cstdlib>
#include <utility>
#include "types.h"

namespace CNORXZ
{
    template <SizeT O, SizeT... Is>
    std::index_sequence<(Is+O)...> mkIsqAdd(std::index_sequence<Is...> is) { return {}; }

    template <SizeT B, SizeT E>
    struct MkIsq
    {
	static auto make()
	{
	    static_assert(B <= E, "begin of sequence must be smaller than end of sequence");
	    return mkIsqAdd<B>( std::make_index_sequence<E-B>{} );
	}

	typedef decltype(make()) type;
    };
    
    template <SizeT B, SizeT E>
    using Isqr = typename MkIsq<B,E>::type;
}

#endif
