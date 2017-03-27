// -*- C++ -*-

#include "ma_functional.h"

namespace MultiArrayTools
{
    namespace
    {

	template <size_t N>
	struct MapEvaluation
	{
	    template <class OutIndex, class MapTuple>
	    static void eval(OutIndex& oi, const MapTuple& mt)
	    {
		oi.template getIndex<N>() = std::get<N>(mt);
		MapEvaluation<N-1>::eval(oi, mt);
	    }
	};
	
	template <>
	struct MapEvaluation<0>
	{
	    template <class OutIndex, class MapTuple>
	    static void eval(OutIndex& oi, const MapTuple& mt)
	    {
		oi.template getIndex<0>() = std::get<0>(mt);
	    }
	};
	
    } // anonymous namespace

    
    template <class OutIndex, class... Maps>
    void IndexMapFunction<OutIndex,Maps...>::linkIndicesTo(IndefinitIndexBase* target)
    {
	/*!!!!*/
    }

    template <class OutIndex, class... Maps>
    void IndexMapFunction<OutIndex,Maps...>::eval()
    {
	MapEvaluation<sizeof...(Maps)-1>::eval(mOIndex, mMap);
    }

    template <class OutIndex, class... Maps>
    IndefinitIndexBase& IndexMapFunction<OutIndex,Maps...>::index()
    {
	return mOIndex;
    }    
    
}
