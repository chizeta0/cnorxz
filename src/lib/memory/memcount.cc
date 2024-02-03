// -*- C++ -*-
/**
   
   @file lib/memory/memcount.cc
   @brief Implementation of MemCount member functions.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
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
