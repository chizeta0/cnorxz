// -*- C++ -*-
/**

   @file opt/cereal/include/range_save_load.h
   @brief CNORXZ Cereal range save load declaration.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

 **/

#ifndef __range_save_load_h__
#define __range_save_load_h__

#include "base/base.h"

namespace CNORXZ
{
    namespace cer
    {
	/** Deserialize and register range from cereal archive.
	    Always use this function to load ranges from archives so that it is
	    registered in the map of the range factories. Otherwise there might be more
	    than one instance of the same range.
	    @param ar Archive to read from.
	    @param name Archive key.
	    @param r Target range pointer.
	*/
	template <class Archive>
	RangePtr save_load(Archive& ar, const String& name, RangePtr& r);
    }
}

#endif
