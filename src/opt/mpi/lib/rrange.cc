// -*- C++ -*-
/**
   
   @file opt/mpi/lib/rrange.cc
   @brief RRange and RIndex implementations

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de
   
**/

#include "rrange.h"

namespace CNORXZ
{

    /*==============+
     |    RIndex    |
     +==============*/

    /*=====================+
     |    RRangeFactory    |
     +=====================*/

    RRangeFactory::RRangeFactory(const Vector<SizeT>& geom) :
	mRA( CRangeFactory(geom.size()), geom )
    {
	MPI_Comm_rank(MPI_COMM_WORLD, &mMyRank);
	int s = 0;
	MPI_Comm_size(MPI_COMM_WORLD, &s);
	CXZ_ASSERT(static_cast<int>(mRA.size()) == s, "geometry (size = " << mRA.size()
		   << " ) is inconsistent with number of mpi ranks ( = " << s << ")");
    }

    void RRangeFactory::make()
    {
	Vector<Uuid> k(mRA.size());
	std::transform(mRA.begin(), mRA.end(), k.begin(),
		       [&](const RangePtr& r) { return r->id(); } );
	mProd = this->fromCreated(typeid(RRange), k);
	if(mProd == nullptr){
	    mProd = std::shared_ptr<RRange>
		( new RRange( std::move(mRA) ) );
	    this->addToCreated(typeid(RRange), k, mProd);
	}
    }

    
    /*==============+
     |    RRange    |
     +==============*/

    RangePtr RRange::sub(SizeT i) const
    {
	return mRA.data()[i];
    }
    
    MArray<RangePtr> RRange::sub() const
    {
	return mRA;
    }
    
    SizeT RRange::size() const
    {
	CIndex i(mRA.range());
	SizeT o = 1;
	i->ifor( mRA(i), [&o](const RangePtr& r) { o *= r->size(); } )();
	return o;
    }
    
    SizeT RRange::dim() const
    {
	return mRA.range().size();
    }
    
    String RRange::stringMeta(SizeT pos) const
    {
	const String blim = "[";
	const String elim = "]";
	const String dlim = ",";
	String out = blim;
	CIndex i(mRA.range());
	SizeT block = mRA.size();
	i->ifor( mRA(i), [&pos,&out,&block](const RangePtr& r) {
	    const SizeT cursize = r->size();
	    block /= cursize;
	    const SizeT curpos = pos / block;
	    pos -= curpos * block;
	    out = out + r->stringMeta(curpos);
	    if(block == 1){
		assert(pos == 0);
		out = out + elim;
		break;
	    }
	    out = out + dlim;
	} )();
	return out;
    }
    
    const TypeInfo& RRange::type() const
    {
	return typeid(RRange);
    }
    
    const TypeInfo& RRange::metaType() const
    {
	return typeid(Vector<SizeT>);
    }
    
    RangePtr RRange::extend(const RangePtr& r) const
    {
	// number of ranks is fixed!!!
	return nullptr;
    }

    /*========================+
     |    RRange (private)    |
     +========================*/

    RRange::RRange(const MArray<RangePtr>& ra) :
	mRA( ra )
    {
	int s = 0;
	MPI_Comm_size(MPI_COMM_WORLD, &s);
	CXZ_ASSERT(static_cast<int>(mRA.size()) == s, "geometry (size = " << mRA.size()
		   << " ) is inconsistent with number of mpi ranks ( = " << s << ")");
    }

    Vector<Uuid> RRange::key() const
    {
	Vector<Uuid> k(mRA.size());
	std::transform(mRA.begin(), mRA.end(), k.begin(),
		       [&](const RangePtr& r) { return r->id(); } );
	return k;
    }
    
}
