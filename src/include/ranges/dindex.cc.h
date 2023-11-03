// -*- C++ -*-
/**
   
   @file include/ranges/dindex.cc.h
   @brief ...


   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __cxz_dindex_cc_h__
#define __cxz_dindex_cc_h__

#include "dindex.h"

namespace CNORXZ
{
    template <class Index, typename Meta>
    DIndex::DIndex(const IndexInterface<Index,Meta>& i) :
	IndexInterface<DIndex,DType>(i.pos()),
	mI(std::make_shared<XIndex<Index,Meta>>(i))
    {}
}

#endif
