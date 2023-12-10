// -*- C++ -*-
/**
   
   @file lib/memory/memcount.cc
   @brief ...


   Copyright (c) 2022 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#include "memory/memcount.h"

namespace CNORXZ
{
    SizeT MemCount::sMemUsage = 0;

    void MemCount::add(SizeT x)
    {
	sMemUsage += x;
    }
    
    void MemCount::sub(SizeT x)
    {
	sMemUsage -= x;
    }

    SizeT MemCount::usage()
    {
	return sMemUsage;
    }

}
