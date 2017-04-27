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
	//CHECK;
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


    
    vec3d2Function::vec3d2Function(const OutRange& outRange) : mOutRange(new OutRange( outRange )),
							       out(mOutRange->begin()) {} 
    
    vec3d2Function::OutIndex vec3d2Function::operator()(const InIndex& i) const
    {
	OutSubIndex& osi = out.template getIndex<0>();
	
	osi.atMeta( i.template getIndex<0>().getMetaPos() * i.template getIndex<0>().getMetaPos() +
		    i.template getIndex<1>().getMetaPos() * i.template getIndex<1>().getMetaPos() +
		    i.template getIndex<2>().getMetaPos() * i.template getIndex<2>().getMetaPos() );
	return out;
    }

    pyProjFunction::pyProjFunction(const OutRange& outRange) : mOutRange(new OutRange( outRange )),
							       out(mOutRange->begin()) {} 

    pyProjFunction::OutIndex pyProjFunction::operator()(const InIndex& i) const
    {
	DistIndex& di = out.template getIndex<0>();
	ScalProdIndex& si = out.template getIndex<1>();
	const int xx = i.template getIndex<1>().getMetaPos();
	const int yy = i.template getIndex<2>().getMetaPos();
	const int zz = i.template getIndex<3>().getMetaPos();
	if(xx == 0 or yy == 0 or zz == 0){ // anistotropy in C2
	    di.atMeta(0);
	    si.atMeta(0);
	}
	
	di.atMeta( xx * xx + yy * yy + zz * zz );
	si.atMeta( abs( xx * i.template getIndex<0>().getMetaPos()[0] +
			yy * i.template getIndex<0>().getMetaPos()[1] +
			zz * i.template getIndex<0>().getMetaPos()[2] ) );
	return out;
    }

    pyProjNoSymFunction::pyProjNoSymFunction(const OutRange& outRange) : mOutRange(new OutRange( outRange )),
									 out(mOutRange->begin()) {} 

    pyProjNoSymFunction::OutIndex pyProjNoSymFunction::operator()(const InIndex& i) const
    {
	DistIndex& di = out.template getIndex<0>();
	ScalProdIndex& si = out.template getIndex<1>();
	const int xx = i.template getIndex<1>().getMetaPos();
	const int yy = i.template getIndex<2>().getMetaPos();
	const int zz = i.template getIndex<3>().getMetaPos();
	if(xx == 0 or yy == 0 or zz == 0){ // anistotropy in C2
	    di.atMeta(0);
	    si.atMeta(0);
	}
	
	di.atMeta( xx * xx + yy * yy + zz * zz );
	si.atMeta( xx * i.template getIndex<0>().getMetaPos()[0] +
		   yy * i.template getIndex<0>().getMetaPos()[1] +
		   zz * i.template getIndex<0>().getMetaPos()[2] );
	return out;
    }
}
