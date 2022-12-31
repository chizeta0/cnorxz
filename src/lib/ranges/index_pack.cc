
#include "ranges/index_pack.h"

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
    
    DPack DPack::rmul(const XIndexPtr& i) const
    {
	auto o = mIs;
	o.push_back(i);
	return DPack(std::move(o));
    }
    
    DPack DPack::lmul(const XIndexPtr& i) const
    {
	Vector<XIndexPtr> o;
	o.reserve(size()+1);
	o.push_back(i);
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

}
