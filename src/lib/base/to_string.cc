// -*- C++ -*-
/**
   
   @file lib/base/to_string.cc
   @brief String converter implementations.

   Declaration of functions that convert a given object/type to a string.
   
   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#include "base/base.h"

namespace CNORXZ
{
    String ToString<String>::func(const String& a)
    {
	return a;
    }
    
    String ToString<DType>::func(const DType& a)
    {
	return a.str();
    }
}
