// -*- C++ -*-
/**
   
   @file tests/test_numbers.h
   @brief Test numbers utilities.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#ifndef __test_numbers_h__
#define __test_numbers_h__

#include <fstream>
#include "base/base.h"

namespace CNORXZ
{
    namespace Test
    {
	/** @cond 0 */
	class Numbers
	{
	private:
	    static Vector<Double> sCont;
	    static void init()
	    {
		std::fstream f(TEST_NUMBER_FILE, f.in); // text file with simple format: size\nnum0\nnum1\n...
		CXZ_ASSERT(f.is_open(), "test number file '" << TEST_NUMBER_FILE << "'not found");
		SizeT nsize;
		f >> nsize;
		sCont.resize(nsize);
		for(SizeT i = 0; i != nsize; ++i){
		    f >> sCont[i];
		}
		CXZ_ASSERT(f.good(), "error while reading test number file '" << TEST_NUMBER_FILE << "'");
		f.close();
	    }	
	    
	public:

	    static auto begin()
	    {
		if(sCont.size() == 0) init();
		return sCont.cbegin();
	    }

	    static auto end()
	    {
		if(sCont.size() == 0) init();
		return sCont.cend();
	    }

	    static auto get(SizeT pos, SizeT size)
	    {
		auto b = begin()+pos;
		auto e = b+size;
		CXZ_ASSERT(e <= end(), "requested test numbers out of range");
		return Vector<Double>(b,e);
	    }
	};
	/** @endcond */
    }
}

#endif
