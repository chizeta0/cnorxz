// -*- C++ -*-

#ifndef __ma_functional_h__
#define __ma_functional_h__

#include <cstdlib>
#include <tuple>
#include <utility>

#include "base_def.h"

namespace MultiArrayTools
{

    // Maps... are ConstMultiArrayOperationRoots where the corresponding MultiArray defines the map
    template <class OutIndex, class... Maps>
    class IndexMapFunction
    {
    public:

	typedef std::tuple<Maps...> MapType;
	
	void linkIndicesTo(IndefinitIndexBase* target);
	void eval();
	
	IndefinitIndexBase& index();
	
    private:
	
	MapType mMap;
	OutIndex mOIndex;
    };

}

#include "ma_functional.cc"

#endif
