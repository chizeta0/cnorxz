// -*- C++ -*-
/**
   
   @file include/xpr/index_id.h
   @brief Index ID type declarations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_index_id_h__
#define __cxz_index_id_h__

#include "base/base.h"

namespace CNORXZ
{

    // if I == 0: no static comparison; fall back to usual runtime comparison of mId
    template <SizeT I = 0>
    class IndexId
    {
    private:
	PtrId mId = 0;

    public:
	DEFAULT_MEMBERS(IndexId);

	explicit constexpr IndexId(PtrId id);

	constexpr const PtrId& id() const;
	
	template <SizeT J>
	constexpr decltype(auto) operator==(const IndexId<J>& a) const;

	template <SizeT J>
	constexpr decltype(auto) operator!=(const IndexId<J>& a) const;
    };

    template <class T>
    struct static_index_id
    {
	static constexpr SizeT value = 0;
    };

    template <SizeT I>
    struct static_index_id<IndexId<I>>
    {
	static constexpr SizeT value = I;
    };
    
}

#endif
