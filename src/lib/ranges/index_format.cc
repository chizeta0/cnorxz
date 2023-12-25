
#include "ranges/index_format.h"

namespace CNORXZ
{
    YFormat::YFormat(const Vector<UPos>& b) :
	mB(b)
    {}

    const Vector<UPos>& YFormat::all() const
    {
	return mB;
    }

    SizeT YFormat::size() const
    {
	return mB.size();
    }

    const UPos& YFormat::get(SizeT i) const
    {
	return mB[i];
    }

    const UPos& YFormat::operator[](SizeT i) const
    {
	return mB[i];
    }

    bool formatIsTrivial(const Vector<SizeT>& f, const Vector<SizeT>& s)
    {
	CXZ_ASSERT(f.size() == s.size(), "got vectors with different size ("
		   << f.size() << " vs " << s.size() << ")");
	SizeT x = 1;
	for(SizeT i_ = f.size(); i_ != 0; --i_) {
	    const SizeT i = i_-1;
	    if(f[i] != x) {
		return false;
	    }
	    x *= s[i];
	}
	return true;
    }
}
