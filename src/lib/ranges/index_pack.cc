// -*- C++ -*-
/**
   
   @file lib/ranges/index_pack.cc
   @brief Index pack implementations.

   Copyright (c) 2024 Christian Zimmermann. All rights reserved.
   Mail: chizeta@f3l.de

**/

#include "ranges/ranges.h"

namespace CNORXZ
{
    DPack::DPack(const Vector<XIndexPtr>& is) :
	mIs(is)
    {}

    DPack::DPack(Vector<XIndexPtr>&& is) :
	mIs(std::forward<Vector<XIndexPtr>>(is))
    {}

    const Vector<XIndexPtr>& DPack::all() const
    {
	return mIs;
    }
    
    SizeT DPack::size() const
    {
	return mIs.size();
    }

    const XIndexPtr& DPack::get(SizeT i) const
    {
	return mIs[i];
    }
    
    const XIndexPtr& DPack::operator[](SizeT i) const
    {
	return get(i);
    }
    
    DPack DPack::rmul(const Sptr<DIndex>& i) const
    {
	auto o = mIs;
	o.push_back(i->xptr());
	return DPack(std::move(o));
    }
    
    DPack DPack::lmul(const Sptr<DIndex>& i) const
    {
	Vector<XIndexPtr> o;
	o.reserve(size()+1);
	o.push_back(i->xptr());
	o.insert(o.end(), mIs.begin(), mIs.end());
	return DPack(std::move(o));
    }
    
    DPack DPack::mul(const DPack& p) const
    {
	Vector<XIndexPtr> o;
	o.reserve(size()+p.size());
	o.insert(o.end(), mIs.begin(), mIs.end());
	o.insert(o.end(), p.all().begin(), p.all().end());
	return DPack(std::move(o));
    }

    RangePtr DPack::mkRange() const
    {
	Vector<RangePtr> o(mIs.size());
	std::transform(mIs.begin(), mIs.end(), o.begin(),
		       [](const auto& i) { return i->range(); } );
	return yrange(o);
    }
    
    SizeT DPack::lex() const
    {
	if(mIs.size() == 0) { return 0; }
	const SizeT isizem1 = mIs.size()-1;
	SizeT o = mIs[isizem1]->lex();
	SizeT m = mIs[isizem1]->lmax().val();
	for(SizeT i = isizem1; i != 0; --i){
	    const SizeT j = i-1;
	    o += mIs[j]->lex() * m;
	    m *= mIs[j]->lmax().val();
	}
	return o;
    }

    SizeT DPack::pos() const
    {
	return lex();
    }
}
