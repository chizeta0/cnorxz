// -*- C++ -*-

#include "ma_functional.h"

namespace MultiArrayTools
{

    template <class InRange, class OutRange>
    IndexMapFunction<InRange,OutRange>::
    IndexMapFunction(const MultiArrayBase<typename OutRange::IndexType,InRange>& ma,
		     const OutRange& outRange,
		     const Name& inName, const Name& outName) : mMap(ma, inName),
								mOutRange(new OutRange( outRange )),
								mOIndex(mOutRange->begin())
    {
	mOIndex.name(outName);
    }
    
    template <class InRange, class OutRange>
    void IndexMapFunction<InRange,OutRange>::linkIndicesTo(IndefinitIndexBase* target)
    {
	mMap.linkIndicesTo(target);
    }

    template <class InRange, class OutRange>
    void IndexMapFunction<InRange,OutRange>::eval() const
    {
	mOIndex.copyPos( mMap.get() );
    }

    template <class InRange, class OutRange>
    IndefinitIndexBase& IndexMapFunction<InRange,OutRange>::index() const
    {
	return mOIndex;
    }    


    /*
    vec3d2Function::vec3d2Function(std::shared_ptr<OutRange>& outRange) : mOutRange(outRange),
									  out(mOutRange->begin()) {} 
    */
    vec3d2Function::OutIndex vec3d2Function::operator()(const InIndex& i) const
    {
	OutSubIndex& osi = out.template getIndex<0>();
	
	osi.atMeta( i.template getIndex<0>().getMetaPos() * i.template getIndex<0>().getMetaPos() +
		    i.template getIndex<1>().getMetaPos() * i.template getIndex<1>().getMetaPos() +
		    i.template getIndex<2>().getMetaPos() * i.template getIndex<2>().getMetaPos() );
	return out;
    }
}
